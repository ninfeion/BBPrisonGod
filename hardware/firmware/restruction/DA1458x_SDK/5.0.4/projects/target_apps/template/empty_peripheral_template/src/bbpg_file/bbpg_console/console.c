#include "console.h"

static uint8_t rxFIFO[UART_CONSOLE_RX_FIFO_SIZE];
static uint16_t rxFIFOPtr;
//static uint16_t receiveCount;

//slip protrocl

#define COMM_HEAD               0xC0
#define COMM_REAR               0XC0
#define COMM_LENGTH_POS         1
#define COMM_RECEIVE_OVERFLOW   96
#define COMM_RECEIVE_TIMEOUT    20  // 20*100ms = 2sec


static uint16_t const ccitt_table[256] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
    0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
    0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
    0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
    0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
    0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
    0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
    0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
    0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
    0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
    0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
    0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
    0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
    0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
    0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
    0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
    0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
    0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
    0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
    0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
    0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
    0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
    0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};


uint16_t crc_ccitt(uint8_t *tar, uint16_t len)
{
    uint16_t crc = 0;

    while (len-- > 0)
    {
        crc = ccitt_table[(crc >> 8 ^ *tar++) & 0xff] ^ (crc << 8);
    }
    return crc; // ~crc;
}


void uartConsoleInit(void)
{
    uint16_t i;
    
    for(i=0; i<UART_CONSOLE_RX_FIFO_SIZE; i++)
    {
        rxFIFO[i] = 0;
    }
    rxFIFOPtr = 0;
    //receiveCount = 0;
}
    

uint8_t uartConsoleAFrameCommandAnalysis(uint8_t *commandFIFO, uint16_t fifoIndex)
{
    //uint16_t i;
    
    switch(commandFIFO[1])
    {
        case 0x01:  ledRedOn(); 
                    printf_string("RED LED ON\n");
                    break;
        case 0x02:  ledRedOff(); 
                    printf_string("RED LED OFF\n");
                    break;
        case 0x03:  ledGreenOn(); 
                    printf_string("GREEN LED ON\n");
                    break;
        case 0x04:  ledGreenOff(); 
                    printf_string("GREEN LED OFF\n");
                    break;
        
        case 0x05:  oledPowerOn();  
                    printf_string("OLED POWER ON\n");
                    break;
        case 0x06:  oledPowerOff(); 
                    printf_string("OLED POWER OFF\n");
                    break;
        
        //case 0x07:  ssd1306();
        //            printf_string("OLED INIT\n");
        //            break;
    }
    //for(i=0; i<*(commandFIFO + COMM_LENGTH_POS); i++)
    //{
    //    
    //}
    
    return 1;
}


void uartReceiveDataIsr(void)
{
    // static uint32_t timeRecord;
   
    if(uart_data_rdy_getf())    // Check register UART_LSR_REG ,data ready indicate bit
    {
        rxFIFO[rxFIFOPtr] = uart_rxdata_getf();
        if(rxFIFO[0] == COMM_HEAD)
        {
            // if(rxFIFOPtr == 0)
            // {
            //    timeRecord = getSystemTime();
            // }
            //else
            //{ 
            //    if((getSystemTime() - timeRecord) > COMM_RECEIVE_TIMEOUT)
            //    {
            //        rxFIFOPtr = 0;
            //        rxFIFO[0] = 0;
            //        printf_string("command receive timeout\n");
            //    }
            //}
            if((rxFIFO[rxFIFOPtr] == COMM_REAR) && (rxFIFOPtr > 0))
            {
                uartConsoleAFrameCommandAnalysis(rxFIFO, rxFIFOPtr);
                rxFIFOPtr = 0;
                rxFIFO[0] = 0;
                //receiveCount = 0;
            }
            else
            {
                if(rxFIFOPtr < COMM_RECEIVE_OVERFLOW)
                {
                    rxFIFOPtr ++;
                }
                else
                {
                    rxFIFOPtr = 0;
                    rxFIFO[0] = 0;
                }   
            }
            //receiveCount ++;  
        }   
        else 
        {
            printf_string("command invalid\n");
        }
    }
}
 
 
void UART_Handler(void)
{
	uint32_t interruptID;
    interruptID = 0x0F & GetWord32(UART_IIR_FCR_REG);

    if(interruptID!=NO_INT_PEND)
    {
        switch(interruptID)
        {
          case UART_TIMEOUT:break;
          case RECEIVED_AVAILABLE:
            {
                uartReceiveDataIsr();
            }
            break;

          case THR_EMPTY:break;

          default:break;
        }
    }
}




