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
 *     SPIデータ送受信関数(1Byte)
 * @param
 *     sent_data:送信データ
 * @return
 *     received_data:受信データwait for finish
 * @note
 *     none
 *===================================================*/
uint8_t spi_send_receive(uint8_t sent_data);



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
void spi_interrupt(void);



#endif	/* _SPI_SLAVE_H */

