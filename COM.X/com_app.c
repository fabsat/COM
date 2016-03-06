/******************************************************************************************
 * com_app.c
 * ver1.00
 * Yu Yoshida
 *=========================================================================================
 * 通信系のアプリケーションソースファイル
 *
 *=========================================================================================
 * ・ver1.00 ||  || 2016/03/06 || Yu Yoshida
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





/*=====================================================
 * @brief
 *     C&DH系にGSからのコマンドを送信
 * @param
 *     void:
 * @return
 *     void:
 * @note
 *     none
 *===================================================*/
void send_command(void)
{
    
    
    /* C&DH系にコマンド送信要求 */
    
}




void receive_from_cdh(destination_t destination)
{
    
}
