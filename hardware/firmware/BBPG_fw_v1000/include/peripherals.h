//------------------------------------------------------------------------------
// (c) Copyright 2012, Dialog Semiconductor BV
// All Rights Reserved
//------------------------------------------------------------------------------
// Dialog SEMICONDUCTOR CONFIDENTIAL
//------------------------------------------------------------------------------
// This code includes Confidential, Proprietary Information and is a Trade 
// Secret of Dialog Semiconductor BV. All use, disclosure, and/or reproduction 
// is prohibited unless authorized in writing.
//------------------------------------------------------------------------------
// Description: Peripheral include file

#include "stdint.h"

#ifndef PERI_H_INCLUDED
#define PERI_H_INCLUDED

void uart_init(void);
char uart_receive_byte(void);
void uart_send_byte(char ch);
void printf_byte(int a);
void printf_string(char * str);
void gpio_init(void);
void gpio_release(void);
void keyboard_init(void);
void keyboard_release(void);

void spi_init(void);
void spi_release(void);
void read_spi_flash(void);

int spi_flash_read_jedec_id(void);
short int spi_flash_read_man_dev_id(void);
char spi_serial_flash_access(char wr_data);
void spi_flash_write_enable(void);
void spi_flash_write_disable(void);
char spi_flash_read_status_reg1(void);
char spi_flash_read_status_reg2(void);
void spi_flash_sector_erase(int page_address);
void spi_flash_block32_erase(int page_address);
void spi_flash_block64_erase(int page_address);
void spi_flash_chip_erase(void);
void spi_flash_page_program(int page_address, char *wr_data_ptr, short int byte_length);

// void i2c_init(void);
// void i2c_release(void);
uint8_t random_read_i2c_eeprom(uint8_t address);
uint16_t sequential_read_i2c_eeprom(uint8_t * rd_data_ptr , uint8_t address , uint16_t size);
void write_byte_i2c_eeprom(uint8_t address , uint16_t wr_data);
void write_page_i2c_eeprom(uint8_t address,uint8_t* wr_data_ptr, uint16_t size);

void print_menu(void);
void print_input(void);
void uart_test(void);
void swt_test(void);
void spi_test(void);
void i2c_test(void);
void quad_test(void);
void buzz_test(void);
void exit_test(void);



void i2cInit(void);

uint8_t i2cReadSingleByte(uint8_t Address, uint8_t TarRegister);

typedef struct 
{
	uint8_t dev_type;
	uint8_t dev_address_mode;
	uint16_t dev_address;
	
	uint8_t speed;
	
	uint8_t GPIO_PORT;
	uint8_t GPIO_SDA_PIN;
	uint8_t GPIO_SCL_PIN;
}iic_dev;
	
#endif
