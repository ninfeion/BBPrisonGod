#ifndef _SW_I2C_H_
#define _SW_I2C_H_

#include "../include/global_io.h"
#include "../include/peripherals.h"
#include "../include/gpio.h"
#include "stdio.h"
#include "systick.h"

#define SDA_H 	 {SetBits16(P05_MODE_REG, PUPD, 0x03); SetBits16(P0_SET_DATA_REG, P0_SET, 0x01<<5);}
#define SDA_L 	 {SetBits16(P05_MODE_REG, PUPD, 0x03); SetBits16(P0_RESET_DATA_REG, P0_RESET, 0x01<<5);}

#define SCL_H 	 {SetBits16(P04_MODE_REG, PUPD, 0x03); SetBits16(P0_SET_DATA_REG, P0_SET, 0x01<<4);}
#define SCL_L 	 {SetBits16(P04_MODE_REG, PUPD, 0x03); SetBits16(P0_RESET_DATA_REG, P0_RESET, 0x01<<4);}

extern uint16_t EXT_TIME;

void timer0Init(void);

void delay(void);
void delay_us(uint32_t time);

void i2cInit(void);
uint8_t SDA_READ(void);
bool I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack(void);
void I2C_NoAck(void); 
bool I2C_WaitAck(void);  
void I2C_SendByte(uint8_t SendByte); 
uint8_t I2C_ReceiveByte(void);
bool Single_Write(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t REG_data);	
uint8_t Single_Read(uint8_t SlaveAddress,uint8_t REG_Address);
uint8_t Multiple_Read(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t length, uint8_t *DATA);
uint8_t Multiple_Write(uint8_t SlaveAddress, uint8_t REG_Address, uint8_t length, uint8_t *DATA);

void mpu9250Init(void);



#define RA_SELF_TEST_X_GYRO			0X00
#define RA_SELF_TEST_Y_GYRO			0X01
#define RA_SELF_TEST_Z_GYRO			0X02

#define RA_SELF_TEST_X_ACCEL		0X0D
#define RA_SELF_TEST_Y_ACCEL		0X0E
#define RA_SELF_TEST_Z_ACCEL		0X0F

#define RA_XG_OFFSET_H					0X13
#define RA_XG_OFFSET_L					0X14
#define RA_YG_OFFSET_H					0X15
#define RA_YG_OFFSET_L					0X16
#define RA_ZG_OFFSET_H					0X17
#define RA_ZG_OFFSET_L					0X18

#define RA_SMPLRT_DIV						0X19 
#define RA_CONFIG								0X1A 
#define RA_GYRO_CONFIG					0X1B 
#define RA_ACCEL_CONFIG_1				0X1C 
#define RA_ACCEL_CONFIG_2				0X1D 

#define RA_LP_ACCEL_ODR					0X1E
#define RA_WOM_THR							0X1F
#define RA_FIFO_EN							0X23

#define RA_I2C_MST_CTRL       	0x24
#define RA_I2C_SLV0_ADDR        0x25
#define RA_I2C_SLV0_REG         0x26
#define RA_I2C_SLV0_CTRL        0x27
#define RA_I2C_SLV1_ADDR        0x28
#define RA_I2C_SLV1_REG         0x29
#define RA_I2C_SLV1_CTRL        0x2A
#define RA_I2C_SLV2_ADDR        0x2B
#define RA_I2C_SLV2_REG         0x2C
#define RA_I2C_SLV2_CTRL        0x2D
#define RA_I2C_SLV3_ADDR        0x2E
#define RA_I2C_SLV3_REG         0x2F
#define RA_I2C_SLV3_CTRL        0x30
#define RA_I2C_SLV4_ADDR        0x31
#define RA_I2C_SLV4_REG         0x32
#define RA_I2C_SLV4_DO          0x33
#define RA_I2C_SLV4_CTRL        0x34
#define RA_I2C_SLV4_DI          0x35
#define RA_I2C_MST_STATUS       0x36
#define RA_INT_PIN_CFG          0x37
#define RA_INT_ENABLE           0x38
#define RA_INT_STATUS           0x3A

#define RA_ACCEL_XOUT_H					0X3B 
#define RA_ACCEL_XOUT_L					0X3C
#define RA_ACCEL_YOUT_H					0X3D
#define RA_ACCEL_YOUT_L					0X3E
#define RA_ACCEL_ZOUT_H					0X3F
#define RA_ACCEL_ZOUT_L					0X40

#define RA_TEMP_OUT_H						0X41  
#define RA_TEMP_OUT_L						0X42

#define RA_GYRO_XOUT_H					0X43 
#define RA_GYRO_XOUT_L					0X44
#define RA_GYRO_YOUT_H					0X45
#define RA_GYRO_YOUT_L					0X46
#define RA_GYRO_ZOUT_H					0X47
#define RA_GYRO_ZOUT_L					0X48

#define RA_USER_CTRL        		0X6A 
#define RA_PWR_MGMT_1						0X6B 
#define RA_PWR_MGMT_2						0X6C 

#define RA_WHO_AM_I							0X75
#define VAL_DEVICE_ID       	  0X71

#define MPU9250_ADDRESS    			0xD2 


#endif
