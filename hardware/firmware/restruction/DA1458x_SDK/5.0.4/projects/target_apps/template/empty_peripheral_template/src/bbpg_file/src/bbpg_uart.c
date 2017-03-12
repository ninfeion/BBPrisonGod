#include "bbpg_uart.h"


/*****************************************************************************************************/
/*							               U   A   R   T                                						             */
/*****************************************************************************************************/

void uart1Init(uint16_t baudr, uint8_t mode)
{
    RESERVE_GPIO(UART1_TX, UART1_GPIO_PORT, UART1_TX_PIN, PID_UART1_TX);
    RESERVE_GPIO(UART1_RX, UART1_GPIO_PORT, UART1_RX_PIN, PID_UART1_RX);
    
    GPIO_ConfigurePin(UART1_GPIO_PORT, UART1_TX_PIN, OUTPUT, PID_UART1_TX, false);
    GPIO_ConfigurePin(UART1_GPIO_PORT, UART1_RX_PIN, INPUT, PID_UART1_RX, false);
	
    SetBits16(CLK_PER_REG, UART1_ENABLE, 1);  	        // enable  clock for UART 1

    SetWord16(UART_LCR_REG, 0x80); 						// set bit to access DLH and DLL register
    // divisor = 1000000 / 115200 = 9
	
    SetWord16(UART_IER_DLH_REG, (baudr >> 8) & 0xFF);	//set high byte
    SetWord16(UART_RBR_THR_DLL_REG, baudr & 0xFF);		//set low byte
	
    SetWord16(UART_LCR_REG,mode);  						// no parity, 1 stop bit 8 data length and clear bit 8
	SetBits16(UART_MCR_REG, UART_SIRE, 0); 		 	    // mode 0 for normal , 1 for IRDA
	SetWord16(UART_IIR_FCR_REG,1);  					// enable fifo  
	SetBits16(UART_IER_DLH_REG,ERBFI_dlh0,1);  	        // IER access, enable receive data available interrupt  	
	
    
    //NVIC_DisableIRQ(UART_IRQn);
    //NVIC_SetPriority(UART_IRQn, 3); // 3
    //NVIC_EnableIRQ(UART_IRQn);
    //NVIC_ClearPendingIRQ(UART_IRQn);
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

void uart1Printf(uint8_t *TarStr, ...)
{
	__va_list args;
		
	uint8_t BUF[UART_PRINTF_BUFFER_SIZE]={0};
	uint16_t length;
	uint16_t sendCount=0;
		
	va_start(args, TarStr);
	length = vsprintf(BUF, TarStr, args);
	
	while(sendCount<length){
		uart_send_byte(BUF[sendCount]); 
		sendCount++;
	}
	
	va_end(args);
}	

