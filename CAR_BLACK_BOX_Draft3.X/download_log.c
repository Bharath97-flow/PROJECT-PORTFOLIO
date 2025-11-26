/*
 * File:   download_log.c
 * Author: ASUS
 *
 * Created on 11 October, 2025, 5:48 PM
 */


#include <xc.h>
#include"link.h"

void my_eeprom_write(unsigned char addr, unsigned char data) {
    while (WR);     // Wait for previous write
    EEADR = addr;   // EEPROM Address
    EEDATA = data;  // Data to write
    EEPGD = 0;      // Point to EEPROM memory
    WREN = 1;       // Enable write
    GIE = 0;        // Disable interrupts
    EECON2 = 0x55;  // Required sequence
    EECON2 = 0xAA;
    WR = 1;         // Start write
    GIE = 1;        // Enable interrupts
    WREN = 0;       // Disable write
}

unsigned char my_eeprom_read(unsigned char addr) {
    EEADR = addr;
    EEPGD = 0;
    RD = 1;
    return EEDATA;
}

unsigned char load_log_index(void)
{
    unsigned char index = eeprom_read(0x00);
    return index;
}

void update_log_index(unsigned char index)
{
    eeprom_write(LOG_INDEX_ADDR, index);
}

void eeprom_write_log(unsigned char index, const log *entry)
{
    unsigned char addr = index * sizeof(log);
    unsigned char i;

    // Write time[9]
    for (i = 0; i < 9; i++)
    {
        my_eeprom_write(addr++, entry->time[i]);
        __delay_ms(5); // small delay for EEPROM write cycle
    }

    // Write event[2]
    for (i = 0; i < 2; i++)
    {
        my_eeprom_write(addr++, entry->event[i]);
        __delay_ms(5);
    }

    // Write speed[3]
    for (i = 0; i < 3; i++)
    {
        my_eeprom_write(addr++, entry->speed[i]);
        __delay_ms(5);
    }
}


void eeprom_read_log(unsigned char index, log *entry)
{
    unsigned char addr = index * sizeof(log);
    unsigned char i;

    // Read time[9]
    for (i = 0; i < 9; i++)
        entry->time[i] = my_eeprom_read(addr++);

    // Read event[2]
    for (i = 0; i < 2; i++)
        entry->event[i] = my_eeprom_read(addr++);

    // Read speed[3]
    for (i = 0; i < 3; i++)
        entry->speed[i] = my_eeprom_read(addr++);
}

void uart_init(void)
{
    SYNC = 0;    // Asynchronous mode
    BRGH = 1;    // High-speed mode
    SPBRG = 25;  // 9600 baud @ 4MHz

    TXEN = 1;    // Enable transmission
    CREN = 1;    // Enable reception
    SPEN = 1;    // Enable serial port (configures TX/RX pins)
    
    TXIF = 1;    // Ensure TXIF flag starts high
}


void uart_tx(char data)
{
    while (!TXIF); // Wait until TX buffer is empty
    TXREG = data;
}

void uart_tx_string(const char *str)
{
    while (*str)
    {
        uart_tx(*str++);
    }
}


void download_log(void)
{
    log temp;
    clear_screen();
    uart_init();
    clcd_print("TRANSMITING DATA",0x80);
    clcd_print("TO THE PC",0xC0+4);
    uart_tx_string("\r\n--- VEHICLE LOG DATA ---\r\n");
    uart_tx_string("No.   Time       Event Speed\r\n");
    uart_tx_string("-----------------------------\r\n");

    for (unsigned char i = 0; i <= log_index; i++)
    {
        eeprom_read_log(i, &temp);

        char buffer[32];
        sprintf(buffer, "%2d.  %s  %s     %s\r\n", i+1, temp.time, temp.event, temp.speed);
        uart_tx_string(buffer);
    }

    uart_tx_string("-----------------------------\r\n");
    uart_tx_string("END OF LOG\r\n");
    clear_screen();
    clcd_print("LOG DATA",0x80+3);
    clcd_print("TRANSFERED TO PC",0xC0);
    __delay_ms(3000);
    clear_screen();
    return ;
}

