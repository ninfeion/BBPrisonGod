/**
****************************************************************************************
*
* @file systick.h
*
* @brief SysTick driver header file.
*
* Copyright (C) 2014. Dialog Semiconductor Ltd, unpublished work. This computer 
 * program includes Confidential, Proprietary Information and is a Trade Secret of 
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited 
 * unless authorized in writing. All Rights Reserved.
*
* <bluetooth.support@diasemi.com> and contributors.
*
****************************************************************************************
*/
#ifndef _SYSTICK_H_
#define _SYSTICK_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include <stdbool.h>
#include <stdint.h>

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef void (*systick_callback_function_t)(void);

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Register Callback function for SysTick exception.
 * @param[in] callback Callback function's reference
 * @return void
 ****************************************************************************************
 */
void systickRegisterCallback(systick_callback_function_t callback);

/**
 ****************************************************************************************
 * @brief Function to start the SysTick timer.
 * @param[in] usec      The duration of the countdown
 * @param[in] exception Set to TRUE to generate an exception when the timer counts down
 *                      to 0, FALSE not to
 * @return void
 ****************************************************************************************
 */
void systickStart(uint32_t usec, uint8_t exception);

/**
 ****************************************************************************************
 * @brief Function to create a delay.
 * @param[in] usec the duration of the delay
 * @return void
 ****************************************************************************************
 */
void systickWait(uint32_t usec);

/**
 ****************************************************************************************
 * @brief Function to stop the SysTick timer.
 * @return void
 ****************************************************************************************
 */
void systickStop(void);

/**
 ****************************************************************************************
 * @brief Function to read the current value of the timer.
 * @return the current value of the timer
 ****************************************************************************************
 */
uint32_t systickValue(void);

uint32_t getSystemTime(void);

void resetSystemTimr(void);

#endif //_SYSTICK_H_
