/*
 * File:   set_time.c
 * Author: ASUS
 *
 * Created on 11 October, 2025, 5:49 PM
 */


#include <xc.h>
#include"link.h"

void set_time(void) {
    clear_screen();
    clcd_print("SET TIME",0x80+4);
    char dummy_time[9];
    for(int i=0;i<9;i++)
    {
        dummy_time[i]=rtc_time[i];
    }
   
    int shift=7,longcount=0;
    while(1)
    {
        clcd_print(dummy_time,0xC0+4);
        if(!PORTB4)
        {
            longcount=0;
            __delay_ms(20);
            while(!PORTB4)
            {
                __delay_us(2);
                longcount++;
                if(longcount>=20000)
                {
                    clock_reg[0]=(((dummy_time[0]-'0')<<4)|dummy_time[1]-'0');
                    write_ds1307(HOUR_ADDR,clock_reg[0]);
                    clock_reg[1]=(((dummy_time[3]-'0')<<4)|dummy_time[4]-'0');
                    write_ds1307(MIN_ADDR,clock_reg[1]);
                    clock_reg[2]=(((dummy_time[6]-'0')<<4)|dummy_time[7]-'0');
                    write_ds1307(SEC_ADDR,clock_reg[2]);
                    clear_screen();
                    clcd_print("TIME UPDATED",0x80+2);
                    clcd_print("SUCCESSFULLY",0xC0+2);
                    __delay_ms(3000);
                    clear_screen();
                    return;
                }
            }
            __delay_ms(20);
            if(dummy_time[0]-'0'==2&&dummy_time[1]-'0'+1==4)
            {
                dummy_time[0]=dummy_time[1]='0';
            }
            else if(dummy_time[shift]-'0'>=9)
            {
                dummy_time[shift-1]++;
                dummy_time[shift]='0';
                if(dummy_time[shift]-'0'==0&&dummy_time[shift-1]-'0'==6)
                {
                    dummy_time[shift-1]=dummy_time[shift]='0';
                }
            }
            else
            {
                dummy_time[shift]++;
            }
            
        }
        if(!PORTB5)
        {
            longcount=0;
            __delay_ms(20);
            while(!PORTB5)
            {
                longcount++;
                if(longcount==20000)
                {
                    clear_screen();
                    clcd_print("SET TIME FAILURE",0x80);
                    __delay_ms(3000);
                    clear_screen();
                    return ;
                }
            }
            __delay_ms(20);
            shift-=3; 
            if(shift<=0)
            {
                shift=1;
            }
        }
    }
            
}
