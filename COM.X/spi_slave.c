/******************************************************************************************
 * spi_slave.c
 * ver1.00
 * Hirofumi Hamada
 *=========================================================================================
 * PIC16F877A用SPI通信(Slave)ライブラリソースファイル
 *
 *=========================================================================================
 * ・ver1.00 || 2015/12/28 || Hirofumi Hamada
 *   初版作成
 * ・ver2.00 || 2016/03/06 || Hirofumi Hamada
 *   send と receiveに関数を分けた NOTIFICATIONピンの追加
 *=========================================================================================
 * PIC16F877A
 * MPLAB X IDE(V3.10/Ubuntu)
 * XC8 (V1.35/Linux)
 *=========================================================================================
 * Created by fabsat Project(Tokai university Satellite Project[TSP])
 *****************************************************************************************/
#include <xc.h>
#include "spi_slave.h"
#include "pic_clock.h"
#include "system_protocol.h"


/*=====================================================
 * @brief
 *     SPI Slave初期化関数
 * @param
 *     spi_isr:spi割り込みの有効無効を設定
 * @return
 *     なし
 * @note
 *     Pin43:SDO(RC5)
 *     Pin42:SDI(RC4/SDA)
 *     Pin37:SCK(RC3/SCL)
 *     Pin24:SS (RA5/AN4/C2OUT)
 *===================================================*/
void spi_slave_start(spi_isr_set_t spi_isr)
{
    /* SPI通信で使用するピンの設定 */
    TRISC3 = 1;               // RC3 = SCK -> INPUT
    TRISC4 = 1;               // RC4 = SDI -> INPUT
    TRISC5 = 0;               // RC5 = SDO -> OUTPUT
    TRISA5 = 1;               // RA5 = SS  -> INPUT
    ADCON0bits.ADON  = 0;     // A/Dコンバータ無効化 ※統合の際には注意
    ADCON1bits.PCFG3 = 0;     // RA5はアナログOff SSピン制御を使用するためにはここを設定しなければいけない
    ADCON1bits.PCFG2 = 1;
    ADCON1bits.PCFG1 = 1;

    /* SPIのレジスタ設定のためにSSPを無効にする */
    SSPCONbits.SSPEN = 0;

    /*  SPIモード設定 */
    SSPSTATbits.SMP = 0;    // SMP must be cleared when SPI is used in mode
    SSPCONbits.CKP  = 0;    // Idle state for clock is a low level
    SSPSTATbits.CKE = 0;    // Transmit occurs on transition from idle to active clock state
    
    /* SPI Slave mode, clock = SCK pin. SS pin control enabled. */
    SSPCONbits.SSPM3 = 0;
    SSPCONbits.SSPM2 = 1;
    SSPCONbits.SSPM1 = 0;
    SSPCONbits.SSPM0 = 0;

    /* Enable SPI Interrupt */
    if(spi_isr == SPI_ISR_ENABLE)
    {
        PIR1bits.SSPIF  = 0;
        PIE1bits.SSPIE  = 1;     // SPI Interrupt enable
        INTCONbits.PEIE = 1;
        INTCONbits.GIE  = 1;        
    }

    /* NOTIFICATIONピンをLow */
    NOTIFICATION_TO_OBC1 = 0;
    NOTIFICATION_TO_OBC2 = 0;
    
    /* End SPI programming and Start serial port */
    SSPCONbits.SSPEN = 1;    // Enables serial port and configures SCK, SDO, SDI, and SS as serial port pins
}


/*=====================================================
 * @brief
 *     SPIデータ受信関数(1Byte)
 * @param
 *     destination:通信の相手先
 * @return
 *     SSPBUF:受信データ
 * @note
 *     none
 *===================================================*/
uint8_t spi_slave_receive(destination_t destination)
{
    uint16_t timeout_counter = 60000;

    /* SSPBUFにダミーデータを書き込む */
    SSPBUF = 0x00;

    /* 通信準備完了を通信相手に通知する */
    switch(destination)
    {
	case OBC1:
            NOTIFICATION_TO_OBC1 = 1;
            break;
        case OBC2:
            NOTIFICATION_TO_OBC2 = 1;
            break;
    }
    
    /* 受信完了待ち */
    while(SSPSTATbits.BF == 0)
    {
        /* TIMEOUT (60[ms]を超えた時) */
        if(timeout_counter == 0)
        {
            return 0xff;
        }

        /* 1[us]ごとにtimeout_counterをデクリメントする。 */
        __delay_us(1);
        timeout_counter--;
    }

    /* 通信準備完了通知をLowにする */
    switch(destination)
    {
	case OBC1:
            NOTIFICATION_TO_OBC1 = 0;
            break;
        case OBC2:
            NOTIFICATION_TO_OBC2 = 0;
            break;
    }
    
    return SSPBUF;    
}


/*=====================================================
 * @brief
 *     SPI Slaveデータ送信関数(1Byte)
 * @param
 *     destination:通信の相手先
 *     data       :送信データ(1Byte)
 * @return
 *     void:
 * @note
 *     none
 *===================================================*/
void spi_slave_send(destination_t destination, uint8_t data)
{
    uint16_t timeout_counter = 60000;
    uint8_t dummy;

    /* 念のためSSPBUFからダミー変数にデータを抜いておく */
    dummy = SSPBUF;

    /* 送信データを書き込む */
    SSPBUF = data;

    /* 通信準備完了を通信相手に通知する */
    switch(destination)
    {
	case OBC1:
            NOTIFICATION_TO_OBC1 = 1;
            break;
        case OBC2:
            NOTIFICATION_TO_OBC2 = 1;
            break;
    }

    /* 受信完了待ち */
    while(SSPSTATbits.BF == 0)
    {
        /* TIMEOUT (60[ms]を超えた時) */
        if(timeout_counter == 0)
        {
            return;
        }

        /* 1[us]ごとにtimeout_counterをデクリメントする。 */
        __delay_us(1);
        timeout_counter--;
    }

    /* 通信準備完了通知をLowにする */
    switch(destination)
    {
	case OBC1:
            NOTIFICATION_TO_OBC1 = 0;
            break;
        case OBC2:
            NOTIFICATION_TO_OBC2 = 0;
            break;
    }
}


/*=====================================================
 * @brief
 *     SPI Slave無効化関数
 * @param
 *     void:
 * @return
 *     void:
 * @note
 *     none
 *===================================================*/
void spi_slave_end(void)
{
    /* SPI割込みの無効化 */
    PIR1bits.SSPIF  = 0;
    PIE1bits.SSPIE  = 0;     // SPI割込み無効
    
    /* SPIを無効にする */        
    SSPCONbits.SSPEN = 0;
}



/*=====================================================
 * @brief
 *     SPIデータ送受信関数(1Byte)
 * @param
 *     sent_data:送信データ
 * @return
 *     received_data:受信データwait for finish
 * @note
 *     none
 *===================================================*/
/*
uint8_t spi_send_receive(uint8_t sent_data)
{
    uint8_t received_data;

    SSPBUF = sent_data;


    while(SSPSTATbits.BF == 0)
    {
        ;        
    }

    received_data = SSPBUF;
    SSPBUF = 0xFF;

    return received_data;
}
*/
