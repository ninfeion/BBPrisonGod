#include "sw_i2c.h"

//#include "i2c.h"
//#include "delay.h"
#include "pwm.h"
uint32_t volatile TIMER_0_COUNT = 0;

void delay_sidan(uint32_t sidan)
{
		uint32_t i,ii;
		for(i=0; i<sidan; i++)
		{
				for(ii=0; ii<16; ii++);
		}
}

void delay_us(uint32_t time)
{
		TIMER_0_COUNT = 0;
		while(TIMER_0_COUNT <= time);
}


void timer0_general_user_callback_function()
{
		TIMER_0_COUNT ++;
}


void timer0Init()
{
    // Stop timer for enter settings
    timer0_stop();

    // register callback function for SWTIM_IRQn irq
    timer0_register_callback(timer0_general_user_callback_function);

    // Enable TIMER0 clock
    set_tmr_enable(CLK_PER_REG_TMR_ENABLED);

    // Sets TIMER0,TIMER2 clock division factor to 1, so TIM0 Fclk is F = 16MHz/1 = 16Mhz
    set_tmr_div(CLK_PER_REG_TMR_DIV_1);

    // clear PWM settings register to not generate PWM
    timer0_set_pwm_high_counter(0);
    timer0_set_pwm_low_counter(0);
    
    // Set timer with 16MHz source clock divided by no so Fclk = 16MHz/1 = 16MHz
    timer0_init(TIM0_CLK_FAST, PWM_MODE_ONE, TIM0_CLK_NO_DIV);

    // reload value for 100ms (T = 1/16MHz * 64 = 4us)
    timer0_set_pwm_on_counter(64);

    // Enable SWTIM_IRQn irq
    timer0_enable_irq();
  
    // Start Timer0
    timer0_start();

    // Disable TIMER0, TIMER2 clocks
    // set_tmr_enable(CLK_PER_REG_TMR_DISABLED);
}


void i2cInit(void)
{
		SetBits16(P04_MODE_REG, PUPD, 0x03);		// SCL
		SetBits16(P05_MODE_REG, PUPD, 0x03);		// SDA

		SDA_L; 
		SCL_L; 
}

uint8_t SDA_READ(void)
{
		SetBits16(P05_MODE_REG, PUPD, 0x00);		// input no resistors selected
		return (((uint8_t)GetWord16(P0_DATA_REG))& (0x01<<(5)))>>(5 );
}

bool I2C_Start(void) 
{ 
		SDA_H; 
		SCL_H; 
		//delay_us(1); // delay 10us
		delay_sidan(4);
		if(!SDA_READ())
				return FALSE; 
		SDA_L; 
		//delay_us(1); 
		delay_sidan(4);		
		if(SDA_READ()) 
			return FALSE; 
		SDA_L; 
		//delay_us(1);
		delay_sidan(4);		
		return TRUE; 
}

void I2C_Stop(void) 
{ 
		SCL_L; 
		delay_us(1); 
		SDA_L; 
		delay_us(1);
		SCL_H; 
		delay_us(1);
		SDA_H; 
		delay_us(1);
}

void I2C_Ack(void) 
{ 
		SCL_L; 
		delay_us(1);
		SDA_L; 
		delay_us(1);
		SCL_H; 
		delay_us(1);
		SCL_L; 
		delay_us(1);
}

void I2C_NoAck(void) 
{ 
		SCL_L; 
		delay_us(1);
		SDA_H; 
		delay_us(1);
		SCL_H; 
		delay_us(1);
		SCL_L; 
		delay_us(1);
}

bool I2C_WaitAck(void)  
{ 
		SCL_L; 
		delay_us(1);
		SDA_H; 
		delay_us(1);
		SCL_H; 
		delay_us(1);
		if(SDA_READ()) 
		{ 
			SCL_L; 
			return FALSE; 
		} 
		SCL_L; 
		return TRUE; 
}

void I2C_SendByte(uint8_t SendByte) 
{ 
		uint8_t i=8; 
		
		while(i--) 
		{ 
			SCL_L; 
			delay_us(1);
			if(SendByte&0x80) 
			{
				SDA_H;
			}			
			else
			{
				SDA_L;
			}				
			SendByte<<=1; 
			delay_us(1);
			SCL_H; 
			delay_us(1);
		} 
		SCL_L; 
}

