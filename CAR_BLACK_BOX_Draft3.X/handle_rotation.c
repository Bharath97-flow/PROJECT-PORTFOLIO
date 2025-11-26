/*
 * File:   handle_rotation.c
 * Author: ASUS
 *
 * Created on 12 October, 2025, 3:18 PM
 */


#include <xc.h>
#include"link.h"

void handle_rotation(void) {
    log temp;
    for(int i=0;i<LIST-1;i++)
    {
        eeprom_read_log(i + 1, &temp);
        eeprom_write_log(i, &temp);
    }
    log_index=LIST-1;
    update_log_index(log_index);
}
