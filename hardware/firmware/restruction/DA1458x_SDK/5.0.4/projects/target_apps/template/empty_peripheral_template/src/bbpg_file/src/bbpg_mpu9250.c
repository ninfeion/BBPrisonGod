#include "bbpg_mpu9250.h"

static void delay_ms(uint32_t time)
{
	uint16_t i,ii;
	for(i=0; i<=time; i++)
	{
        for(ii=0; ii<=160; ii++);
	}
}


void mpu9250Init(GPIO_PORT I2C_GPIO_PORT, GPIO_PIN I2C_SCL_PIN, GPIO_PIN I2C_SDA_PIN,
				 uint16_t dev_address, uint8_t speed, uint8_t address_mode, uint8_t address_size)
{
    RESERVE_GPIO(I2C_SCL, I2C_GPIO_PORT, I2C_SCL_PIN, PID_I2C_SCL);
    RESERVE_GPIO(I2C_SDA, I2C_GPIO_PORT, I2C_SDA_PIN, PID_I2C_SDA);
    
	GPIO_ConfigurePin(I2C_GPIO_PORT, I2C_SCL_PIN, INPUT, PID_I2C_SCL, false);
    GPIO_ConfigurePin(I2C_GPIO_PORT, I2C_SDA_PIN, INPUT, PID_I2C_SDA, false);
	
	hwI2CInit(dev_address, speed, address_mode, address_size);
	
    /*
     * BBFLIGHT SET
     *****
     */
    hwI2CWriteByte(RA_PWR_MGMT_1, 0x08);     // set PD_PTAT = 1
    delay_ms(1000);
    hwI2CWriteByte(RA_PWR_MGMT_2, 0x07);     // disable all gyro axis
	delay_ms(1000);
	hwI2CWriteByte(RA_SMPLRT_DIV, 0xFF);     // 0x00	SAMPLE_RATE=Internal_Sample_Rate / (1 + SMPLRT_DIV)
	//hwI2CWriteByte(RA_CONFIG, 0x02);         // 0x02	Set gyro sample rate to 1 kHz and DLPF to 92 Hz
	//hwI2CWriteByte(RA_GYRO_CONFIG, 0x18);    // 2000d/s
	hwI2CWriteByte(RA_ACCEL_CONFIG_1, 0x18); // 16g range
	hwI2CWriteByte(RA_ACCEL_CONFIG_2, 0x05); // Set accelerometer rate to 1 kHz and bandwidth to 10 Hz
    
    delay_ms(1000);
    
	#ifdef MPU_PASSMODE_ENABLED
        hwI2CWriteByte(RA_INT_PIN_CFG,0x02); // turn on Bypass Mode 
		delay_ms(1000);
	#endif
		
	#ifdef USE_MAG_PASSMODE
	// SetWord16(I2C_TAR
	
        hwI2CWriteByte(AK8963_CNTL2, AK8963_CNTL2_SRST); // address should be switch to ak's, reset AK8963
		delay_ms(100);
		hwI2CWriteByte(AK8963_CNTL,AK8963_POWER_DOWN); // address should be switch to ak's, into power down
		delay_ms(100);
	#endif       
    
    
    /*********************
     * BBPG SET
     *********************
     */
    /*
    hwI2CWriteByte(RA_PWR_MGMT_1, 0x28);        // set CYCLE = 1, set SLEEP = 0, set PD_PTAT = 1
    delay_ms(1000);
    hwI2CWriteByte(RA_PWR_MGMT_2, 0x07);        // disable all gyro axis
    delay_ms(1000);
    hwI2CWriteByte(RA_LP_ACCEL_ODR, 0x05);      // set the low power accel ODR: 7.81Hz
    
    hwI2CWriteByte(RA_SMPLRT_DIV, 0x00);        // low power mode, don't care this register
    hwI2CWriteByte(RA_CONFIG, 0x00);            // set fifo mode to 0: fifo would over written when full, if mode is 1 would not write when full
    hwI2CWriteByte(RA_ACCEL_CONFIG_1, 0x18);    // 16g accel full scale range, disable xyz self test
	hwI2CWriteByte(RA_ACCEL_CONFIG_2, 0x08);    // Set accel_fchoice_b = 1, then ACCEL_FCHOICE = 0(inverted);the ORD follow LP_ACCEL_ODR register
    
    hwI2CWriteByte(RA_FIFO_EN, 0x08);           // only enable accel fifo write
     
    delay_ms(1000);
    */
}

uint16_t readMPUFIFOCount(void)
{
    uint8_t buffer[2];
    //hwI2CReadMultipleBytes(buffer, RA_FIFO_CNT_H, 2);
    hwI2CReadByte(RA_FIFO_CNT_H, &buffer[0]);
    hwI2CReadByte(RA_FIFO_CNT_L, &buffer[1]);
    
    return ((uint16_t) buffer[0]<<8) | ((uint16_t) buffer[1]);
}

void readMPUFIFO(uint8_t* data, uint32_t size)
{
    hwI2CReadMultipleBytes(data, RA_FIFO_R_W, size);
}

uint8_t getMPU9250ID(void)
{
	uint8_t MPU_ID;
	hwI2CReadByte(RA_WHO_AM_I, &MPU_ID);
	return MPU_ID;
}

