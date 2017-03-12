#ifndef _BBPG_INTERACTION_H_
#define _BBPG_INTERACTION_H_

#include "stdint.h"

typedef enum
{
    BBPG_OLED_NONE_DISPLAY,
    BBPG_OLED_LOOP_DISPLAY,
    BBPG_OLED_ORDER_DISPLAY,
}BBPG_OLED_DISPLAY_MODE_T;

typedef enum
{
    BBPG_INTERACT_WAIT_CONN_DIS,
    BBPG_INTERACT_WAIT_IDENTIFY_DIS,
    BBPG_INTERACT_CONN_OK_DIS,
    
    BBPG_INTERACT_LEFT_FROM_LAUNCH_PIC,
    
    BBPG_INTERACT_DISCONNECT_DIS,
    
    BBPG_INTERACT_UNDO_ALARM,
    BBPG_INTERACT_UNDO_ALARM_RELIEVE,
    
    BBPG_INTERACT_IDENTIFY_FAIL,
    
    BBPG_INTERACT_OLED_WAKEUP_LOOP_DIS,
    BBPG_INTERACT_OLED_WAKEUP_ORDER_DIS,
    
    BBPG_INTERACT_OLED_ORDER_CHANGE_FRAME,
    
    BBPG_INTERACT_OLED_ORDER_TRANS_LOOP,
    
    BBPG_INTERACT_OLED_GOTO_SLEEP,
    
    BBPG_INTERACT_BATTERY_MONITOR_INIT,
    
    BBPG_INTERACT_BATTERY_LOW_POWER_ALARM,
    BBPG_INTERACT_BATTERY_LOW_POWER_ALARM_RELIEVE,
    
    BBPG_INTERACT_TIME_TIKTOC_INIT,
    BBPG_INTERACT_ACCEL_TIMER_INIT,
}BBPG_INTERACT_INPUT_TYPE_T;

typedef enum
{
    BBPG_BAT_UNINIT,
    
    BBPG_BAT_OK,
    BBPG_BAT_WORRY, // 5%-15%
    
    BBPG_BAT_OMG,   // 0%-5%
}BBPG_BATTERY_STATE_T;

typedef enum
{
    ORDER_DISPLAY_NONE,
    ORDER_DISPLAY_TIME,
    ORDER_DISPLAY_STEP_COUNT,
    ORDER_DISPLAY_BATTERY_VAL,
    ORDER_DISPLAY_HEART_RATE,
}ORDER_DISPLAY_FRAME_SELECT_T;

typedef void (* UI_FRAME_CALLBACK)(void);

typedef struct 
{
    uint8_t priority;
    uint16_t showTime;
    UI_FRAME_CALLBACK ui_frame_cb;
}UI_FRAME_CLASS;

extern BBPG_OLED_DISPLAY_MODE_T BBPG_OLED_DISPLAY_MODE;

void BBPGInteractInputSend(BBPG_INTERACT_INPUT_TYPE_T input);

void uiLoopDisRegisterFrame(UI_FRAME_CLASS* tarFrame);
void uiLoopDisUnregisterFrame(uint8_t frameNum);

void ui_loop_display_timer_cb_handler(void);
void ui_order_display_timer_cb_handler(void);

void identify_timer_cb_handler(void);
void undo_alarm_timer_cb_handler(void);
void battery_ad_update_timer_cb_handler(void);
void time_tiktoc_timer_cb_handler(void);
void accel_update_timer_cb_handler(void);

// loop display frame callback
void waitConnect_1_cb(void);
void waitConnect_2_cb(void);
void identify_1_cb(void);     
void identify_2_cb(void);
void identify_fail_1(void);
void identify_fail_2(void);
void identify_success_1(void);
void identify_success_2(void);
void connect_loss_1(void);
void connect_loss_2(void);
void undo_alarm_1(void);
void undo_alarm_2(void);
void battery_alarm(void);
void wake_up_time_show(void);
void list_empty_show(void);

#endif

