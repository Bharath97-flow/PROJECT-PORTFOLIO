/*
 * File:   main.c
 * Author: ASUS
 *
 * Created on 27 September, 2025, 6:35 PM
 */


#include <xc.h>
#include"link.h"
#include <stdio.h>

void initialize_default_password(void)
{
    // Read the first byte to see if EEPROM is blank (0xFF means erased)
    unsigned char first_byte = eeprom_read(0x00);

    if (first_byte == 0xFF) // EEPROM not initialized yet
    {
        const char *default_pass = "0101";
        for (int i = 0; i < 4; i++)
        {
            eeprom_write(i, default_pass[i]);
            __delay_ms(10); // small delay for EEPROM write cycle
        }
    }
}

void main(void) {
        lcd_config();
        initialize_default_password();
       for (int i = 0; i < 4; i++) 
        {
            password[i] = eeprom_read(i);
        }
        password[4] = '\0';
start:
    adc_init();
    unsigned char clock_reg[3]; 
    rtc_init_config(); 
    clear_screen();
    dash_display(); 
    clcd_write(0xC0+10,INST_MODE);
    clcd_write(gear_array[gear_index],DATA_MODE);
    volatile int collision_index=0;
    if(on_flag)
    {
        clcd_print("ON",0xC0+10);
        on_flag=0;
        
    }
        while(!PORTB4);
    while(1)
    {
        speed_display();
        get_time(clock_reg);       
        display_time(clock_reg);
        gear_shift[0]=gear_index+'0';
        gear_shift[1]='\0';
        if(event_log!=gear_index)
        {
            if(!lock)
            {
                event_log=gear_index;
                lock=1;
            }
            else
            {
                log_index++;
                if(log_index>=LIST)
                {
                    handle_rotation();
                }
                event_log=gear_index;
                log new_log;
                strcpy(new_log.speed, speed_text);
                strcpy(new_log.time, rtc_time);
                if(collision_index)
                  new_log.event[0] = gear_array[6];
                else
                  new_log.event[0] = gear_array[gear_index];
                new_log.event[1] = '\0';
                eeprom_write_log(log_index, &new_log);
            }
        }
            
            if(PORTB2==0)
            {
                while(!PORTB2);
               __delay_ms(20); 
               clcd_write(0xC0+11,INST_MODE);
               clcd_write(' ',DATA_MODE);
               if(--gear_index==-1)
               {
                   gear_index=0;
               }
               gear_shift[0]=(char)gear_index;
               gear_shift[1]='\0';
               clcd_write(0xC0+10,INST_MODE);
               clcd_write(gear_array[gear_index],DATA_MODE);
               collision_index=0;
            }
            else if(PORTB1==0)
            {
                while(!PORTB1);
                __delay_ms(20);
                clcd_write(0xC0+11,INST_MODE);
                clcd_write(' ',DATA_MODE);          
                if(++gear_index==6)
                {
                    gear_index=5;
                }
                gear_shift[0]=(char)gear_index;
                gear_shift[1]='\0';
                clcd_write(0xC0+10,INST_MODE);
                clcd_write(gear_array[gear_index],DATA_MODE);
                collision_index=0;
            }
            else if(PORTB0==0)
            {
                while(!PORTB0);
                __delay_ms(20);
                clcd_write(0xC0+11,INST_MODE);
                clcd_write(' ',DATA_MODE);  
                gear_shift[0]=6+'0';
                gear_shift[1]='\0';
                clcd_write(0xC0+10,INST_MODE);
                clcd_write(gear_array[6],DATA_MODE);   
                collision_index=1;
                event_log=6;
            }
            else if(PORTB3==0||PORTB4==0)
            {
                while(!PORTB3||!PORTB4);
                __delay_ms(20);
                char temp_password[5];
                clear_screen();
                clcd_print("PASSWORD",0x80+4);
                int block=0;
                while(1)
                {
                    clear_screen();
                    clcd_print("PASSWORD",0x80+4);
                    int result=password_check(temp_password,0);
                    if(result==1)
                    {
                        clear_screen();
                        menu();
                        goto start;
                    }
                    else if(!result)
                    {
                        clear_screen();
                        clcd_print("INVALID PASSWORD",0x80);
                        block++;
                        if(block<3)
                        {
                            
                            clcd_write(0xC0,INST_MODE);
                            clcd_write(3-block+'0',DATA_MODE);
                            if(3-block==1)
                            {
                                clcd_print("ATEMPT LEFT",0xC0+2);
                            }
                            else
                            {
                                clcd_print("ATTEMPTS LEFT",0xC0+2);
                            }
                            __delay_ms(3000);
                        }
                    }
                    else if(result==2)
                    {
                        clear_screen();
                        clcd_print("SCREEN TIMEOUT",0x80);
                        __delay_ms(3000);
                       // clear_screen();
                        goto start;
                    }
                    else if(result==3)
                    {
                        goto start;
                    }
                    if(block==3)
                    {
                        block=0;
                        clear_screen();
                        clcd_print("YOU ARE BLOCKED",0x80);
                        block_seconds = 120;      // countdown from 120 seconds
                        block_active = 1;
                        timer1_init();            // start 1-second timer

                        while(block_active)       // stay here until block period ends
                        {
                            clcd_write(0xC0, INST_MODE);
                            clcd_print("Wait: ", 0xC0);

                            // Convert seconds to string and display
                            char buf[4];
                            sprintf(buf, "%3d", block_seconds);
                            clcd_write(0xC0 + 6, INST_MODE);
                            for(char *p = buf; *p; p++)
                            {
                                clcd_write(*p, DATA_MODE);
                            }
                            clcd_print("secs",0xC0+10);
                            __delay_ms(200);      // small refresh delay for display stability
                        }
                        goto start;

                    }
                }
            }
    }
    return;
}
