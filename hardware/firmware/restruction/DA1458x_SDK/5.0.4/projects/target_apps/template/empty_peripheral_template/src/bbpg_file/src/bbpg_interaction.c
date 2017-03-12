#include "bbpg_interaction.h"

#include "app_api.h"
#include "app.h"
#include "custs1_task.h"
#include "user_custs1_def.h"
#include "app_easy_timer.h"

#include "bbpg_adc.h"
#include "bbpg_alarm.h"
#include "bbpg_user_setup.h"
#include "bbpg_oled_draw.h"

#include "bbpg_event_schedule.h" // include the global variable "BBPG_CONNECTION_STATE"

#include "nm_step_count.h"

// BBPG INTERACTION VARIABLE
BBPG_OLED_DISPLAY_MODE_T BBPG_OLED_DISPLAY_MODE = BBPG_OLED_NONE_DISPLAY;
BBPG_BATTERY_STATE_T BBPG_BAT_STATE;
uint8_t BATTERY_PERCENTAGE = 0;

// TODO: UI frame list check initialization
static UI_FRAME_CLASS* ui_frame_list[BBPG_UI_MAX_FRAME] = {0};
static ORDER_DISPLAY_FRAME_SELECT_T ui_order_display_select = ORDER_DISPLAY_NONE;

static ke_msg_id_t ui_loop_display_timer = 0xFFFF;
static ke_msg_id_t ui_order_display_timer = 0xFFFF;
static uint8_t ui_order_just_enter_flag = 0;

static ke_msg_id_t identify_timer = 0xFFFF;
static ke_msg_id_t undo_alarm_timer = 0xFFFF;
static ke_msg_id_t battery_adc_timer = 0xFFFF;

static ke_msg_id_t time_tiktoc_timer = 0xFFFF;
static ke_msg_id_t accel_update_timer = 0xFFFF;

UI_FRAME_CLASS frame_waitConnect_1 =
{
    .priority = 0,
    .showTime = 200,
    .ui_frame_cb = waitConnect_1_cb
};

void waitConnect_1_cb(void)
{
    oledDrawClear();
    oledDrawCnChar(0, 8, "等"); 
    oledDrawCnChar(16,8, "待");
    oledDrawCnChar(32,8, "配");
    oledDrawCnChar(48,8, "对");
}

UI_FRAME_CLASS frame_waitConnect_2 =
 {
    .priority = 1,
    .showTime = 200,
    .ui_frame_cb = waitConnect_2_cb
};

void waitConnect_2_cb(void)
{
    oledDrawPicture(WAIT_CONNECT_PIC);
}

UI_FRAME_CLASS frame_identify_1 = 
{
    .priority = 0,
    .showTime = 200,
    .ui_frame_cb = identify_1_cb
};

void identify_1_cb(void)
{
    oledDrawPicture(INDENTIFY_1_PIC);
}

UI_FRAME_CLASS frame_identify_2 = 
{
    .priority = 1,
    .showTime = 200,
    .ui_frame_cb = identify_2_cb
};

void identify_2_cb(void)
{
    oledDrawPicture(INDENTIFY_2_PIC);
}

UI_FRAME_CLASS frame_identify_fail_1 = 
{
    .priority = 0,
    .showTime = 200,
    .ui_frame_cb = identify_fail_1
};

void identify_fail_1(void)
{
    oledDrawClear();
    oledDrawCnChar(0, 8, "设"); 
    oledDrawCnChar(16,8, "备");
    oledDrawCnChar(32,8, "异");
    oledDrawCnChar(48,8, "常");    
}

UI_FRAME_CLASS frame_identify_fail_2 = 
{
    .priority = 1,
    .showTime = 200,
    .ui_frame_cb = identify_fail_2
};

void identify_fail_2(void)
{
    oledDrawPicture(INDENTIFY_FAIL_PIC);
}

UI_FRAME_CLASS frame_identify_success_1 =
{
    .priority = 0,
    .showTime = 200,
    .ui_frame_cb = identify_success_1
};

