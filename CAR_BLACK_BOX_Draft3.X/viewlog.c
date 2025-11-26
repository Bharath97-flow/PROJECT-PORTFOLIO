/*
 * File:   viewlog.c
 * Author: ASUS
 *
 * Created on 11 October, 2025, 4:38 PM
 */


#include <xc.h>
#include"link.h"

void view_log()
{
    eeprom_busy = 1;
    int scroll_index=0;
    clear_screen();
    if(log_index<0)
    {
        clcd_print("EMPTY",0x80+5);
        clcd_print("LOG",0xC0+6);
        __delay_ms(3000);
        clear_screen();
        return;
    }
    else
    {
        clcd_print("#    TIME   E SP",0x80);
        int compare_scroll=scroll_index;
        
        while(1)
        {
            log temp;
            eeprom_read_log(scroll_index, &temp);
            if(compare_scroll!=scroll_index)
            clcd_print("                ", 0xC0);
            if(scroll_index+1==10)
            {
                clcd_print("10",0xC0);
            }
            else
            {
                clcd_print(" ",0xC0+1);
                clcd_write(0xC0,INST_MODE);
                clcd_write((char)scroll_index+1+'0',DATA_MODE);
            }
            __delay_ms(20);
            clcd_print(temp.time,0xC0+3);
            __delay_ms(20);
            clcd_print(temp.event,0xC0+12);
            __delay_ms(20);
            clcd_print(temp.speed,0xC0+14);
            __delay_ms(20);
            if(!PORTB4)
            {
                __delay_ms(20);
                int longcount=0;
                while(!PORTB4)
                {
                    __delay_ms(1);
                    longcount++;
                    if(longcount>=2000)
                    {
                        eeprom_busy = 0;
                        clear_screen();
                        return;
                    }
                }
                if(longcount<2000&&scroll_index+1<=log_index)
                {
                    scroll_index++;
                }
            }
            else if(!PORTB3)
            {
                __delay_ms(20);
                while(!PORTB3);
                if(scroll_index-1>=0)
                {
                    scroll_index--;
                }

            }
            compare_scroll=scroll_index;
        }
    }
    
}
    