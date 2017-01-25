
#include "peripherals.h"
#include "global_io.h"
#include "gpio.h"
#include "core_cm0.h"
#include "stdio.h"


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
		
		SetWord16(P02_MODE_REG,FUNC_UART1_TX);		// set P0_4 to uart1 TX function
		SetWord16(P03_MODE_REG,FUNC_UART1_RX);		// set P0_5 to uart1 RX function 
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

/*		I I C     */

#define SEND_I2C_COMMAND(X)                SetWord16(I2C_DATA_CMD_REG, (X))

#define WAIT_WHILE_I2C_FIFO_IS_FULL()      while((GetWord16(I2C_STATUS_REG) & TFNF) == 0)

#define WAIT_UNTIL_I2C_FIFO_IS_EMPTY()     while((GetWord16(I2C_STATUS_REG) & TFE) == 0)

#define WAIT_UNTIL_NO_MASTER_ACTIVITY()    while((GetWord16(I2C_STATUS_REG) & MST_ACTIVITY) !=0)

#define WAIT_FOR_RECEIVED_BYTE()           while(GetWord16(I2C_RXFLR_REG) == 0)
/*
void i2cInit()//(iic_dev iic_dev_tar)
{
		SetBits16(P04_MODE_REG, PUPD, 0x00);
		SetBits16(P04_MODE_REG, PID, 	9);
	
		SetBits16(P05_MODE_REG, PUPD, 0x00);
		SetBits16(P05_MODE_REG, PID, 	10);
	
		SetBits16(CLK_PER_REG, I2C_ENABLE, 1);                                        // enable  clock for I2C
    SetWord16(I2C_ENABLE_REG, 0x0);                                               // Disable the I2C controller
    SetWord16(I2C_CON_REG, I2C_MASTER_MODE | I2C_SLAVE_DISABLE | I2C_RESTART_EN); // Slave is disabled
    SetBits16(I2C_CON_REG, I2C_SPEED, I2C_STANDARD);                                     // Set speed
    SetBits16(I2C_CON_REG, I2C_10BITADDR_MASTER, 0x00);                   			// Set addressing mode
	
    SetWord16(I2C_TAR_REG, 0x69 & 0x3FF);                                  // Set Slave device address
    SetWord16(I2C_ENABLE_REG, 0x1);                                               // Enable the I2C controller
    WAIT_UNTIL_NO_MASTER_ACTIVITY();                                              // Wait for I2C master FSM to become IDLE
}

uint8_t i2cReadSingleByte(uint8_t Address, uint8_t TarRegister)
{
		uint8_t Data;
		static uint16_t temp;
		
		temp = GetWord16(I2C_DATA_CMD_REG);
	
		SetBits16(I2C_TAR_REG, GC_OR_START, 1);							// write into fifo generate a start condition
		SetWord16(I2C_DATA_CMD_REG, (Address<<1)+0x01 );		// send salve address
		
		WAIT_UNTIL_I2C_FIFO_IS_EMPTY();
	
		SetBits16(I2C_TAR_REG, GC_OR_START, 0);							// write into fifo generate a general call
		SetWord16(I2C_DATA_CMD_REG, TarRegister );				  // send register address 
	
		WAIT_UNTIL_I2C_FIFO_IS_EMPTY();
		// while((GetWord16(I2C_TX_ABRT_SOURCE_REG)&0x01)==0); // wait ack
		
		SetBits16(I2C_TAR_REG, GC_OR_START, 1);
		SetWord16(I2C_DATA_CMD_REG, (Address<<1)+0x00 );		// read

		temp = GetWord16(I2C_DATA_CMD_REG);	
		WAIT_UNTIL_I2C_FIFO_IS_EMPTY();
	
		// while((GetWord16(I2C_TX_ABRT_SOURCE_REG)&0x01)==0);

		// WAIT_FOR_RECEIVED_BYTE();	                       		// Wait for received data
    temp = GetWord16(I2C_DATA_CMD_REG);
		// Data = ((uint8_t) (0x00FF & GetWord16(I2C_DATA_CMD_REG)));     // Get received byte
    
		return Data;
		// SetWord16(I2C_TAR_REG, 0x69 & 0x3FF); 
}

void i2cWriteSingleByte(uint8_t data)
{
		SEND_I2C_COMMAND(data & 0xFF);
}

*/