void identify_success_1(void)
{
    oledDrawClear();
    oledDrawCnChar(0, 8, "配"); 
    oledDrawCnChar(16,8, "对");
    oledDrawCnChar(32,8, "成");
    oledDrawCnChar(48,8, "功");
}

UI_FRAME_CLASS frame_identify_success_2 = 
{
    .priority = 1,
    .showTime = 200,
    .ui_frame_cb = identify_success_2
};

void identify_success_2(void)
{
    oledDrawPicture(CONNECT_OK_PIC);
}

UI_FRAME_CLASS frame_connect_loss_1 = 
{
    .priority = 0,
    .showTime = 200,
    .ui_frame_cb = connect_loss_1 
};

void connect_loss_1(void)
{
    oledDrawClear();
    oledDrawCnChar(0, 8, "配"); 
    oledDrawCnChar(16,8, "对");
    oledDrawCnChar(32,8, "丢");
    oledDrawCnChar(48,8, "失");    
}

UI_FRAME_CLASS frame_connect_loss_2 = 
{
    .priority = 1,
    .showTime = 200,
    .ui_frame_cb = connect_loss_2 
};

void connect_loss_2(void)
{
    oledDrawPicture(CONNECT_LOSS_PIC);
}

UI_FRAME_CLASS frame_undo_alarm_1 =
{
    .priority = 2,
    .showTime = 200,
    .ui_frame_cb = undo_alarm_1
};

void undo_alarm_1(void)
{
    oledDrawClear();
    oledDrawCnChar(0, 8, "破"); 
    oledDrawCnChar(16,8, "拆");
    oledDrawCnChar(32,8, "警");
    oledDrawCnChar(48,8, "报");
}

UI_FRAME_CLASS frame_undo_alarm_2 =
{
    .priority = 3,
    .showTime = 200,
    .ui_frame_cb = undo_alarm_2
};

void undo_alarm_2(void)
{
    oledDrawPicture(UNDO_ALARM_PIC);
}

UI_FRAME_CLASS frame_battery_alarm =
{
    .priority = 4,
    .showTime = 200,
    .ui_frame_cb = battery_alarm
};

void battery_alarm(void)
{
    oledDrawClear();
    oledDrawCnChar(0, 8, "电"); 
    oledDrawCnChar(16,8, "量");
    oledDrawCnChar(32,8, "警");
    oledDrawCnChar(48,8, "报");
}

UI_FRAME_CLASS frame_wake_up_time_show =
{
    .priority = 5,
    .showTime = 500,
    .ui_frame_cb = wake_up_time_show
};

void wake_up_time_show(void)
{
    #include <time.h>
    struct tm *time_ptr;  

    time_ptr=localtime(&UNIX_TIMESTAMP); 
                    
    //oledDrawClear();
    //oledDrawAsciiChar(2 ,4,((uint8_t)(time_ptr->tm_hour/10)+0x30));
    //oledDrawAsciiChar(14,4,((uint8_t)(time_ptr->tm_hour%10)+0x30));
    //oledDrawAsciiChar(26,4,0x3a);
    //oledDrawAsciiChar(38,4,((uint8_t)(time_ptr->tm_min/10)+0x30));
    //oledDrawAsciiChar(50,4,((uint8_t)(time_ptr->tm_min%10)+0x30));

    oledDrawTime(((uint8_t)(time_ptr->tm_hour/10)+0x30), 
                 ((uint8_t)(time_ptr->tm_hour%10)+0x30), 
                 ((uint8_t)(time_ptr->tm_min/10)+0x30), 
                 ((uint8_t)(time_ptr->tm_min%10)+0x30));
}

UI_FRAME_CLASS frame_list_empty_show =
{
    .priority = 6,
    .showTime = 200,
    .ui_frame_cb = list_empty_show
};

void list_empty_show(void)
{
    oledDrawPicture(LAUNCH_PIC);
}

void uiLoopDisRegisterFrame(UI_FRAME_CLASS* tarFrame)
{
    ui_frame_list[tarFrame->priority] = tarFrame;
}

void uiLoopDisUnregisterFrame(uint8_t frameNum)
{
    ui_frame_list[frameNum] = NULL;
}

