/*
 * File:   lcd.c
 * Author: ASUS
 *
 * Created on 2 October, 2025, 6:31 AM
 */


#include <xc.h>
#include "link.h"

void clear_screen()
{
    int i=0;
    while(i<16)
    {
        clcd_write(0x80+i,INST_MODE);
        clcd_write(' ',DATA_MODE);
        i++;
    }
    i=0;
    while(i<16)
    {
        clcd_write(0xC0+i,INST_MODE);
        clcd_write(' ',DATA_MODE);
        i++;
    }
}

void clcd_write(unsigned char byte, unsigned char mode)
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
    __delay_ms(40);
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