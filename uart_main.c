/*
 * File:   uart_main.c
 * Author: ASUS
 *
 * Created on 21 August, 2025, 12:55 PM
 */


#include <xc.h>
#include <string.h>
#define INST_MODE 0
#define DATA_MODE 1
#define PORTE2   RE2
#define PORTE1   RE1
#define TRISC6   RC6
#define TRISC7   RC7
#define _XTAL_FREQ 2000000
#pragma config WDTE = OFF

static void clcd_write(unsigned char byte, unsigned char mode)
{
    PORTE2 = mode;
    PORTD = byte;
    
    PORTE1 = 1;
    __delay_us(100);
    PORTE1 = 0;
    
    __delay_us(4100);
}

void lcd_config()
{
    TRISD=0X00;
    TRISE=0x00;
    PORTE=0x02;
    __delay_ms(30);
     clcd_write(0X33,INST_MODE);
    __delay_us(4100);
    clcd_write(0x33, INST_MODE);
    __delay_us(100);
    clcd_write(0x33, INST_MODE);
    __delay_us(1); 
    
    clcd_write(0x38, INST_MODE);
    __delay_us(100);
    clcd_write(0x01, INST_MODE);
    __delay_us(500);
    clcd_write(0x0C, INST_MODE);
    __delay_us(100);
}
void uart_config()
{
    TRISC6 = 0;     // RC6 as o/p 
    TRISC7 = 1;    // RC7 as i/p
    TX9=0;
    TXEN=1;
    SYNC=0;
    BRGH=1;
    SPEN=1;
    RX9=0;
    CREN=1;
    SPBRG=129;
}
void main(void) {
    int t=0,h=0,ind=0;
    char address[]={0x80,0xC0};
    char str[17];
    lcd_config();
    uart_config();
    while(1)
    {
        
        char rx;
        
        while(!RCIF);
        rx=RCREG;
        if(rx == '\r'||rx=='\n') 
        {
            while(!TXIF);
            TXREG = '\r';   
            while(!TXIF);
            TXREG = '\n';  
        }
        else if(rx!='\n')
        {
            while(!TXIF);
            TXREG = rx;     // Echo normal characters
        }
        

        str[ind++] = rx;       // always push to buffer
        clcd_write(address[t]+h, 0);
        clcd_write(rx, 1);
        h++;
        if(rx == '\r' || rx == '\n' || h == 16)
        {
            TXREG=rx;
            str[ind] = '\0'; // terminate string
            if(t == 0)
            {
                // First line complete ? go to second line
                t = 1;
                h = 0;
            }
            else
            {
                // Shift line 2 -> line 1
                clcd_write(address[0], 0);
                for(int i = 0; i < 16; i++)
                {
                    clcd_write((i < ind) ? str[i] : ' ', 1);
                }

                // Clear second line
                clcd_write(address[1], 0);
                for(int i = 0; i < 16; i++) clcd_write(' ', 1);

                t = 1;
                h = 0;
            }
            ind = 0;
        }
        

        
        
    }
    
    return;
}
