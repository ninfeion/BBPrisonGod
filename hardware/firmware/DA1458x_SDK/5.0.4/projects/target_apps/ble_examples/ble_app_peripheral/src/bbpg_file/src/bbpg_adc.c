#include "bbpg_adc.h"

void adcIOInit(void)
{
    RESERVE_GPIO(BAT_ADC_P00, BAT_ADC_PORT, BAT_ADC_PIN, PID_ADC);
    
    GPIO_ConfigurePin(BAT_ADC_PORT, BAT_ADC_PIN, INPUT, PID_ADC, false);
}

/**
 ****************************************************************************************
 * @brief Filters the adc_sample_value.
 * @param[in] new_adc_value The adc_sample value
 * @return The filtered output
 ****************************************************************************************
 */
static inline uint16_t battery_moving_average_filter(uint16_t new_adc_value)
{
    static uint32_t old_value __attribute__((section("retention_mem_area0"),zero_init));
    const int filter_coefficient = 4;     
    
    //this part of the filter is used to remove the delay on start up
    if ( old_value == 0)
        old_value = new_adc_value << filter_coefficient;
    else
    //this part filters the values 
        old_value = old_value - (old_value >> filter_coefficient) + new_adc_value;
    return ( old_value >> filter_coefficient);
}

/**
 ****************************************************************************************
 * @brief Filters the adc_sample_value.
 * @param[in] new_adc_value The adc_sample value
 * @return The filtered output
 ****************************************************************************************
 */
static inline uint16_t battery_moving_average_with_reaction_filter(uint16_t new_adc_value)
{
    static uint32_t old_value __attribute__((section("retention_mem_area0"),zero_init));
    //the filter implement a moving average on 2^filter_coefficient values
    const int filter_coefficient = 3;     

    //the adc_sample value is 11-bit we consider a change larger than 1.5% of
    //the reference voltage to be a big change and the filter will 
    //go immediately to the new value    
    const int considered_big_change = 32; 
    
    //this part of the filter is used to quickly jump to the new value when the 
    //sampling rate is to slow and the battery changes with a quick rate
    if (abs(old_value-new_adc_value)>considered_big_change)
        old_value = new_adc_value << filter_coefficient;
    else
    //this part filters fairly constant values to avoid 
    //the random noise in the measurement
        old_value = old_value - (old_value >> filter_coefficient) + new_adc_value;
    return ( old_value >> filter_coefficient);
}

//internal configuration option to select the filtering of the adc_values
//0<= No filter, 1<=Drop the last 4-bits, 2<= Moving average filter, 3<=Moving average, reactive at big changes
static const uint8_t battery_filter_option=0;

static inline uint16_t battery_filter_value (uint16_t new_adc_value)
{
     switch(battery_filter_option)
     {
         case 0:
                 return(new_adc_value);
         case 1:
                 return(new_adc_value&0xFFF0);
         case 2:
                 return(battery_moving_average_filter(new_adc_value));
         case 3:
                 return(battery_moving_average_with_reaction_filter(new_adc_value));
     }
     return (new_adc_value);
}

float adcGetP00VbatVal(void)
{
    uint32_t adc_sample;
    
    adc_calibrate();
    adc_init(GP_ADC_SE, GP_ADC_SIGN, GP_ADC_ATTN3X);
    adc_usDelay(20);

    adc_enable_channel(ADC_CHANNEL_P00);
    adc_sample = adc_get_sample();
    adc_disable();

    adc_sample = battery_filter_value(adc_sample);
    
    return ((float)(adc_sample * ADC_ATTENUATION * ADC_VOLTAGE_REFER)) / ADC_RESOLUTION;
}



