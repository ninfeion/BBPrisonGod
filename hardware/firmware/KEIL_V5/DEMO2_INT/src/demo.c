
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
		
		SetBits16(P27_MODE_REG, PUPD, 0x03);
		SetBits16(P28_MODE_REG, PUPD, 0x03);
		SetBits16(P29_MODE_REG, PUPD, 0x03);

		while(1)
		{
			i++;
			if (i > 400000) 	i = 0;
			
			if (i == 0)		
			{				
				SetBits16(P2_SET_DATA_REG, P2_SET, 0x3ff); 		//P1.0 ON
			}
			if (i == 50000) 	
			{
				SetBits16(P2_RESET_DATA_REG, P2_RESET, 0x3ff); //P1.0 OFF
			}
		}
}


