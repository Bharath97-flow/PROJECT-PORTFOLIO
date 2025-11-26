/*
 * File:   menu.c
 * Author: ASUS
 *
 * Created on 11 October, 2025, 10:29 AM
 */


#include <xc.h>
#include"link.h"

void menu()
{
    const char *display[]={"View log","Clear log","Download log","Change passwrd","Set time"};
    void (*menu_functions[])(void) = {
    view_log,
    clear_log,
    download_log,
    change_password,
    set_time
    };

    int i=0;
    while(1)
    {
        int longcount = 0;
        if(i<4)
        {
            clcd_write(0x80,INST_MODE);
            clcd_write('*',DATA_MODE);
            clcd_write(0xC0,INST_MODE);
            clcd_write(' ',DATA_MODE);
        }
        else
        {
            clcd_write(0x80,INST_MODE);
            clcd_write(' ',DATA_MODE);
            clcd_write(0xC0,INST_MODE);
            clcd_write('*',DATA_MODE);
        }
        if(i<4)
        {
            clcd_print(display[i],0x80+2);
            clcd_print(display[i+1],0xC0+2);
        }
        else
        {
            clcd_print(display[i-1],0x80+2);
            clcd_print(display[i],0xC0+2);
        }
        if(!PORTB5)
        {
            __delay_ms(20);
            clear_screen();
            return ;
        }
            
        if (!PORTB4) // scroll down
        {
            longcount=0;
            while(!PORTB4)
            {
                __delay_ms(1);
                longcount++;
                if (longcount >= 2000) // long press detected (~2s)
                {
                    return; // exit to dashboard
                }
            }
            if (longcount < 2000 && i < 4)
            {
                clear_screen();
                i++;

            }
            
        }
        if (!PORTB3) // scroll up
        {
           __delay_ms(20);
           longcount=0;
            while(!PORTB3) 
            {
                __delay_ms(1);
                longcount++;
                if(longcount>=2000)
                {
                   
                    menu_functions[i]();
                    clear_screen();
                    __delay_ms(200);  // small debounce delay
                }
            }
            if (i > 0&&longcount<2000)
            {
                clear_screen();
                i--;

            }
        }
    }
}
