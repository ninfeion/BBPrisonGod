#include "bbpg_hw_i2c.h"


/*
 * DEFINES
 ****************************************************************************************
 */

#define SEND_I2C_COMMAND(X)                SetWord16(I2C_DATA_CMD_REG, (X))

#define WAIT_WHILE_I2C_FIFO_IS_FULL()      while((GetWord16(I2C_STATUS_REG) & TFNF) == 0)

#define WAIT_UNTIL_I2C_FIFO_IS_EMPTY()     while((GetWord16(I2C_STATUS_REG) & TFE) == 0)

#define WAIT_UNTIL_NO_MASTER_ACTIVITY()    while((GetWord16(I2C_STATUS_REG) & MST_ACTIVITY) !=0)

#define WAIT_FOR_RECEIVED_BYTE()           while(GetWord16(I2C_RXFLR_REG) == 0)

/*
 * LOCAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

static uint8_t mem_address_size;    // 2 byte address is used or not.
static uint8_t i2c_dev_address;     // Device address

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void hwI2CInit(uint16_t dev_address, uint8_t speed, uint8_t address_mode, uint8_t address_size)
{
    mem_address_size = address_size;
    SetBits16(CLK_PER_REG, I2C_ENABLE, 1);                                        // enable  clock for I2C
    SetWord16(I2C_ENABLE_REG, 0x0);                                               // Disable the I2C controller
    SetWord16(I2C_CON_REG, I2C_MASTER_MODE | I2C_SLAVE_DISABLE | I2C_RESTART_EN); // Slave is disabled
    SetBits16(I2C_CON_REG, I2C_SPEED, speed);                                     // Set speed
    SetBits16(I2C_CON_REG, I2C_10BITADDR_MASTER, address_mode);                   // Set addressing mode
    SetWord16(I2C_TAR_REG, dev_address & 0x3FF);                                  // Set Slave device address
    SetWord16(I2C_ENABLE_REG, 0x1);                                               // Enable the I2C controller
    WAIT_UNTIL_NO_MASTER_ACTIVITY();                                              // Wait for I2C master FSM to become IDLE
    i2c_dev_address = dev_address;
}


void hwI2CRelease(void)
{
    SetWord16(I2C_ENABLE_REG, 0x0);                             // Disable the I2C controller
    SetBits16(CLK_PER_REG, I2C_ENABLE, 0);                      // Disable clock for I2C
}   


i2c_error_code hwI2CWaitAck(void)
{
    uint16_t tx_abrt_source;

    // Check if ACK is received
    for (uint32_t i = 0; i < I2C_MAX_RETRIES; i++)
    {
        SEND_I2C_COMMAND(0x08);                                 // Make a dummy access
        WAIT_UNTIL_I2C_FIFO_IS_EMPTY();                         // Wait until Tx FIFO is empty
        WAIT_UNTIL_NO_MASTER_ACTIVITY();                        // Wait until no master activity
        tx_abrt_source = GetWord16(I2C_TX_ABRT_SOURCE_REG);     // Read the I2C_TX_ABRT_SOURCE_REG register
        GetWord16(I2C_CLR_TX_ABRT_REG);                         // Clear I2C_TX_ABRT_SOURCE register
        if ((tx_abrt_source & ABRT_7B_ADDR_NOACK) == 0)
        {
            return I2C_NO_ERROR;
        }
    }
    return I2C_7B_ADDR_NOACK_ERROR;
}

/**
 ****************************************************************************************
 * @brief Send I2C EEPROM memory address.
 * @param[in] address The I2C EEPROM memory address
 * @return void
 ****************************************************************************************
 */
static void hwI2CSendAddress(uint32_t address)
{
    switch(mem_address_size)
    {
        case I2C_2BYTES_ADDR:
             SetWord16(I2C_ENABLE_REG, 0x0);
             SetWord16(I2C_TAR_REG, i2c_dev_address | ((address & 0x10000) >> 16));  // Set Slave device address
             SetWord16(I2C_ENABLE_REG, 0x1);
             WAIT_UNTIL_NO_MASTER_ACTIVITY();            // Wait until no master activity
             SEND_I2C_COMMAND((address >> 8) & 0xFF);    // Set address MSB, write access
             break;

        case I2C_3BYTES_ADDR:
             SEND_I2C_COMMAND((address >> 16) & 0xFF);   // Set address MSB, write access
             SEND_I2C_COMMAND((address >> 8) & 0xFF);    // Set address MSB, write access
             break;
    }

    SEND_I2C_COMMAND(address & 0xFF);                    // Set address LSB, write access
}