void BBPGInteractInputSend(BBPG_INTERACT_INPUT_TYPE_T input)
{
    switch(input)
    {
        case BBPG_INTERACT_WAIT_CONN_DIS:
            {
                if(BBPG_OLED_DISPLAY_MODE == BBPG_OLED_LOOP_DISPLAY)
                {
                    uiLoopDisRegisterFrame(&frame_waitConnect_1);
                    uiLoopDisRegisterFrame(&frame_waitConnect_2); 
                }
                else
                {
                    BBPG_OLED_DISPLAY_MODE = BBPG_OLED_LOOP_DISPLAY;
                
                    uiLoopDisRegisterFrame(&frame_waitConnect_1);
                    uiLoopDisRegisterFrame(&frame_waitConnect_2); 

                    if(ui_loop_display_timer == 0xFFFF)
                    {
                        ui_loop_display_timer = app_easy_timer(1, ui_loop_display_timer_cb_handler); 
                    }
                }
            }
            break;
            
        case BBPG_INTERACT_WAIT_IDENTIFY_DIS:
            {
                if(BBPG_OLED_DISPLAY_MODE == BBPG_OLED_LOOP_DISPLAY)
                {
                    uiLoopDisRegisterFrame(&frame_identify_1);
                    uiLoopDisRegisterFrame(&frame_identify_2);  
                }
                else
                {
                    BBPG_OLED_DISPLAY_MODE = BBPG_OLED_LOOP_DISPLAY;
                    
                    uiLoopDisRegisterFrame(&frame_identify_1);
                    uiLoopDisRegisterFrame(&frame_identify_2);
                    
                    if(ui_loop_display_timer == 0xFFFF)
                    {
                        ui_loop_display_timer = app_easy_timer(1, ui_loop_display_timer_cb_handler); 
                    }
                }
                
                identify_timer = app_easy_timer(1000, identify_timer_cb_handler); // 10s
            }
            break;
            
        case BBPG_INTERACT_CONN_OK_DIS:
            {
                //IDENTIFY SUCCESS
                uiLoopDisRegisterFrame(&frame_identify_success_1);
                uiLoopDisRegisterFrame(&frame_identify_success_2);
                
                if(identify_timer != 0xFFFF)
                {
                    app_easy_timer_cancel(identify_timer);
                    identify_timer = 0xFFFF;
                }
            }
            break;
            
        case BBPG_INTERACT_IDENTIFY_FAIL:
            {
                uiLoopDisRegisterFrame(&frame_identify_fail_1);
                uiLoopDisRegisterFrame(&frame_identify_fail_2);
            }
            break;
    
        case BBPG_INTERACT_LEFT_FROM_LAUNCH_PIC:
            {
                // THIS ACTION WILL BE EXECUTED ONCE, OCCUR BY TOUCH
                
                #ifdef BBPG_MOTOR_ENABLE
                    uint32_t motor_delay;
                                                
                    enableMotorGPIO();
                    motor_delay = BBPG_MOTOR_DELAY_PAR;
                    while(motor_delay --);
                    disableMotorGPIO(); 
                #endif
                
                uiLoopDisRegisterFrame(&frame_waitConnect_1);
                uiLoopDisRegisterFrame(&frame_waitConnect_2);          
                
                if(BBPG_OLED_DISPLAY_MODE != BBPG_OLED_LOOP_DISPLAY)
                {
                    BBPG_OLED_DISPLAY_MODE = BBPG_OLED_LOOP_DISPLAY;
                }
                
                if(ui_loop_display_timer == 0xFFFF)
                {
                    ui_loop_display_timer = app_easy_timer(1, ui_loop_display_timer_cb_handler);   
                }
            }
            break;
    
        case BBPG_INTERACT_DISCONNECT_DIS:
            {
                uiLoopDisRegisterFrame(&frame_connect_loss_1);
                uiLoopDisRegisterFrame(&frame_connect_loss_2);
            }
            break;
    
        case BBPG_INTERACT_UNDO_ALARM:
            {                
                //if(BBPG_OLED_DISPLAY_MODE == BBPG_OLED_ORDER_DISPLAY)
                //{
                //    BBPG_OLED_DISPLAY_MODE = BBPG_OLED_LOOP_DISPLAY;
                //}
                
                uiLoopDisRegisterFrame(&frame_undo_alarm_1);
                uiLoopDisRegisterFrame(&frame_undo_alarm_2);
                
                undo_alarm_timer = app_easy_timer(1, undo_alarm_timer_cb_handler);
            }
            break;
            
        case BBPG_INTERACT_UNDO_ALARM_RELIEVE:
            {    
                uiLoopDisUnregisterFrame(2);
                uiLoopDisUnregisterFrame(3);
                
                setPwm2Duty(100); 
                setPwm4Duty(0); 
        
                if (undo_alarm_timer != 0xFFFF)
                {
                    app_easy_timer_cancel(undo_alarm_timer);
                    undo_alarm_timer = 0xFFFF;
                }  
            }
            break;
            
        case BBPG_INTERACT_OLED_WAKEUP_LOOP_DIS:
            {
                oledExitingSleep(); 
                
                if(BBPG_OLED_DISPLAY_MODE != BBPG_OLED_LOOP_DISPLAY)
                {
                    BBPG_OLED_DISPLAY_MODE = BBPG_OLED_LOOP_DISPLAY;
                }
                
                if(ui_loop_display_timer == 0xFFFF)
                {
                    ui_loop_display_timer = app_easy_timer(1, ui_loop_display_timer_cb_handler); 
                }
            }
            break;
            
        case BBPG_INTERACT_OLED_WAKEUP_ORDER_DIS:  
            {
                #ifdef BBPG_MOTOR_ENABLE
                    uint32_t motor_delay;
                                                
                    enableMotorGPIO();
                    motor_delay = BBPG_MOTOR_DELAY_PAR;
                    while(motor_delay --);
                    disableMotorGPIO(); 
                #endif              

                oledExitingSleep(); 
                
                if(BBPG_OLED_DISPLAY_MODE != BBPG_OLED_ORDER_DISPLAY)
                {
                    BBPG_OLED_DISPLAY_MODE = BBPG_OLED_ORDER_DISPLAY;
                }
                
                ui_order_display_select = ORDER_DISPLAY_TIME;
                
                ui_order_just_enter_flag = 0;
                
                //wake_up_time_show();
                
                ui_order_display_timer = app_easy_timer(1, ui_order_display_timer_cb_handler); // directly show time ble will be cut, should return and call a call back handler show time
            }
            break;
            
        case BBPG_INTERACT_OLED_ORDER_CHANGE_FRAME:
            {      
                #ifdef BBPG_MOTOR_ENABLE
                    uint32_t motor_delay;
                                                
                    enableMotorGPIO();
                    motor_delay = BBPG_MOTOR_DELAY_PAR;
                    while(motor_delay --);
                    disableMotorGPIO(); 
                #endif             
                
                if (ui_order_display_timer != 0xFFFF)
                {
                    app_easy_timer_cancel(ui_order_display_timer);
                    ui_order_display_timer = 0xFFFF;
                } 
                
                switch(ui_order_display_select)
                {
                    case ORDER_DISPLAY_TIME:
                        ui_order_display_select = ORDER_DISPLAY_STEP_COUNT;
                        //oledDrawClear();
                        oledDrawOtherMenu(MENU_STEP_COUNT, 0x01 +0x30, 0x04+0x30, 0x01+0x30, 0x02+0x30);
                        break;
                    
                    case ORDER_DISPLAY_STEP_COUNT:
                        ui_order_display_select = ORDER_DISPLAY_BATTERY_VAL;
                        //oledDrawClear();
                        oledDrawOtherMenu(MENU_BATTERY, BATTERY_PERCENTAGE/100 +0x30, 
                                                        BATTERY_PERCENTAGE/10 +0x30, 
                                                        BATTERY_PERCENTAGE%10 +0x30,
                                                        0x3f); // percentage symbol
                        break;
                        
                    case ORDER_DISPLAY_BATTERY_VAL:
                        ui_order_display_select = ORDER_DISPLAY_HEART_RATE;
                        //oledDrawClear();
                        oledDrawOtherMenu(MENU_HEART_RATE, 0x10 +0x30, // blank space
                                                           0x00 +0x30, 0x07 +0x30, 0x02 +0x30);                    
                        break;
                    
                    case ORDER_DISPLAY_HEART_RATE:
                        ui_order_display_select = ORDER_DISPLAY_TIME;
                        //oledDrawClear();
                        wake_up_time_show();
                        break;
                    
                    default:
                        break;
                }
                
                ui_order_display_timer = app_easy_timer(500, ui_order_display_timer_cb_handler); // after 5s sleep
            }
            break;
            
        case BBPG_INTERACT_OLED_GOTO_SLEEP:
            {
                #ifdef BBPG_MOTOR_ENABLE
                    uint32_t motor_delay;
                                                
                    enableMotorGPIO();
                    motor_delay = BBPG_MOTOR_DELAY_PAR;
                    while(motor_delay --);
                    disableMotorGPIO(); 
                #endif
                
                uiLoopDisUnregisterFrame(0);
                uiLoopDisUnregisterFrame(1);
                
                uiLoopDisUnregisterFrame(6);
                
                oledEnterSleep();
                
                if(BBPG_OLED_DISPLAY_MODE == BBPG_OLED_LOOP_DISPLAY)
                {
                    if (ui_loop_display_timer != 0xFFFF)
                    {
                        app_easy_timer_cancel(ui_loop_display_timer);
                        ui_loop_display_timer = 0xFFFF;
                    }             
                }                    
                
                BBPG_OLED_DISPLAY_MODE = BBPG_OLED_NONE_DISPLAY;
            }
            break;
            
        case BBPG_INTERACT_BATTERY_MONITOR_INIT:
            {
                if(BBPG_BAT_STATE == BBPG_BAT_UNINIT)
                {
                    battery_adc_timer = app_easy_timer(1, battery_ad_update_timer_cb_handler);
                }
            }
            break;
            
        case BBPG_INTERACT_BATTERY_LOW_POWER_ALARM:
            {
                if(BBPG_OLED_DISPLAY_MODE == BBPG_OLED_LOOP_DISPLAY)
                {
                    uiLoopDisRegisterFrame(&frame_battery_alarm);
                }
            }
            break;
        
        case BBPG_INTERACT_BATTERY_LOW_POWER_ALARM_RELIEVE:
            {
                if(BBPG_OLED_DISPLAY_MODE == BBPG_OLED_LOOP_DISPLAY)
                {
                    uiLoopDisUnregisterFrame(4);
                }
            }            
            break;
            
        case BBPG_INTERACT_OLED_ORDER_TRANS_LOOP:
            {                
                if (ui_order_display_timer != 0xFFFF)
                {
                    app_easy_timer_cancel(ui_order_display_timer);
                    ui_order_display_timer = 0xFFFF;
                }
                
                if(ui_loop_display_timer == 0xFFFF)
                {
                    ui_loop_display_timer = app_easy_timer(1, ui_loop_display_timer_cb_handler); 
                }
                
                BBPG_OLED_DISPLAY_MODE = BBPG_OLED_LOOP_DISPLAY;
            }
            break;
            
        case BBPG_INTERACT_TIME_TIKTOC_INIT:
            {
                time_tiktoc_timer = app_easy_timer(1, time_tiktoc_timer_cb_handler);
            }
            break;
            
        case BBPG_INTERACT_ACCEL_TIMER_INIT:
            {
                //accel_update_timer = app_easy_timer(1, accel_update_timer_cb_handler);
            }
            break;
            
        default:
            break;
    }   
}
 
