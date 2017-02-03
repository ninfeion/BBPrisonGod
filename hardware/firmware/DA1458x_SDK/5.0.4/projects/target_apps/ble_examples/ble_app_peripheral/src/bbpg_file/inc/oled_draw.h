#ifndef _OLED_DRAW_H_
#define _OLED_DRAW_H_

#include "oled_dis.h"

#define OLED_VIRTUAL_RAM_SIZE 256

typedef enum
{
    LAUNCH_PIC,
    WAIT_CONNECT_PIC,
    INDENTIFY_1_PIC,
    INDENTIFY_2_PIC,
    
    CONNECT_OK_PIC,
    CONNECT_LOSS_PIC,

}PIC_SELECT;

void oledDrawClear(void);

void oledDrawPicture(PIC_SELECT pictype);

void oledDrawPointTest(uint8_t x, uint8_t y, uint8_t val);

void oledDrawAsciiChar(uint8_t x, uint8_t y, uint8_t ascii);
uint8_t oledDrawCnChar(uint8_t x, uint8_t y, uint8_t* cnstr);

void oledDrawHorizontalStringSet(uint8_t *str, uint8_t strLen,uint8_t refreshXStepPar);
void oledDrawHorizontalStringRefresh(void);

//void oledDraw16X16Pic(uint8_t x, uint8_t y, uint8_t* map);

#endif

