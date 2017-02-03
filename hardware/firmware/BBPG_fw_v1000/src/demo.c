#include "global_io.h"
#include "gpio.h"

#include "hw_i2c.h"
#include "uart.h"
#include "mpu9250.h"
#include "led.h"

#include "oled_dis.h"

#include "console.h"
#include "systick.h"

#include "touch_detect.h"

void breakDetectInit(GPIO_PORT undo_port, GPIO_PIN undo_pin,
                     GPIO_PORT cut_port, GPIO_PIN cut_pin);

int main (void)
{
    long i;
	
    int16_t accelData[3];
    int16_t gyroData[3];
 	
    // system init
    SetWord16(CLK_AMBA_REG, 0x00); 					// set clocks (hclk and pclk ) 16MHz
    SetWord16(SET_FREEZE_REG,FRZ_WDOG);				// stop watch dog	
    SetBits16(SYS_CTRL_REG,PAD_LATCH_EN,1);        	// open pads
    SetBits16(SYS_CTRL_REG,DEBUGGER_ENABLE,1);     	// open debugger
    SetBits16(PMU_CTRL_REG, PERIPH_SLEEP,0);       	// exit peripheral power down
		
    ledIOInit(LED_PORT, 
              LED_RED_PIN, LED_GREEN_PIN);
		
    uart1Init(UART1_BAUDRATE, UART1_DATALENGTH);
	

    mpu9250Init(MPU_I2C_GPIO_PORT, 
                MPU_I2C_SCL_PIN, 
			    MPU_I2C_SDA_PIN, 
				MPU9250_ADDRESS, I2C_STANDARD, I2C_7BIT_ADDR, I2C_1BYTE_ADDR);

    oledInit(OLED_POWER_GPIO_PORT, OLED_POWER_GPIO_PIN,
             OLED_I2C_GPIO_PORT, 
             OLED_I2C_SCL_PIN, OLED_I2C_SDA_PIN,
             OLED_RES_PORT, OLED_RES_PIN);

    oledPowerOn();
    
    //systickStart(SYSTEM_TIME_PERIOD * 1000, TRUE); // 100ms trigger once isr
    
    //uartConsoleInit();
    
    touchModuleInit(TOUCH_IO_PORT, TOUCH_IO_PIN);
    
    oledFill(0x55, 0xaa);
    

    breakDetectInit(GPIO_PORT_2, GPIO_PIN_9,
                    GPIO_PORT_2, GPIO_PIN_3);
                    
    //GPIO_RegisterCallback(GPIO0_IRQn, undoDetectIRQ);
    //GPIO_RegisterCallback(GPIO1_IRQn, cutDetectIRQ);

    while(1)
    {
        if(GPIO_GetPinStatus(GPIO_PORT_2, GPIO_PIN_9))
        {
            ledGreenOn();
        }
        
        if(GPIO_GetPinStatus(GPIO_PORT_2, GPIO_PIN_3))
        {
            ledRedOn();
        }
    }
    
	while(1)
	{
		i++;
			
		if (i > 1000000) 	i = 0;
			
		if (i == 500000)		
		{				
            ledRedOff();
			uart1Printf("led off\n");
				
			readMPU9250AccelRaw(accelData);
			readMPU9250GyroRaw(gyroData);
				
			uart1Printf("ACCEL X:%d,Y:%d,Z:%d\n", accelData[0], accelData[1], accelData[2]);
			uart1Printf("GYRO X:%d,Y:%d,Z:%d\n", gyroData[0], gyroData[1], gyroData[2]);
		}
		if (i == 900000) 	
		{
			ledRedOn();
			uart1Printf("led on\n");
		}
			
	}
}

void undoDetectIRQ(void)
{
    ledGreenOff();
}

void cutDetectIRQ(void)
{
    ledRedOff();
}

void breakDetectInit(GPIO_PORT undo_port, GPIO_PIN undo_pin,
                     GPIO_PORT cut_port, GPIO_PIN cut_pin)
{
    RESERVE_GPIO(UNDO_DETECT, undo_port, undo_pin, PID_GPIO);
    RESERVE_GPIO(CUT_DETECT, cut_port, cut_pin, PID_GPIO);
    
    GPIO_ConfigurePin(undo_port, undo_pin, INPUT, PID_GPIO, false);
    GPIO_ConfigurePin(cut_port, cut_pin, INPUT, PID_GPIO, false);

    GPIO_EnableIRQ(undo_port, undo_pin, GPIO0_IRQn, true, true, 10);
    GPIO_EnableIRQ(cut_port, cut_pin, GPIO1_IRQn, true, true, 10);
    
    GPIO_RegisterCallback(GPIO0_IRQn, undoDetectIRQ);
    GPIO_RegisterCallback(GPIO1_IRQn, cutDetectIRQ);
}