/*
void accel_update_timer_cb_handler(void)
{
    int16_t accelRaw[3];
    
    accel_update_timer = 0xFFFF;
    
    readMPU9250AccelRaw(accelRaw);
    nmStepCount(MOTION_TIMESTAMP_MS, accelRaw[0], accelRaw[1], accelRaw[2]);
    
    accel_update_timer = app_easy_timer(10, accel_update_timer_cb_handler);
}
*/

void time_tiktoc_timer_cb_handler(void)
{
    time_tiktoc_timer = 0xFFFF;
    
    static uint8_t unix_count = 0;
    if(unix_count++ >=10)
    {
        UNIX_TIMESTAMP++;
        unix_count = 0;
    }
    
    MOTION_TIMESTAMP_MS = MOTION_TIMESTAMP_MS + 100;
    
    time_tiktoc_timer = app_easy_timer(10/*100ms*/, time_tiktoc_timer_cb_handler); // never stop
}

void ui_order_display_timer_cb_handler(void)
{
    ui_order_display_timer = 0xFFFF;
    
    if(ui_order_just_enter_flag == 0)
    {
        ui_order_just_enter_flag = 1;
        
        wake_up_time_show();
         
        ui_order_display_timer = app_easy_timer(500, ui_order_display_timer_cb_handler); // after 5s sleep
    }
    else
    {   
        ui_order_display_select = ORDER_DISPLAY_NONE;
        
        BBPGEventInputSend(BBPG_EVENT_INPUT_ORDER_DISPLAY_TIMEOUT);
    }
}

