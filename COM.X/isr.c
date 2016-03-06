#include <xc.h>
#include "pic_clock.h"
#include "isr.h"
#include "system_protocol.h"
#include "com_app.h"



/*=====================================================
 * 割込みルーチン関数
 *===================================================*/
void interrupt isr(void)
{
    uint8_t data;
    
    /* SPI割込み */
    if(PIR1bits.SSPIF == 1)
    {
        data = SSPBUF;          // SPI受信データを受け取る(OBC1 or OBC2を判断するデータを受け取る)
        cdh_call_handler(data); // 
        SSPBUF         = 0x00;  // これがないと次に割込みが入った時にSSPBUFに渡すものがないため割込みが入らない
        PIR1bits.SSPIF = 0;     // SPI割込みフラグクリア
    }

    /* I/O割込み (地上局からのコマンド) */
    if(0)
    {
        // 地上局にACKを返す
        // パケットによってOBC1かOBC2か判断する
        // パケットをOBCに送る(NOTIFICATIONピンをHighにして通知する)
        // CWデータを見てOBC1 or OBC2が故障していたらそれに対処してパケットを送信する
        // ミッションコマンドなど根本的に不可能な場合はそれを地上局に通知する
    }
}
