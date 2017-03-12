#ifndef _OLED_DRAW_H_
#define _OLED_DRAW_H_

#include "bbpg_oled_dis.h"

#define OLED_VIRTUAL_RAM_SIZE 256

typedef enum
{
    LAUNCH_PIC,
    WAIT_CONNECT_PIC,
    INDENTIFY_1_PIC,
    INDENTIFY_2_PIC,

    INDENTIFY_FAIL_PIC,
    
    CONNECT_OK_PIC,
    CONNECT_LOSS_PIC,
    
    UNDO_ALARM_PIC,

}PIC_SELECT;

typedef enum 
{
    MENU_BATTERY,
    MENU_STEP_COUNT,
    MENU_HEART_RATE,
    MENU_DISTANCE,
}MENU_DIS_SELECT;

void oledDrawClear(void);

void oledDrawPicture(PIC_SELECT pictype);

void oledDrawPointTest(uint8_t x, uint8_t y, uint8_t val);

void oledDrawAsciiChar(uint8_t x, uint8_t y, uint8_t ascii);
uint8_t oledDrawCnChar(uint8_t x, uint8_t y, uint8_t* cnstr);

void oledDrawTime(uint8_t hour_ten, uint8_t hour_zero, uint8_t min_ten, uint8_t min_zero);
void oledDrawOtherMenu(MENU_DIS_SELECT dis_type, uint8_t num_1, uint8_t num_2, uint8_t num_3, uint8_t num_4);

void oledDrawHorizontalStringSet(uint8_t *str, uint8_t strLen,uint8_t refreshXStepPar);
void oledDrawHorizontalStringRefresh(void);

//void oledDraw16X16Pic(uint8_t x, uint8_t y, uint8_t* map);

#endif