void battery_ad_update_timer_cb_handler(void)
{
    float bat_adc_sample;
    
    battery_adc_timer = 0xFFFF;
    
    bat_adc_sample = adcGetP00VbatVal() *BBPG_BAT_CAL_K;
    
    if( bat_adc_sample > BBPG_BAT_WORRY_VAL )
    {
        BBPG_BAT_STATE = BBPG_BAT_OK;        
        BBPGInteractInputSend(BBPG_INTERACT_BATTERY_LOW_POWER_ALARM_RELIEVE);
    }
    else if((BBPG_BAT_WORRY_VAL >= bat_adc_sample) && (bat_adc_sample> BBPG_BAT_OMG_VAL))
    {
        BBPG_BAT_STATE = BBPG_BAT_WORRY;        
        BBPGInteractInputSend(BBPG_INTERACT_BATTERY_LOW_POWER_ALARM_RELIEVE);
    }
    else if( BBPG_BAT_OMG_VAL >= bat_adc_sample)
    { 
        BBPG_BAT_STATE = BBPG_BAT_OMG;
        BBPGInteractInputSend(BBPG_INTERACT_BATTERY_LOW_POWER_ALARM);
    }
    
    if(bat_adc_sample <= BBPG_BAT_OMG_VAL)
    {
        BATTERY_PERCENTAGE = 0;
    }
    else
    {
        BATTERY_PERCENTAGE = (uint8_t)(((bat_adc_sample-BBPG_BAT_OMG_VAL) /(BBPG_BAT_OK_VAL-BBPG_BAT_OMG_VAL)) *100);
    }
        
    battery_adc_timer = app_easy_timer(1000/*10s*/, battery_ad_update_timer_cb_handler);
}
 
