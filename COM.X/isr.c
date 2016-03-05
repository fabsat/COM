#include <xc.h>
#include "pic_clock.h"
#include "isr.h"
#include "system_protocol.h"



void interrupt isr(void)
{
    /* SPI割込み */
    if(PIR1bits.SSPIF == 1)
    {
        
        
        SSPBUF = 0xff;     // これがないと次に割込みが入った時にSSPBUFに渡すものがないため割込みが入らない
        PIR1bits.SSPIF = 0;        
    }
}
