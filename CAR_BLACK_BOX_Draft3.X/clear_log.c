/*
 * File:   clear_log.c
 * Author: ASUS
 *
 * Created on 11 October, 2025, 5:48 PM
 */


#include <xc.h>
#include"link.h"
void clear_log(void) {
    if(log_index==-1)
    {
        clear_screen();
        clcd_print("EMPTY",0x80+5);
        clcd_print("LOG",0xC0+6);
        __delay_ms(3000);
        return ;
    }
    clear_screen();
    clcd_print("LOG",0x80+6);
    clcd_print("CLEARED",0xC0+4);
    __delay_ms(3000);
    log_index=-1;
    update_log_index(log_index);
    return;
}
