
#include "../include/global_io.h"
#include "../include/gpio.h"
#include "../include/peripherals.h"
#include <core_cm0.h>

// interrupt handlers

static int timer_irq_num;
int wakeup_times;

int count_switch_on_low;
int count_switch_on_high;
	

void SWTIM_Handler(void) // Interrupt every 50 ms
{
	timer_irq_num++;

	 // during 1 seconds it is swicthed on
	if (timer_irq_num<20) // If the counters reach 100 ms (4)
	{
	SetBits16(P1_SET_DATA_REG, P1_SET, 0x01); //P1_0 ON

	}
	// during 2 seconds it is swicthed on
	if (timer_irq_num>=21) // If the counters above 100 ms (5)
	{
	SetBits16(P1_RESET_DATA_REG, P1_RESET, 0x01); //P1_0 OFF
	}

	if (timer_irq_num>60) // If the counters above 5 sec (100)
	{
		timer_irq_num=0;
	  count_switch_on_low++;
		if (count_switch_on_low== 0xFF)
		{
			count_switch_on_high++;
			count_switch_on_low=0;
		}
		printf_string("          P1_0 has been powered-on/off ");
	  printf_byte(count_switch_on_high);
		printf_byte(count_switch_on_low);
  	printf_string(" times.\n\r");

	}
	
	if ( (count_switch_on_high == 0x27) && (count_switch_on_low == 0x10 ) ) // 100 advertising // 00 64
	{
		NVIC_DisableIRQ(SWTIM_IRQn);  	// disable interrupt
		SetBits16(TIMER0_CTRL_REG,TIM0_CTRL,0); // Disable Timer 
		printf_string("\n\r\n\r************** End of the test **************\n\r\n\r");
	}		
}

		
