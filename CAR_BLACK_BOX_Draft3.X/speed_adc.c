/*
 * File:   speed_adc.c
 * Author: ASUS
 *
 * Created on 2 October, 2025, 5:51 AM
 */


#include <xc.h>
#include"link.h"
void adc_init()
{
    ADCON0 = 0x41;  
    ADCON1 = 0x0E;  
    ADFM = 1;       
    CHS2=0;
    CHS1=0;
    CHS0=0;
    PCFG3=1;
    PCFG2=1;
    PCFG1=1;
    PCFG0=0;
    TRISA0 = 1;  
    ADON=1;
}

unsigned int read_adc()
{
    GO=1;
    while(GO);
    return ((ADRESH) << 8 | ADRESL);
}

void speed_string(unsigned char *arr, unsigned int speed)
{
    arr[0] = ' ';
    arr[1] = ' ';
    arr[2] = ' ';
    arr[3] = '\0'; 
    // Ensure the speed is capped at 99.
    if (speed > 99) {
        speed = 99;
    }
    
    // Manually format the 3-character string for display.
    arr[0] = (speed / 10) % 10 + '0';  // Tens digit
    arr[1] = speed % 10 + '0';         // Ones digit
    arr[2] = '\0';                     // Null terminator

    // If the speed is a single digit, add a leading space for alignment.
    if (speed < 10) {
        arr[0] = ' ';
    }
}

