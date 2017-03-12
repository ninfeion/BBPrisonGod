#ifndef _OLED_DIS_H_
#define _OLED_DIS_H_

#include "gpio.h"
//#include "hw_i2c.h"

//#define OLED_I2C_ADDRESS 0x3c



void oledIOInit(GPIO_PORT oled_PowerControlPort,GPIO_PIN oled_PowerControlPin,
              GPIO_PORT oledI2CPort, GPIO_PIN oledI2CSCL, GPIO_PIN oledI2CSDA,
              GPIO_PORT reset_Port, GPIO_PIN reset_Pin);

void oledSSD1306Initialization(void);
void oledPowerOn(void);
void oledPowerOff(void);
void oledEnterSleep(void);
void oledExitingSleep(void);

void oledFillTest(uint8_t dat1, uint8_t dat2);
void oledFullWrite(uint8_t *data);

void oledDisplayOff(void);
void oledDisplayOn(void);

#endif


