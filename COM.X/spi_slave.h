/******************************************************************************************
 * spi_slave.h
 * ver1.00
 * Hirofumi Hamada
 *=========================================================================================
 * PIC16F877A用SPI Slaveライブラリヘッダファイル
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

#ifndef _SPI_SLAVE_H
#define	_SPI_SLAVE_H

#include <xc.h>
#include "pic_clock.h"
#include "system_protocol.h"

/* SPI interrupt enable type */
typedef enum
{
    SPI_ISR_ENABLE,      // Enable
    SPI_ISR_DISABLE,     // Disable
} spi_isr_set_t;
    


/* Prototype of function */
/*=====================================================
 * @brief
 *     SPI Slave初期化関数
 * @param
 *     spi_isr:spi割り込みの有効無効を設定
 * @return
 *     なし
 * @note
 *     pin43:SDO(RC5)
 *     pin42:SDI(RC4/SDA)
 *     pin37:SCK(RC3/SCL)
 *     pin24:SS (RA5/AN4/C2OUT)
 *===================================================*/
void spi_slave_start(spi_isr_set_t spi_isr);


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
uint8_t spi_slave_receive(destination_t destination);


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
void spi_slave_send(destination_t destination, uint8_t data);


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
void spi_slave_end(void);



#endif	/* _SPI_SLAVE_H */

