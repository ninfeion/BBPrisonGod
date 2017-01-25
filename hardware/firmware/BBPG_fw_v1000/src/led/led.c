#include "led.h"

static GPIO_PORT ledPort;
static GPIO_PIN	redPin;
static GPIO_PIN	greenPin;

void ledIOInit(GPIO_PORT led_port, GPIO_PIN red_pin, GPIO_PIN green_pin)
{
		ledPort = led_port;
		redPin = red_pin;
		greenPin = green_pin;
	
		GPIO_ConfigurePin(led_port, red_pin, OUTPUT, PID_GPIO, true);
		GPIO_ConfigurePin(led_port, green_pin, OUTPUT, PID_GPIO, true);
}


void ledRedOn(void)
{
		GPIO_SetInactive(ledPort, redPin);
}
	
void ledRedOff(void)
{
		GPIO_SetActive(ledPort, redPin);
}
	
void ledGreenOn(void)
{
		GPIO_SetInactive(ledPort, greenPin);
}

void ledGreenOff(void)
{
		GPIO_SetActive(ledPort, greenPin);
}