/*
uint8_t getAK8963ID_Bypass(void)
{
	Single_Write(MAG_ADDRESS,AK8963_CNTL1,0x11);//single read 
	delay_ms(2);	
	return Single_Read(MAG_ADDRESS, AK8963_WIA);
}
*/

bool mpu9250Check(void)
{
	if(getMPU9250ID() == VAL_DEVICE_ID)
		return true;
	else
		return false;
}
/*
bool AK8963_Check_Bypass(void)
{
	if(Get_AK8963_ID_Bypass() == VAL_MAG_DEVICE_ID )
		return TRUE;
	else
		return FALSE;
}
*/
void readMPU9250AccelRaw(int16_t *ACCELDATA)
{
	uint8_t BUF[6];
	
	hwI2CReadMultipleBytes(BUF, RA_ACCEL_XOUT_H, 6);
	ACCELDATA[0] = (int16_t)((BUF[0] << 8) | BUF[1]);
	ACCELDATA[1] = (int16_t)((BUF[2] << 8) | BUF[3]);
	ACCELDATA[2] = (int16_t)((BUF[4] << 8) | BUF[5]);
}

/*
void readMPU9250GyroRaw(int16_t *GYRODATA)
{
	uint8_t BUF[6];
	
	hwI2CReadMultipleBytes(BUF, RA_GYRO_XOUT_H, 6);
	GYRODATA[0] = (int16_t)((BUF[0] << 8) | BUF[1]);
	GYRODATA[1] = (int16_t)((BUF[2] << 8) | BUF[3]);
	GYRODATA[2] = (int16_t)((BUF[4] << 8) | BUF[5]);
}

int16_t readMPU9250TempRaw(void)
{
	uint8_t BUF[2];
	int16_t TEMPDATA;
	hwI2CReadByte(RA_TEMP_OUT_H, &BUF[0]);
	hwI2CReadByte(RA_TEMP_OUT_H, &BUF[1]);
	TEMPDATA = (int16_t)((BUF[0] << 8) | BUF[1]);
	return TEMPDATA;
}

void READ_MPU9250_Bypass_MAG_RAW(int16_t *MAGDATA)
{ 
	uint8_t BUF[6];
	// 每读一次，ak8963自动进入powerdown模式,每读一次都要重新设置单测量模式 地磁读的周期不能小于7ms
	// Single_Write(MAG_ADDRESS,AK8963_CNTL1,0x01); // 14位 single read mode
	Single_Write(MAG_ADDRESS,AK8963_CNTL1,0x11);    // 16位 single read mode
	delay_ms(2);

	Multiple_Read(MAG_ADDRESS ,AK8963_HXL , 6, BUF);
	MAGDATA[0] = (int16_t)((BUF[1] << 8) | BUF[0]);
	MAGDATA[1] = (int16_t)((BUF[3] << 8) | BUF[2]);
	MAGDATA[2] = (int16_t)((BUF[5] << 8) | BUF[4]);
}


void readMpu9250BypassMagRawStateMachineReady(uint8_t scale)
{
	if(scale ==0)
	{
		Single_Write(MAG_ADDRESS,AK8963_CNTL1,0x01);   //14位 single read mode
	}
	else if(scale ==1)
	{
		Single_Write(MAG_ADDRESS,AK8963_CNTL1,0x11);   //16位 single read mode
	}
}


void readMpu9250BypassMagRawStateMachineRead(int16_t *MAGDATA)
{ 
	uint8_t BUF[6];

	Multiple_Read(MAG_ADDRESS ,AK8963_HXL , 6, BUF);
	MAGDATA[0] = (int16_t)((BUF[1] << 8) | BUF[0]);
	MAGDATA[1] = (int16_t)((BUF[3] << 8) | BUF[2]);
	MAGDATA[2] = (int16_t)((BUF[5] << 8) | BUF[4]);
}


void imuCalibration(ImuData *tarData)
{
	uint8_t i,ii;
	int16_t accelRaw[3], gyroRaw[3];
	uint8_t magProm[3];
	int32_t accelSum[3] = {0, 0, 0};
	int32_t gyroSum[3]  = {0, 0, 0};
	
	for(i=0; i<30; i++)
	{
		READ_MPU9250_ACCEL_RAW(accelRaw);
		READ_MPU9250_GYRO_RAW(gyroRaw);
		
		for(ii=0; ii<3; ii++)
		{
			accelSum[ii] += accelRaw[ii];
			gyroSum[ii]  += gyroRaw[ii];
		}
	}
	
	for(i=0; i<3; i++)
	{
		tarData->accelOffset[i] = (int16_t)(accelSum[i]/30);
		tarData->gyroOffset[i]  = (int16_t)(gyroSum[i]/30);
	}
	tarData->accelOffset[2] = tarData->accelOffset[2] - (int16_t)ACCELLSB; // Z axis calibration
	
	Single_Write(MAG_ADDRESS, AK8963_CNTL1, AK8963_FUSE_ROM_ACCESS);
	delay_ms(10);
    Multiple_Read(MAG_ADDRESS , AK8963_ASAX, 3, magProm);
	for(i=0; i<3; i++)
	{
		tarData->magASA[i] = (int16_t)magProm[i] + 128;
	}
}
		
*/









