/*
 * File:   change_password.c
 * Author: ASUS
 *
 * Created on 11 October, 2025, 5:49 PM
 */


#include <xc.h>
#include"link.h"

void change_password(void)
{
    clcd_print("CURRENT PASSWORD",0x80);
    while(!PORTB3||!PORTB4);
    __delay_ms(20);
    clear_screen();
    clcd_print("CURRENT PASSWORD",0x80);
    int result;
    char temp_password[5];
    result=password_check(temp_password,0);
    if(result==1)
    {
        clcd_print("ENTER   NEW  KEY",0x80);
        clcd_print("                 ",0xC0);
        while(!PORTB3||!PORTB4);
        __delay_ms(20);
        clcd_print("ENTER   NEW  KEY",0x80);
        int res;
        res=password_check(temp_password,1);
        if(res==4)
        {
            for (int i = 0; i < 4; i++)
            {
                eeprom_write(i, password[i]);   
            }
            clcd_print("PASSWORD",0x80+4);
            clcd_print("UPDATED",0xC0+4);
            __delay_ms(3000);
            clear_screen();
            return ;
        }
        else if(res==2)
        {
            clear_screen();
            clcd_print("SCREEN TIMEOUT",0x80);
            __delay_ms(3000);
            clear_screen();
            return ;
        }
        else if(res==3)
        {
            clear_screen();
            return ;
        }
        else if(res==0)
        {
            clear_screen();
            clcd_print("PASSWORD",0x80);
            clcd_print("MIS MATCH",0xC0);
            __delay_ms(3000);
            clear_screen();

            return ;
        }   
    }
    else if(result==2)
    {
        clear_screen();
        clcd_print("SCREEN TIMEOUT",0x80);
        __delay_ms(3000);
        clear_screen();
        return ;
    }
    else if(!result)
    {
        clcd_print("INVALID PASSWORD",0x80);  
        __delay_ms(3000);
        clear_screen();
        return ;
    }
    else if(result==3)
    {
        clear_screen();
        return ;
    }
    else if(result==5)
    {
        clear_screen();
        return;
    }
}
