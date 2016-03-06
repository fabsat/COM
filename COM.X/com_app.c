/******************************************************************************************
 * com_app.c
 * ver1.00
 * Yu Yoshida
 *=========================================================================================
 * 通信系のアプリケーションソースファイル
 *
 *=========================================================================================
 * ・ver1.00 || 2016/03/06 || Yu Yoshida
 *   初版
 *=========================================================================================
 * PIC16F877A
 * MPLAB X IDE
 * XC8
 *=========================================================================================
 * Created by fabsat Project(Tokai university Satellite Project[TSP])
 *****************************************************************************************/
#include <xc.h>
#include "pic_clock.h"
#include "com_app.h"
#include "system_prototcol.h"
#include "spi_slave.h"


/*=====================================================
 * @brief
 *     C&DHからのCallの応答シーケンス
 * @param
 *     data_from_cdh:SPI割込みで来る最初のデータ
 * @return
 *     void:
 * @note
 *     none
 *===================================================*/
void cdh_call_handler(uint8_t data_from_cdh)
{
    destination_t destination;    
    uint8_t buf;
    const uint8_t bit_mask = 0b00001111;

    /* 下位4bitを取り出しdestinationを調べる */
    buf = data_from_cdh & bit_mask;
    switch(buf)
    {
	case 0x01:
            destination = OBC1;
            break;
        case 0x02:
            destination = OBC2;
            break;
    }

    /* 上位4bitを調べ送受信を選択する */
    buf = data_from_cdh >> 4;
    switch(buf)
    {
	case 0:
            spi_slave_send(destination, command_to_obc); // コマンドを送信する
            break;
        case 1:
            packet_receive_slave(destination);           // OBCからパケットを受信する
            break;
    }
}