uint8_t I2C_ReceiveByte(void) 
{  
    uint8_t i=8; 
    uint8_t ReceiveByte=0;
	
    SDA_H; 
    while(i--) 
    { 
      ReceiveByte<<=1;       
      SCL_L; 
			delay_us(1);
			SCL_H; 
      delay_us(1);
      if(SDA_READ()) 
      { 
        ReceiveByte|=0x01; 
      } 
    } 
    SCL_L; 
    return ReceiveByte; 
}

bool Single_Write(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t REG_data)		  
{
  	if(!I2C_Start())
		{
			return FALSE;
		}
    I2C_SendByte(SlaveAddress);  
    if(!I2C_WaitAck())
		{
			I2C_Stop(); 
			return FALSE;
		}
    I2C_SendByte(REG_Address );       
    I2C_WaitAck();	
    I2C_SendByte(REG_data);
    I2C_WaitAck();   
    I2C_Stop(); 
    delay_us(100);
    return TRUE;
}

uint8_t Single_Read(uint8_t SlaveAddress,uint8_t REG_Address)
{   
		uint8_t REG_data;  
   	
		if(!I2C_Start())
		return FALSE;
    I2C_SendByte(SlaveAddress); 
		if(!I2C_WaitAck())
		{
			I2C_Stop();
			return FALSE;
		}
    I2C_SendByte((uint8_t) REG_Address);      
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(SlaveAddress+1);
    I2C_WaitAck();
    REG_data= I2C_ReceiveByte();
    I2C_NoAck();
    I2C_Stop();
		return REG_data;
}						   

uint8_t Multiple_Read(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t length, uint8_t *DATA)
{
		uint8_t count,datatemp;
		
		I2C_Start();
		I2C_SendByte(SlaveAddress);
		I2C_WaitAck();
		I2C_SendByte(REG_Address); 
		I2C_WaitAck();
		I2C_Start();
		I2C_SendByte(SlaveAddress+1);
		I2C_WaitAck();
		for(count=0;count<length;count++)
		{
			if(count!= length-1 )
			{
				datatemp = I2C_ReceiveByte();
				I2C_Ack();
			}
			else
			{
				datatemp = I2C_ReceiveByte();
				I2C_NoAck();
			}
			DATA[count] = datatemp;
		}
		I2C_Stop();
		return count;
}


uint8_t Multiple_Write(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t length, uint8_t *DATA)
{
		uint8_t count = 0;
		I2C_Start();
		I2C_SendByte(SlaveAddress);	   
		I2C_WaitAck();
		I2C_SendByte(REG_Address);   
		I2C_WaitAck();	  
		for(count=0;count<length;count++)
		{
			I2C_SendByte(DATA[count]); 
			I2C_WaitAck(); 
		 }
		I2C_Stop();

    return 1; 
}


void mpu9250Init(void)
{
	uint8_t i;
	/*
	while(1)
	{
		SCL_L;
		SDA_L;
		delay_us(10);
		SCL_H;
		SDA_H;
		delay_us(10);
	}
*/
	
	Single_Write(MPU9250_ADDRESS,RA_PWR_MGMT_1, 0x00);	   // out of sleep mode

	delay_us(100000);                                    // to wait for 9250 reset done
	Single_Write(MPU9250_ADDRESS,RA_SMPLRT_DIV, 0x00);     // 0x00	SAMPLE_RATE=Internal_Sample_Rate / (1 + SMPLRT_DIV)
	Single_Write(MPU9250_ADDRESS,RA_CONFIG, 0x02);         // 0x02	Set gyro sample rate to 1 kHz and DLPF to 92 Hz
	Single_Write(MPU9250_ADDRESS,RA_GYRO_CONFIG, 0x18);    // 2000d/s
	Single_Write(MPU9250_ADDRESS,RA_ACCEL_CONFIG_1, 0x18); // 16g range
	Single_Write(MPU9250_ADDRESS,RA_ACCEL_CONFIG_2, 0x02); // Set accelerometer rate to 1 kHz and bandwidth to 92 Hz
	
	#ifdef MPU_PASSMODE_ENABLED
		Single_Write(MPU9250_ADDRESS,RA_INT_PIN_CFG,0x02); 	 // turn on Bypass Mode 
		delay_us(2500000); 
	#endif
	
	#ifdef USE_MAG_PASSMODE
		Single_Write(MAG_ADDRESS,AK8963_CNTL2, AK8963_CNTL2_SRST); 	// reset AK8963
		delay_us(2500000); 
		Single_Write(MAG_ADDRESS, AK8963_CNTL,AK8963_POWER_DOWN); 	// into power down
		delay_us(2500000);
	#endif
}

void delay(void) 
{    
		uint32_t i;    
		for(i=0;i<=100;i++); 
}



