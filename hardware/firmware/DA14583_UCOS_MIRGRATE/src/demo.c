
// blinky + UART test
// update: 20140605

#include "../include/global_io.h"
#include "../include/peripherals.h"
#include "../include/gpio.h"

int main (void)
{
		long i;
		
		// system init
		SetWord16(CLK_AMBA_REG, 0x00); 									// set clocks (hclk and pclk ) 16MHz
		SetWord16(SET_FREEZE_REG,FRZ_WDOG);							// stop watch dog	
    SetBits16(SYS_CTRL_REG,PAD_LATCH_EN,1);        	// open pads
    SetBits16(SYS_CTRL_REG,DEBUGGER_ENABLE,1);     	// open debugger
    SetBits16(PMU_CTRL_REG, PERIPH_SLEEP,0);       	// exit peripheral power down
		
		SetBits16(P25_MODE_REG, PUPD, 0x03);
		SetBits16(P26_MODE_REG, PUPD, 0x03);
	
		SetBits16(P27_MODE_REG, PUPD, 0x03);
	
		SetBits16(P00_MODE_REG, PUPD, 0x00);
	
		uart_init();
	
		while(1)
		{
			i++;
			if (i > 1000000) 	i = 0;
			
			if (i == 500000)		
			{				
				SetBits16(P2_SET_DATA_REG, P2_SET, 0x0E0); 		//P1.0 ON
				printf_string("led off\n");
			}
			if (i == 900000) 	
			{
				SetBits16(P2_RESET_DATA_REG, P2_RESET, 0x0E0); //P1.0 OFF
				printf_string("led on\n");
			}
		}
}


