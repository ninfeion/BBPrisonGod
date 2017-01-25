#ifndef _HW_I2C_H_
#define _HW_I2C_H_

#include "stdint.h"
#include "datasheet.h"
#include "ll.h"

#define I2C_MAX_RETRIES                 (200000)

enum I2C_SPEED_MODES
{
    I2C_STANDARD = 1,
    I2C_FAST,
};

enum I2C_ADDRESS_MODES
{
    I2C_7BIT_ADDR,
    I2C_10BIT_ADDR,
};

enum I2C_ADRESS_BYTES_COUNT
{
    I2C_1BYTE_ADDR,
    I2C_2BYTES_ADDR,
    I2C_3BYTES_ADDR,
};

typedef enum
{
    I2C_NO_ERROR,
    I2C_7B_ADDR_NOACK_ERROR,
    I2C_INVALID_EEPROM_ADDRESS
} i2c_error_code;


void hwI2CInit(uint16_t dev_address, uint8_t speed, uint8_t address_mode, uint8_t address_size);
void hwI2CRelease(void);
i2c_error_code hwI2CWaitAck(void);
static void hwI2CSendAddress(uint32_t address);
i2c_error_code hwI2CReadByte(uint32_t address, uint8_t *byte);
i2c_error_code hwI2CWriteByte(uint32_t address, uint8_t byte);
i2c_error_code hwI2CReadMultipleBytes(uint8_t *rd_data_ptr, uint32_t address, uint32_t size);
i2c_error_code hwI2CWriteMultipleBytes(uint8_t *wr_data_ptr, uint32_t address, uint16_t size);


#endif


