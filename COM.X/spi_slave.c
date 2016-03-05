/******************************************************************************************
 * spi_slave.c
 * ver1.00
 * Hirofumi Hamada
 *=========================================================================================
 * PIC16F877A用SPI通信(Slave)ライブラリソースファイル
 *
 *=========================================================================================
 * ・ver1.00 || 初版 || 2015/12/28 || Hirofumi Hamada
 *   初版作成
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


/* static関数のプロトタイプ宣言 */
static void spi_reset(void);


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

    /* NITIFICATIONピンをLow */
    NOTIFICATION_TO_OBC1 = 0;
    NOTIFICATION_TO_OBC2 = 0;

    /* End SPI programming and Start serial port */
    SSPCONbits.SSPEN = 1;    // Enables serial port and configures SCK, SDO, SDI, and SS as serial port pins
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
uint8_t spi_send_receive(uint8_t sent_data)
{
    uint8_t received_data;

    /* transmit data into SSPBUF */
    SSPBUF = sent_data;

    /* Waiting for SPI finish */
    while(SSPSTATbits.BF == 0)
    {
        ;        
    }

    /* Get received data from register */
    received_data = SSPBUF;
    SSPBUF = 0xFF;

    return received_data;
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
 *     SPI Interrupt function
 * @param
 *     none:
 * @return
 *     none:
 * @note
 *     none
 *===================================================*/
void spi_interrupt(void)
{
    uint8_t data;

    data = SSPBUF;
    put_char(data);
    put_string("\r\n");
}


/*-----------------------------------------------------
 * @brief
 *     SPIリセット関数
 * @param
 *     なし
 * @return
 *     なし
 * @note
 *     待ち状態のreset_counterによって呼び出される
 *---------------------------------------------------*/
static void spi_reset(void)
{
    unsigned char dummy;
    
    SSPEN = 0;         //  Reset SPI module
    SSPEN = 1;         //  Reset SPI module
    dummy = SSPBUF;
    SSPIF = 0;
    SSPEN = 0;         //  Reset SPI module
    SSPEN = 1;         //  Reset SPI module
}
