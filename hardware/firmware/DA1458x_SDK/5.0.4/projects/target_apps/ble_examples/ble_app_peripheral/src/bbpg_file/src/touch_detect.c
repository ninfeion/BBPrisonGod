#include "touch_detect.h"
#include "bbpg_led.h"
//P2_2

void touchModuleInit(GPIO_PORT touch_port, GPIO_PIN touch_pin)
{
    RESERVE_GPIO(TOUCH_DETECT, touch_port, touch_pin, PID_GPIO);
    GPIO_ConfigurePin(touch_port, touch_pin, INPUT, PID_GPIO, false);
      
    GPIO_EnableIRQ(touch_port, touch_pin, GPIO2_IRQn, false, true, 1);
    
    //GPIO_RegisterCallback(GPIO2_IRQn, touchDetectIRQ);    
}


