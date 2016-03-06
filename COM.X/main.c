#include <xc.h>
#include "pic_clock.h"
#include "system_protocol.h"
#include "isr.h"


// CONFIG
#pragma config FOSC  = HS       // Oscillator Selection bits (HS oscillator)
#pragma config WDTE  = OFF      // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP   = OFF      // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD   = OFF      // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT   = OFF      // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP    = OFF      // Flash Program Memory Code Protection bit (Code protection off)


/******************************************************************************************
 *                                  static関数のプロトタイプ宣言                            *
 *****************************************************************************************/
static void pic_port_init(void);


/******************************************************************************************
 *                                  グローバル変数宣言                                      *
 *****************************************************************************************/
cw_t cw_data = CW_DATA_INIT;


/******************************************************
 * main function
 *****************************************************/
int main(void)
{
    pic_port_init();
    
    
    
    
    
    
    while(1)
    {
        if(cdh_call_status)
        {
            
        }

        if(command_status)
        {
            
        }
    }

    return 0;
}


/*-----------------------------------------------------
 * PORTを初期化
 *---------------------------------------------------*/
static void pic_port_init(void)
{    
    TRISA = 0x00;
    TRISB = 0x00;
    TRISC = 0x00;
    TRISD = 0x00;
    TRISE = 0x00;
    
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
}