void ui_loop_display_timer_cb_handler(void)
{
    UI_FRAME_CLASS* frame_ptr_temp;
    static uint8_t frame_count = 0;    
    uint8_t frame_empty_check_count;
    
    ui_loop_display_timer = 0xFFFF; // reset timer
    
    frame_ptr_temp = ui_frame_list[frame_count]; // for bug that the undo alarm cancel and display incorrectly ##DEBUG.1702191936 
    if(ui_frame_list[frame_count] != NULL)
    {   
        //ui_frame_list[frame_count]->ui_frame_cb(); ##DEBUG.1702191936 
        //ui_refresh_timer = app_easy_timer(ui_frame_list[frame_count]->showTime, ui_refresh_timer_cb_handler); ##DEBUG.1702191936 
        frame_ptr_temp->ui_frame_cb();
        ui_loop_display_timer = app_easy_timer(frame_ptr_temp->showTime, ui_loop_display_timer_cb_handler);
                
        frame_count = (frame_count<BBPG_UI_MAX_FRAME-1)?(frame_count+1):(0);
    }
    else
    {
        while(1)
        {
            frame_count = (frame_count<BBPG_UI_MAX_FRAME-1)?(frame_count+1):(0);
                    
            frame_ptr_temp = ui_frame_list[frame_count]; // for bug that the undo alarm cancel and display incorrectly ##DEBUG.1702191936 
            if(ui_frame_list[frame_count] != NULL)
            {
                //ui_frame_list[frame_count]->ui_frame_cb(); ##DEBUG.1702191936 
                //ui_refresh_timer = app_easy_timer(ui_frame_list[frame_count]->showTime, ui_refresh_timer_cb_handler); ##DEBUG.1702191936 
                frame_ptr_temp->ui_frame_cb();
                ui_loop_display_timer = app_easy_timer(frame_ptr_temp->showTime, ui_loop_display_timer_cb_handler);
                        
                frame_count = (frame_count<BBPG_UI_MAX_FRAME-1)?(frame_count+1):(0);
                break;
            }
            else    // check whether frame list empty or not.
            {
                for(frame_empty_check_count=0; frame_empty_check_count< BBPG_UI_MAX_FRAME; frame_empty_check_count++)
                {
                    if(ui_frame_list[frame_empty_check_count] != NULL)
                    {
                        break;
                    }
                }
                if(frame_empty_check_count == BBPG_UI_MAX_FRAME)
                {
                    uiLoopDisRegisterFrame(&frame_list_empty_show);
                                       
                    ui_loop_display_timer = app_easy_timer(1, ui_loop_display_timer_cb_handler);                       
                    break;
                }
            }
        }
    }
}

