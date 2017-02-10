#ifndef _BBPG_ADC_H_
#define _BBPG_ADC_H_

#include "stdint.h"
#include "bbpg_user_setup.h"
#include "gpio.h"
#include "adc.h"
#include "stdlib.h"

#define ADC_RESOLUTION      1023
#define ADC_ATTENUATION     3
#define ADC_VOLTAGE_REFER   1200  //mv

void adcIOInit(void);
float adcGetP00VbatVal(void);

#endif

