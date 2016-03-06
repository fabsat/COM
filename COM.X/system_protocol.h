/******************************************************************************************
 * system_protocol.h
 * ver1.00
 * Hirofumi Hamada
 *=========================================================================================
 * 通信系用System&Protocolレイヤーライブラリのヘッダファイル
 * MCU間のデータ送受信プロトコル
 *
 *=========================================================================================
 * ・ver1.00 || 2016/03/06 || Hirofumi Hamada
 *   初版
 *=========================================================================================
 * PIC16F877A
 * MPLAB X IDE
 * XC8
 *=========================================================================================
 * Created by fabsat Project(Tokai university Satellite Project[TSP])
 *****************************************************************************************/

#ifndef SYSTEM_PROTOCOL_H
#define	SYSTEM_PROTOCOL_H

#include <xc.h>
#include "pic_clock.h"



/******************************************************************************************
 *                                  各サブシステム関連の定義                                *
 *****************************************************************************************/
/* C&DHへの通知ピンを定義 */
#define NOTIFICATION_TO_OBC1 PORTAbits.RA0
#define NOTIFICATION_TO_OBC2 PORTAbits.RA1
#define NOTIF_TO_OBC1_TRIS   TRISAbits.TRISA0
#define NOTIF_TO_OBC2_TRIS   TRISAbits.TRISA1


/* データ転送の相手先 */
typedef enum
{
    OBC1 = 0x01,    
    OBC2 = 0x02,
} destination_t;

/* 使用しているマイコンがOBC1=0x01, OBC2=0x02, COM=0x03, POW=0x04 */
#define USE_MCU 0x03    

/* C&DHからの送信データの有無 */
extern destination_t cdh_call_status;


/********************************************************************************
 *                          パケット関連の各種定義                                *
 *******************************************************************************/
/* パケットのdata_type一覧 */
typedef enum
{
    CW_TYPE = 0x01,
    DATA_TYPE,
    COMMAND_TYPE,
} data_type_t;


/* コマンド一覧 */
#define LED_BLINK     (0x01)
#define GET_I2C_TEMP  (0x02)


/* データ終了コマンド一覧 */
typedef enum
{
    DATA_END = 0b00010000,
    DATA_CONTINUE,    
} data_end_command_t;


/* コマンド受信時格納用 */
extern uint8_t received_command;

/* 受信したコマンドがあるか あればステータスが1となる */
extern uint8_t command_status;


/********************************************************************************
 *                             CWに関する各種定義                                *
 *******************************************************************************/
/* CWで発信するデータリストのステータス保存用構造体 */
typedef struct
{
    uint8_t power1[2];
    uint8_t power2[2];
    uint8_t power3[2];
    uint8_t power4[2];
    uint8_t power5[2];
    uint8_t temp[2];
    uint8_t obc1:1;
    uint8_t obc2:1;
    uint8_t powmcu:1;
} cw_t;


/* cw_struct_t型変数の初期化用 */
#define CW_DATA_INIT  \
{                     \
	{0x00, 0x00}, \
	{0x00, 0x00}, \
	{0x00, 0x00}, \
	{0x00, 0x00}, \
	{0x00, 0x00}, \
	{0x00, 0x00}, \
	0,            \
        0,            \
	0             \
}


/********************************************************************************
 *                       ライブラリ関数のプロトタイプ宣言                          *
 *******************************************************************************/

/*=====================================================
 * @brief
 *     system_protocolの初期設定
 * @param
 *     void:
 * @return
 *     void:
 *  @note
 *     各MCUからの状態通知ピンの設定
 *===================================================*/
void sys_init(void);


/*=====================================================
 * @brief
 *     Payloadに送信データを格納していく
 * @param
 *     p_data      :uint8_t, uint16_t, doubleの
 *                  配列へのポインタ
 *
 *     data_len    :p_dataは何データあるか
 *                  ただし以下のデータ数制限がある
 *                  uint8_tは15データ
 *                  uint16_tは10データ
 *                  doubleは6データ
 *
 *     byte_of_data:格納したいデータ型のバイト数
 *                  uint8_tなら1
 *                  uintt16_tなら2
 *                  doubleなら4
 * @return
 *     Flee space of payload:ペイロードの残り空き容量
 *     0xff:渡されたデータ全ては格納できない
 *          またはbyte_of_dataが適切な値でない
 * @note
 *     最大ペイロードサイズまで格納できる
 *===================================================*/
uint8_t sent_data_set(void *p_data, uint8_t data_len, uint8_t byte_of_data);


/*=====================================================
 * @brief
 *     指定したサブシステムにデータを送信する(Master用)
 * @param
 *     destination     :送信の相手先
 *     data_type       :payloadに格納したデータのタイプ
 *     data_end_command:データはまだ継続するかどうか
 * @return
 *     void:
 * @note
 *     この関数実行後にsetしたデータ内容は初期化される
 *===================================================*/
void send_data_master(destination_t destination, data_type_t data_type, data_end_command_t data_end_command);



#endif	/* _DATA_SEND_RECEIVE_PROTOCOL_H */

