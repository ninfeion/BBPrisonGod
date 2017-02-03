#ifndef _BBPG_LED_H_
#define _BBPG_LED_H_

#include "gpio.h"
#include "bbpg_user_setup.h"

void ledIOInit(GPIO_PORT led_port, GPIO_PIN red_pin, GPIO_PIN green_pin);

void ledRedOn(void);
void ledRedOff(void);
void ledGreenOn(void);
void ledGreenOff(void);

#endif

