#include <xc.h>
#include"link.h"
#define LINE1 0x80
#define LINE2 0xC0
#pragma config WDTE = OFF  
#define INST_MODE 0
#define DATA_MODE 1

void clcd_print(char *str, unsigned char addr)
{
    int i=0;
    
    
    while (*str != '\0')
    {
        clcd_write(addr+i, INST_MODE);
        clcd_write(*str, DATA_MODE); 
        str++;
        i++;
    }
}

void init_ds1307(void)
{
    unsigned char dummy;
   
    dummy = read_ds1307(SEC_ADDR);
    dummy = dummy & 0x7F;
    write_ds1307(SEC_ADDR, dummy);// ch = 0
}

unsigned char read_ds1307(unsigned char addr)
{
    unsigned char data;
    
    i2c_start();
    i2c_write(SLAVE_WRITE);
    i2c_write(addr);
    i2c_rep_start();
    i2c_write(SLAVE_READ);
    data = i2c_read(0);
    i2c_stop();
    
    return data;
}

void write_ds1307(unsigned char addr, unsigned char data) // SEc_ADDR, data
{
    i2c_start();
    i2c_write(SLAVE_WRITE);
    i2c_write(addr);
    i2c_write(data);
    i2c_stop();
}

void display_time(unsigned char *clock_reg)
{
    //BCD Format 
    //clock_reg[0] = HH
    //clock_reg[1] = MM
    //clock_reg[2] = SS
   
    /* To store the time in HH:MM:SS format */
    //static char time[9];  // "HH:MM:SS"
    // HH -> 
    rtc_time[0] = ((clock_reg[0] >> 4) & 0x03) + '0';
    rtc_time[1] = (clock_reg[0] & 0x0F) + '0';
    
    rtc_time[2] = ':';
    // MM 
    rtc_time[3] = ((clock_reg[1] >> 4) & 0x07) + '0';
    rtc_time[4] = (clock_reg[1] & 0x0F) + '0';
    
    rtc_time[5] = ':';
    // SS
    rtc_time[6] = ((clock_reg[2] >> 4) & 0x07) + '0';
    rtc_time[7] = (clock_reg[2] & 0x0F) + '0';
    rtc_time[8] = '\0';
    
    clcd_print(rtc_time,0xC0);
}

void get_time(unsigned char *clock_reg)
{
    clock_reg[0] = read_ds1307(HOUR_ADDR); // HH -> BCD 
    clock_reg[1] = read_ds1307(MIN_ADDR); // MM -> BCD 
    clock_reg[2] = read_ds1307(SEC_ADDR); // SS -> BCD 
}

void rtc_init_config(void) {
    init_i2c(100000); //100K
    init_ds1307();
}