#include "app_api.h"
#include "app.h"

static void app_disconnect(void)
{
    struct gapc_disconnect_cmd *cmd = KE_MSG_ALLOC(GAPC_DISCONNECT_CMD,
                                                   KE_BUILD_ID(TASK_GAPC, app_env->conidx), 
                                                   TASK_APP,
                                                   gapc_disconnect_cmd);

    cmd->operation = GAPC_DISCONNECT;
    cmd->reason = CO_ERROR_REMOTE_USER_TERM_CON;

    // Send the message
    ke_msg_send(cmd);
}

void identify_timer_cb_handler(void)
{
    identify_timer = 0xFFFF;
    
    app_disconnect();
    
    BBPGInteractInputSend(BBPG_INTERACT_IDENTIFY_FAIL);
}

void undo_alarm_timer_cb_handler(void)
{
    static uint8_t ledCount;
    
    undo_alarm_timer = 0xFFFF;
    
    if(BBPG_OPTION.UNDO_ALARM_ENABLE)
    {
        if(BBPG_CONNECTION_STATE == BBPG_CONNECTED)
        {
            struct custs1_val_ntf_req* req;
            uint8_t breakNotify[32];
      
            //breakNotify[0] = 0x01;
            sprintf(breakNotify, "%s %d", DATA_UNDO_ALARM_SEND_TO_PHONE, UNIX_TIMESTAMP);
                    
            req = KE_MSG_ALLOC_DYN(CUSTS1_VAL_NTF_REQ,
                                   TASK_CUSTS1,
                                   TASK_APP,
                                   custs1_val_ntf_req,
                                   DEF_CUST1_BBPG_BREAK_CHECK_CHAR_LEN);

            req->conhdl = app_env->conhdl;
            req->handle = CUST1_IDX_BBPG_BREAK_CHECK_VAL;
            req->length = DEF_CUST1_BBPG_BREAK_CHECK_CHAR_LEN;
               
            memcpy(req->value, &breakNotify, DEF_CUST1_BBPG_BREAK_CHECK_CHAR_LEN);
                    
            ke_msg_send(req);
        }

        if(ledCount)
        {
            ledCount = ~ledCount;

            setPwm2Duty(0); // flash the led
            setPwm4Duty(50); // disable the sounder
        }
        else
        {
            ledCount = ~ledCount;

            setPwm2Duty(100);
            setPwm4Duty(50);                 
        }
    }    
    undo_alarm_timer = app_easy_timer(100, undo_alarm_timer_cb_handler); // 1s period
}

