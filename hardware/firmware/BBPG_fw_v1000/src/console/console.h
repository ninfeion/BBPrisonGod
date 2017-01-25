#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#define UART_CONSOLE_RX_FIFO_SIZE 128

#include "uart.h"
#include "reg_uart.h"   // uart register
#include "systick.h"

#include "led.h"
#include "oled_dis.h"

void uartConsoleInit(void);
uint16_t crc_ccitt(uint8_t *tar, uint16_t len);

#endif


