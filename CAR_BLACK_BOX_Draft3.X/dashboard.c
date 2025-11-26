/*
 * File:   dashboard.c
 * Author: ASUS
 *
 * Created on 2 October, 2025, 5:52 AM
 */


#include <xc.h>
#include"link.h"

unsigned char evt[]="E";
unsigned char time_text[]="TIME";
unsigned char speed[]="SP";

void dash_display()
{
    int i=0;
    while(time_text[i])
    {
        clcd_write(0x80+2+i,INST_MODE);
        clcd_write(time_text[i],DATA_MODE);
        i++;
    }
    i=0;
    while(evt[i])
    {
        clcd_write(0x80+10+i,INST_MODE);
        clcd_write(evt[i],DATA_MODE);
        i++;
    }
    i=0;
    while(speed[i])
    {   
        clcd_write(0x80+14+i,INST_MODE);
        clcd_write(speed[i],DATA_MODE);
        i++;
    }
    
}

int speed_display()
{  
    int speed_read_complete = ((unsigned long)read_adc() * 99) / 1023;
    speed_string((unsigned char*)speed_text, speed_read_complete);
    int i=0;
    while(speed_text[i])
    {
        clcd_write(0xC0 + 14 + i, INST_MODE);
        clcd_write(speed_text[i++],DATA_MODE);
    }
    
    if(speed_read_complete!=speed_log)
    {
        if(!lock)
        {
            speed_log=speed_read_complete;
            lock=1;
        }
        else
        {
            log_index++;
            update_log_index(log_index);
            if(log_index>=LIST)
            {
                handle_rotation();
            }
            speed_log=speed_read_complete;
            log new_log;
            strcpy(new_log.speed, speed_text);
            strcpy(new_log.time, rtc_time);
            new_log.event[0] = gear_array[gear_index];
            new_log.event[1] = '\0';
            eeprom_write_log(log_index, &new_log);
        }
        
    }
            

    return 0; 
}
