#ifndef _OLED_DIS_H_
#define _OLED_DIS_H_

#include "gpio.h"
#include "hw_i2c.h"

#define OLED_I2C_ADDRESS 0x3c



void oledInit(GPIO_PORT oled_PowerControlPort,GPIO_PIN oled_PowerControlPin,
              GPIO_PORT oledI2CPort, GPIO_PIN oledI2CSCL, GPIO_PIN oledI2CSDA,
              GPIO_PORT reset_Port, GPIO_PIN reset_Pin);

void oledPowerOn(void);
void oledPowerOff(void);

void oledFill(unsigned char dat1,unsigned char dat2);


#endif


