#include "oled_draw.h"

#include "ascii_12X24_lib.h"
#include "cn_16X16_lib.h"
#include "launch_64X32_lib.h"
#include "wait_connect_64X32_lib.h"
#include "identify_1_64X32_lib.h"
#include "identify_2_64X32_lib.h"
#include "connect_ok_64X32_lib.h"
#include "connect_loss_64X32_lib.h"

static uint8_t oled_virtual_ram[OLED_VIRTUAL_RAM_SIZE];

static void writeRamToOLED(void)
{
    oledFullWrite(oled_virtual_ram);
}

static void oledDrawFullWrite(uint8_t *tardat)
{
    uint16_t count;
    
    for(count=0; count<OLED_VIRTUAL_RAM_SIZE; count++)
    {
        oled_virtual_ram[count] = *(tardat + count);
    }
    writeRamToOLED();
}
    
void oledDrawPicture(PIC_SELECT pictype)
{
    oledDisplayOff();
    switch(pictype)
    {
        case(LAUNCH_PIC):
            oledDrawFullWrite(launchBitmapDot);
            break;
        case(WAIT_CONNECT_PIC):
            oledDrawFullWrite(waitConnectBitmapDot);
            break;
        case(INDENTIFY_1_PIC):
            oledDrawFullWrite(identify1BitmapDot);
            break;
        case(INDENTIFY_2_PIC):
            oledDrawFullWrite(identify2BitmapDot);
            break;        
        case(CONNECT_OK_PIC):
            oledDrawFullWrite(connectOkBitmapDot);
            break;
        
        case(CONNECT_LOSS_PIC):
            oledDrawFullWrite(connectLossBitmapDot);
            break;
        
        default:
            break;
    }
    oledDisplayOn();
}

void oledDrawClear(void)
{
    uint16_t i;
    
    for(i=0; i<OLED_VIRTUAL_RAM_SIZE; i++)
    {
        oled_virtual_ram[i] = 0x00;
    }
    oledDisplayOff();
    writeRamToOLED();
    oledDisplayOn();
}

void oledDrawPointTest(uint8_t x, uint8_t y, uint8_t val)
{
    if(val)
    {
        oled_virtual_ram[(y/8)*64+x] = oled_virtual_ram[(y/8)*64+x] | (0x01<<(y%8));
    }
    else
    {
        oled_virtual_ram[(y/8)*64+x] = oled_virtual_ram[(y/8)*64+x] & (~(0x01<<(y%8)));
    }
    
    writeRamToOLED();
}


static void oledDrawPoint(uint8_t x, uint8_t y, uint8_t val)
{
    if(val)
    {
        oled_virtual_ram[(y/8)*64+x] = oled_virtual_ram[(y/8)*64+x] | (0x01<<(y%8));
    }
    else
    {
        oled_virtual_ram[(y/8)*64+x] = oled_virtual_ram[(y/8)*64+x] & (~(0x01<<(y%8)));
    }    
}


void oledDraw12X24Pic(uint8_t x, uint8_t y, uint8_t* map)
{
    uint8_t mapX, mapY;
    
    for(mapX=0; mapX<12; mapX++)
    {
        for(mapY=0; mapY<24; mapY++)
        {
            oledDrawPoint((x+mapX>=64)?(x+mapX-64):((x+mapX<0)?(x+mapX+64):(x+mapX)), 
                          (y+mapY>=32)?(y+mapY-32):((y+mapY<0)?(y+mapY+32):(y+mapY)), 
                          (*(map+(mapY/8)*12+mapX)) & (0x01<<(mapY%8)));            
        }
    }
    
    writeRamToOLED();
}

void oledDraw16X16Pic(uint8_t x, uint8_t y, uint8_t* map)
{
    uint8_t mapX, mapY;
    
    for(mapX=0; mapX<16; mapX++)
    {
        for(mapY=0; mapY<16; mapY++)
        {
            oledDrawPoint((x+mapX>=64)?(x+mapX-64):((x+mapX<0)?(x+mapX+64):(x+mapX)), 
                          (y+mapY>=32)?(y+mapY-32):((y+mapY<0)?(y+mapY+32):(y+mapY)),
                          (*(map+(mapY/8)*16+mapX)) & (0x01<<(mapY%8)));            
        }
    }
    
    writeRamToOLED();
}

uint8_t oledDrawCnChar(uint8_t x, uint8_t y, uint8_t* cnstr)
{
    uint8_t cnLibIndex;
    
    for(cnLibIndex=0; cnLibIndex< CN_LIB_LENGTH; cnLibIndex++)
    {
        if(((GB_16[cnLibIndex].Index[0])==(int8_t)(*cnstr)) && 
           ((GB_16[cnLibIndex].Index[1])==((int8_t)*(cnstr+1))))
        {
            oledDraw16X16Pic(x, y, GB_16[cnLibIndex].Msk);
            return 1;
        }
    }
    
    return 0;
}

void oledDrawAsciiChar(uint8_t x, uint8_t y, uint8_t ascii)
{
    oledDraw12X24Pic(x, y, ASCII_LIB + (ascii-0x20)*36);
}

static uint8_t *horizontalStringBuffer; 
static uint8_t horizontalStringLen;
static uint16_t xHaveScan;
static uint8_t refreshXStep;

#define OFFSET_64_DIVIDED_BY_12   2
#define OFFSET_32_DIVIDED_BY_24   4

void oledDrawHorizontalStringSet(uint8_t *str, uint8_t strLen,uint8_t refreshXStepPar)
{
    oledDisplayOff();
    horizontalStringBuffer = str;
    horizontalStringLen = strLen;
    xHaveScan = 0;
    refreshXStep = refreshXStepPar;
    oledDisplayOn();
}   

void oledDrawHorizontalStringRefresh(void)
{
    uint8_t scanX, scanY;
    //uint8_t currentDrawChar;
    
    uint8_t* mapBasic;
    uint8_t mapOffset;
       
    for(scanX=0; scanX<64; scanX++)
    {
        for(scanY=0; scanY<32; scanY++)
        {
            if( (scanX>OFFSET_64_DIVIDED_BY_12)&&(scanX<64-OFFSET_64_DIVIDED_BY_12)
              &&(scanY>OFFSET_32_DIVIDED_BY_24)&&(scanY<32-OFFSET_32_DIVIDED_BY_24))
            {                
                mapBasic = (ASCII_LIB+((*(horizontalStringBuffer + (xHaveScan + scanX-OFFSET_64_DIVIDED_BY_12)/12))-0x20)*36);
                mapOffset= ((scanY-OFFSET_32_DIVIDED_BY_24)/8)*12+(xHaveScan + scanX-OFFSET_64_DIVIDED_BY_12)%12;
                
                oledDrawPoint(scanX, scanY, (*(mapBasic+mapOffset)) & (0x01<<((scanY-OFFSET_32_DIVIDED_BY_24)%8)));              
            }       
        }
    }
    
    xHaveScan = xHaveScan + refreshXStep;
    if(xHaveScan >= horizontalStringLen * 12)
    {
        xHaveScan = 0;
    }
    oledDisplayOff();
    writeRamToOLED();
    oledDisplayOn();
}            

