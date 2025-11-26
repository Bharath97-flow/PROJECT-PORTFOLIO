/*
 * File:   timeout_isr.c
 * Author: ASUS
 *
 * Created on 8 October, 2025, 5:40 PM
 */


#include <xc.h>
#include "link.h"

void __interrupt() timer_isr(void)
{
    if (T0IF)
    {
        T0IF = 0;
        TMR0 = 0;
        tmr0_overflow_count++;
        if (tmr0_overflow_count >= 298)
        {
            timeout_flag = 1;
            T0IE = 0;
        }
    }

     if (TMR1IF)
    {
        TMR1IF = 0;
        TMR1 = 3036; // reload
        if (block_active)
        {
            if (block_seconds > 0)
                block_seconds--;
            else
            {
                block_active = 0;
                T1CONbits.TMR1ON = 0;
                TMR1IE = 0;
            }
        }
    }
}

void timer0_init(void)
{
    OPTION_REG = 0b00000111;  // Prescaler 1:256 for maximum slow-down
    TMR0 = 0;                 // Start from 0
    T0IF = 0;
    T0IE = 1;
    GIE = 1;
}


void timer1_init(void)
{
    T1CON = 0x31;    // Fosc/4, prescaler 1:8, enable Timer1
    TMR1 = 3036;     // preload for 1 second @ 2 MHz
    TMR1IE = 1;
    PEIE = 1;
    GIE = 1;
}