i2c_error_code hwI2CReadByte(uint32_t address, uint8_t *byte)
{
    if (hwI2CWaitAck() != I2C_NO_ERROR)
    {
        return I2C_7B_ADDR_NOACK_ERROR;
    }

    // Critical section
    GLOBAL_INT_DISABLE();

    hwI2CSendAddress(address);

    SEND_I2C_COMMAND(0x0100);                       // Set R/W bit to 1 (read access)
    
    // End of critical section
    GLOBAL_INT_RESTORE();
    
    WAIT_FOR_RECEIVED_BYTE();                       // Wait for received data
    *byte = 0xFF & GetWord16(I2C_DATA_CMD_REG);     // Get received byte
    
    WAIT_UNTIL_I2C_FIFO_IS_EMPTY();                 // Wait until Tx FIFO is empty
    WAIT_UNTIL_NO_MASTER_ACTIVITY();                // wait until no master activity 

    return I2C_NO_ERROR;
}


i2c_error_code hwI2CReadMultipleBytes(uint8_t *rd_data_ptr, uint32_t address, uint32_t size)
{
    uint16_t j;
    
	if (hwI2CWaitAck() != I2C_NO_ERROR)
    {
				return I2C_7B_ADDR_NOACK_ERROR;
    }

    // Critical section
    GLOBAL_INT_DISABLE();

    hwI2CSendAddress(address);

    for (j = 0; j < size; j++)
    {
        WAIT_WHILE_I2C_FIFO_IS_FULL();              // Wait if Tx FIFO is full
        SEND_I2C_COMMAND(0x0100);                   // Set read access for <size> times
    }
    
    // End of critical section
    GLOBAL_INT_RESTORE();

    // Get the received data
    for (j = 0; j < size; j++)
    {
        WAIT_FOR_RECEIVED_BYTE();                   // Wait for received data
        *rd_data_ptr =(0xFF & GetWord16(I2C_DATA_CMD_REG));  // Get the received byte
        rd_data_ptr++;
    }
    
    WAIT_UNTIL_I2C_FIFO_IS_EMPTY();                 // Wait until Tx FIFO is empty
    WAIT_UNTIL_NO_MASTER_ACTIVITY();                // wait until no master activity 
		
    return I2C_NO_ERROR;
}


i2c_error_code hwI2CWriteByte(uint32_t address, uint8_t byte)
{
    if (hwI2CWaitAck() != I2C_NO_ERROR)
    {
        return I2C_7B_ADDR_NOACK_ERROR;
    }

    // Critical section
    GLOBAL_INT_DISABLE();
    
    hwI2CSendAddress(address);
    
    SEND_I2C_COMMAND(byte & 0xFF);                  // Send write byte
    
    // End of critical section
    GLOBAL_INT_RESTORE();
    
    WAIT_UNTIL_I2C_FIFO_IS_EMPTY();                 // Wait until Tx FIFO is empty
    WAIT_UNTIL_NO_MASTER_ACTIVITY();                // Wait until no master activity

    return I2C_NO_ERROR;
}


i2c_error_code hwI2CWriteMultipleBytes(uint8_t *wr_data_ptr, uint32_t address, uint16_t size)
{
    if (hwI2CWaitAck() != I2C_NO_ERROR)
    {
				return I2C_7B_ADDR_NOACK_ERROR;
    }

    // Critical section
    GLOBAL_INT_DISABLE();

    hwI2CSendAddress(address);

    do
    {
        WAIT_WHILE_I2C_FIFO_IS_FULL();          // Wait if I2C Tx FIFO is full
        SEND_I2C_COMMAND(*wr_data_ptr & 0xFF);  // Send write data
        wr_data_ptr++;
        size--;
    }
    while (size != 0);

    // End of critical section
    GLOBAL_INT_RESTORE();

    WAIT_UNTIL_I2C_FIFO_IS_EMPTY();             // Wait until Tx FIFO is empty
    WAIT_UNTIL_NO_MASTER_ACTIVITY();            // Wait until no master activity

    return I2C_NO_ERROR;
}
