#include "touch_detect.h"
#include "bbpg_led.h"
//P2_2

static uint8_t flag;

void touchDetectIRQ(void)
{
    if(flag)
    {
        ledGreenOff();
        flag = 0;
    }
    else
    {
        ledGreenOn();
        flag = 1;
    }
}

void touchModuleInit(GPIO_PORT touch_port, GPIO_PIN touch_pin)
{
    RESERVE_GPIO(TOUCH_DETECT, touch_port, touch_pin, PID_GPIO);
    GPIO_ConfigurePin(touch_port, touch_pin, INPUT, PID_GPIO, false);
      
    GPIO_EnableIRQ(touch_port, touch_pin, GPIO2_IRQn, false, true, 1);
    
    //GPIO_RegisterCallback(GPIO2_IRQn, touchDetectIRQ);    
    
    //NVIC_DisableIRQ(GPIO2_IRQn);
    //NVIC_SetPriority(GPIO2_IRQn, 2);
    //NVIC_EnableIRQ(GPIO2_IRQn);
    //NVIC_ClearPendingIRQ(GPIO2_IRQn);
}


