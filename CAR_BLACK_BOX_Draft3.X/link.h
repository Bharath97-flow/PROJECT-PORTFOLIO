/* 
 * File:   link.h
 * Author: ASUS
 *
 * Created on 27 September, 2025, 6:36 PM
 */

#ifndef LINK_H

#define	LINK_H

#define PORTB0 RB0
#define PORTB1 RB1
#define PORTB2 RB2
#define PORTB3 RB3
#define PORTB4 RB4
#define PORTB5 RB5
#define PORTB6 RB6
#define PORTE0 RE0
#define PORTE1 RE1
#define PORTE2 RE2
#define FOSC 2000000
#define _XTAL_FREQ 2000000
#define INST_MODE 0
#define DATA_MODE 1
#include<string.h>
#include<stdio.h>
#pragma config WDTE = OFF
#define LIST    10
#define LOG_INDEX_ADDR 0x00

//                                                                      GLOBAL VARIABLES
volatile unsigned int tmr0_overflow_count = 0;
volatile int timeout_flag = 0; 
volatile unsigned int block_seconds;
volatile int block_active;
char gear_array[8]="NR1234C";
char password[5];
int gear_index=0;
int on_flag=1;
volatile int log_index=-1;
#pragma pack(push, 1)
typedef struct 
{
    char time[9];
    char event[2];
    char speed[3];
    
}log;
#pragma pack(pop)
int speed_log=0;
int event_log=0;
char gear_shift[2];
int lock=0;
unsigned char eeprom_busy;
//                                                                      TEXT VARIABLES
char speed_text[4];
char rtc_time[9];
char clock_reg[3];
//                                                                         LCD CONFIG

void clcd_write(unsigned char byte, unsigned char mode);
void lcd_config();
void clear_screen();

//                                                                 DASHBOARD FUNCTIONS dashboard.c

void dash_display();
int speed_display();
void speed_string(unsigned char *arr, unsigned int speed);
int password_check(char *temp_password,int new_pass);

//                                                          ANALOG TO DIGITAL CONVERSION speed_adc.c

void adc_init();
unsigned int read_adc();

//                                                                           DS 1307
void clcd_print(char *str, unsigned char addr);
void init_ds1307(void);
unsigned char read_ds1307(unsigned char addr);
void write_ds1307(unsigned char addr, unsigned char data);
void display_time(unsigned char *clock_reg);
void get_time(unsigned char *clock_reg);
void rtc_init_config(void);
void init_i2c(unsigned long baud);
static void i2c_wait_for_idle(void);
void i2c_start(void);
void i2c_rep_start(void);
void i2c_stop(void);
unsigned char i2c_read(unsigned char ack);
int i2c_write(unsigned char data);

//                                                                       I2C AND DS1307 CONSTANTS
#define SLAVE_WRITE             0b11010000
#define SLAVE_READ              0b11010001
#define SEC_ADDR                0x00
#define MIN_ADDR                0x01
#define HOUR_ADDR               0x02

//                                                                          TIMER CONFIGURATION
void __interrupt() timer_isr(void);
void timer0_init(void);
void timer1_init(void);

//                                                                          MENU AND ITS FUNCTIONS

void menu();
void view_log();
void set_time(void);
void change_password(void);
void download_log(void);
void clear_log(void);
void handle_rotation(void);
void change_password(void);
void my_eeprom_write(unsigned char addr, unsigned char data);
unsigned char my_eeprom_read(unsigned char addr);
unsigned char load_log_index(void);
void update_log_index(unsigned char index);
void eeprom_write_log(unsigned char index, const log *entry);
void eeprom_read_log(unsigned char index, log *entry);


#endif	/* LINK_H */

