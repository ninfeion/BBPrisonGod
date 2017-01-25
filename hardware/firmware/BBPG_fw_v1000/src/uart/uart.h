#ifndef _UART_H_
#define _UART_H_

#include "stdarg.h"
#include "stdio.h"
#include "gpio.h"

#include "user_setup.h"

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdint.h>       // integer definition
#include <stdbool.h>      // boolean definition

/*
 * DEFINES
 *****************************************************************************************
 */

/// Divider for 921600 bits/s
//#define UART_BAUDRATE_921K6         tbd
/// Divider for 460800 bits/s
//#define UART_BAUDRATE_460K8         tbd
/// Divider for 230400 bits/s
//#define UART_BAUDRATE_230K4         tbd2
/// Divider for 115200 bits/s
#define UART_BAUDRATE_115K2         9   // = 16000000 / (16 * 115200),  actual baud rate = 111111.111,  error = -3.549%
/// Divider for 57600 bits/s
#define UART_BAUDRATE_57K6          17  // = 16000000 / (16 * 57600),   actual baud rate = 58823.529,   error = 2.124%
/// Divider for 38400 bits/s
#define UART_BAUDRATE_38K4          26  // = 16000000 / (16 * 38400),   actual baud rate = 38461.538,   error = 0.16%
/// Divider for 28800 bits/s
#define UART_BAUDRATE_28K8          35  // = 16000000 / (16 * 28800),   actual baud rate = 28571.429,   error = -0.794%
/// Divider for 19200 bits/s
#define UART_BAUDRATE_19K2          52  // = 16000000 / (16 * 19200),   actual baud rate = 19230.769,   error = 0.16%
/// Divider for 9600 bits/s
#define UART_BAUDRATE_9K6           104 // = 16000000 / (16 * 9600),    actual baud rate = 9615.385,    error = 0.16%
/// Divider for 2400 bits/s
#define UART_BAUDRATE_2K4           417 // = 16000000 / (16 * 2400),    actual baud rate = 2398.082,    error = -0.08%


/*
 * ENUMERATION DEFINITIONS
 *****************************************************************************************
 */

/// Generic enable/disable enum for UART driver
enum
{
    /// uart disable
    UART_DISABLE = 0,
    /// uart enable
    UART_ENABLE  = 1
};

/// Character format
enum
{
    /// char format 5
    UART_CHARFORMAT_5 = 0,
    /// char format 6
    UART_CHARFORMAT_6 = 1,
    /// char format 7
    UART_CHARFORMAT_7 = 2,
    /// char format 8
    UART_CHARFORMAT_8 = 3	// 8bits
};

/// Stop bit
enum
{
    /// stop bit 1
    UART_STOPBITS_1 = 0,
    /* Note: The number of stop bits is 1.5 if a
     * character format with 5 bit is chosen */
    /// stop bit 2
    UART_STOPBITS_2 = 1
};

/// Parity bit
enum
{
    /// even parity
    UART_PARITYBIT_EVEN  = 0,
    /// odd parity
    UART_PARITYBIT_ODD   = 1,
    /// space parity
    UART_PARITYBIT_SPACE = 2, // The parity bit is always 0.
    /// mark parity
    UART_PARITYBIT_MARK  = 3  // The parity bit is always 1.
};

/* Error detection */
enum
{
    /// error detection disabled
    UART_ERROR_DETECT_DISABLED = 0,
    /// error detection enabled
    UART_ERROR_DETECT_ENABLED  = 1
};

/// status values
enum
{
    /// status ok
    UART_STATUS_OK,
    /// status not ok
    UART_STATUS_ERROR
};


enum UART_ID
{
    MODEM_STAT         = 0,
    NO_INT_PEND        = 1,
    THR_EMPTY          = 2,
    RECEIVED_AVAILABLE = 4,
    UART_TIMEOUT       = 12
};


void uart1Init(uint16_t baudr, uint8_t mode);
void uart1Printf(uint8_t *TarStr, ...);

void printf_string(char * str);

#endif


