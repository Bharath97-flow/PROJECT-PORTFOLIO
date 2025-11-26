/*
 * File:   password.c
 * Author: ASUS
 *
 * Created on 7 October, 2025, 5:22 PM
 */


#include <xc.h>
#include"link.h"
#include<string.h>

int password_check(char *temp_password,int new_pass)
{
    int longcount=0;
    timeout_flag = 0;
    tmr0_overflow_count = 0;
    T0IF = 0;
    timer0_init();
    int i=0,count=0;
    while(1)
    {
      if(timeout_flag)
      {
        timeout_flag=0;
        T0IE = 0;  
        return 2;
      }
        if(i==4)
        {
            break;
        }
            if(!PORTB3)
            {
                while(!PORTB3);
                __delay_ms(800);
                temp_password[i]='0';
                i++;
                clcd_write(0xC0+5+i,INST_MODE);
                clcd_write('*',DATA_MODE);
            }
            else if(!PORTB4)
            {
                while(!PORTB4);
                __delay_ms(800);
                temp_password[i]='1';
                i++;
                clcd_write(0xC0+5+i,INST_MODE);
                clcd_write('*',DATA_MODE);
                longcount=0;
                while(!PORTB4)
                {
                    longcount++;
                    if(longcount>=2000)
                    {
                        return 3;
                    }
                }
            }

    }
    temp_password[4]='\0';
    __delay_ms(500);
    if(new_pass==1)
    {
        char temp_check[5];
        clear_screen();
        clcd_print("RE ENTER NEW KEY",0x80);
        i=0;
        while(1)
        {
          if(timeout_flag)
          {
            timeout_flag=0;
            T0IE = 0;  
            return 2;
          }
            if(i==4)
            {
                break;
            }
                if(!PORTB3)
                {
                    while(!PORTB3);
                    __delay_ms(800);
                    temp_check[i]='0';
                    i++;
                    clcd_write(0xC0+5+i,INST_MODE);
                    clcd_write('*',DATA_MODE);
                }
                else if(!PORTB4)
                {
                    while(!PORTB4);
                    __delay_ms(800);
                    temp_check[i]='1';
                    i++;
                    clcd_write(0xC0+5+i,INST_MODE);
                    clcd_write('*',DATA_MODE);
                    longcount=0;
                    while(!PORTB4)
                    {
                        longcount++;
                        if(longcount>=2000)
                        {
                            return 3;
                        }
                    }
                }
                else if(!PORTB5)
                {
                    longcount=0;
                    while(!PORTB5)
                    {
                        __delay_ms(1);
                        longcount++;
                        if(longcount>=2000)
                        {
                            return 5;
                        }
                    }
                }
        }
        temp_check[4]='\0';
        __delay_ms(500);
        int check=strcmp(temp_check,temp_password);
        if(!check)
        {
            strcpy(password,temp_password);
            
            clear_screen();
            
            return 4;
        }
        else if(check==1)
        {
            return 0;
        }
    }
        
    if(!strcmp(temp_password,password))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
