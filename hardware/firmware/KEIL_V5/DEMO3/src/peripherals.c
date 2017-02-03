

#include "../include/global_io.h"
#include "../include/gpio.h"
#include "../include/core_cm0.h"


/*****************************************************************************************************/
/*							                         G  P  I  O                                                  */
/*****************************************************************************************************/

void gpio_release(void){
 	NVIC_DisableIRQ(GPIO0_IRQn);       // Disable this interrupt
	NVIC_DisableIRQ(GPIO1_IRQn);       // Disable this interrupt
	NVIC_DisableIRQ(GPIO2_IRQn);       // Disable this interrupt
	NVIC_DisableIRQ(GPIO3_IRQn);       // Disable this interrupt
	SetWord16(GPIO_IRQ0_IN_SEL_REG,NO_PIN);    // No Pin
	SetWord16(GPIO_IRQ1_IN_SEL_REG,NO_PIN);    // No Pins
	SetWord16(GPIO_IRQ2_IN_SEL_REG,NO_PIN);    // No Pins
	SetWord16(GPIO_IRQ3_IN_SEL_REG,NO_PIN);    // No Pins

	SetWord16(P20_MODE_REG,FUNC_GPIO|DIR_INPUT ); // Set P20 as output
	SetWord16(P23_MODE_REG,FUNC_GPIO|DIR_INPUT);	// Set P23 as input	with pulldown
	SetWord16(P24_MODE_REG,FUNC_GPIO|DIR_INPUT);	// Set P23 as input	with pulldown	
	SetWord16(P25_MODE_REG,FUNC_GPIO|DIR_INPUT);	// Set P23 as input with pulldown
	SetWord16(P26_MODE_REG,FUNC_GPIO|DIR_INPUT);	// Set P23 as input with pulldown
	SetWord16(P2_RESET_DATA_REG,0);
}

void gpio_init(void){
	SetWord16(P20_MODE_REG,FUNC_GPIO|DIR_OUTPUT  ); // Set P20 as output
	SetWord16(P23_MODE_REG,FUNC_GPIO|DIR_PULLDOWN);	// Set P23 as input	with pulldown
	SetWord16(P24_MODE_REG,FUNC_GPIO|DIR_PULLDOWN);	// Set P23 as input	with pulldown	
	SetWord16(P25_MODE_REG,FUNC_GPIO|DIR_PULLDOWN);	// Set P23 as input with pulldown
	SetWord16(P26_MODE_REG,FUNC_GPIO|DIR_PULLDOWN);	// Set P23 as input with pulldown
	SetWord16(P2_SET_DATA_REG,1);

	SetWord16(GPIO_DEBOUNCE_REG, 0xF3F );  		// enable debounce for GPIO 0..3 with time 63ms 
	SetWord16(GPIO_INT_LEVEL_CTRL_REG, 0xF00);  // wait for release, high pulse
	
	SetWord16(GPIO_IRQ0_IN_SEL_REG,P23_PIN);    // Assign to P23
	SetWord16(GPIO_IRQ1_IN_SEL_REG,P24_PIN);    // Assign to P24
	SetWord16(GPIO_IRQ2_IN_SEL_REG,P25_PIN);    // Assign to P25
	SetWord16(GPIO_IRQ3_IN_SEL_REG,P26_PIN);    // Assign to P26
	
	SetWord16( GPIO_RESET_IRQ_REG, 0x000F );  // reset the IRQ at the GPIO 0,1,2,3
    NVIC_EnableIRQ(GPIO0_IRQn);       // Enable this interrupt
	NVIC_EnableIRQ(GPIO1_IRQn);       // Enable this interrupt
	NVIC_EnableIRQ(GPIO2_IRQn);       // Enable this interrupt
	NVIC_EnableIRQ(GPIO3_IRQn);       // Enable this interrupt
}


/*****************************************************************************************************/
/*							               U   A   R   T                                						             */
/*****************************************************************************************************/

void uart_init(void){
	SetBits16(CLK_PER_REG, UART1_ENABLE, 1);  	// enable  clock for UART 1

	SetWord16(UART_LCR_REG, 0x80); // set bit to access DLH and DLL register
	// divisor = 1000000 / 115200 = 9
	SetWord16(UART_IER_DLH_REG,(9&0xFF>>8));//set high byte
	SetWord16(UART_RBR_THR_DLL_REG,9&0xFF);//set low byte
	SetWord16(UART_LCR_REG,3);  	// no parity, 1 stop bit 8 data length and clear bit 8
	SetBits16(UART_MCR_REG, UART_SIRE, 0);  // mode 0 for normal , 1 for IRDA
	SetWord16(UART_IIR_FCR_REG,1);  // enable fifo  
	SetBits16(UART_IER_DLH_REG,ERBFI_dlh0,0);  // IER access, disable interrupt for available data
	
	SetWord16(P04_MODE_REG,FUNC_UART1_TX);		// set P0_4 to uart1 TX function
	SetWord16(P05_MODE_REG,FUNC_UART1_RX);		// set P0_5 to uart1 RX function 
	//SetWord16(P00_MODE_REG,FUNC_UART1_TX);		// set P0_0 to uart1 TX function
	//SetWord16(P01_MODE_REG,FUNC_UART1_RX);		// set P0_1 to uart1 RX function 
}

char uart_receive_byte(void){
	do{
	}while((GetWord16(UART_LSR_REG)&0x01)==0);	// wait until serial data is ready 
	return 0xFF&GetWord16(UART_RBR_THR_DLL_REG);	// read from receive data buffer
}

void uart_send_byte(char ch){
	while((GetWord16(UART_LSR_REG)&0x20)==0);	// read status reg to check if THR is empty
	SetWord16(UART_RBR_THR_DLL_REG,(0xFF&ch)); // write to THR register
	return;
}

void printf_string(char * str){
	while(*str!=0){	      // while not reach the last string character
		uart_send_byte(*str); // send next string character
		str++;
	}
}

void printf_byte(int a){		  // print a Byte in hex format
	char b;
	b=((0xF0&a)>>4);
	b+= (b<10)?48:55;
	uart_send_byte(b);
	b=(0xF&a);
	b+= (b<10)?48:55;
	uart_send_byte(b);
}


