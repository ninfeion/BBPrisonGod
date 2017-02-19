/**
 ****************************************************************************************
 *
 * @file user_custs1_impl.c
 *
 * @brief Peripheral project Custom1 Server implementation source code.
 *
 * Copyright (C) 2015. Dialog Semiconductor Ltd, unpublished work. This computer
 * program includes Confidential, Proprietary Information and is a Trade Secret of
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited
 * unless authorized in writing. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "gpio.h"
#include "app_api.h"
#include "app.h"
#include "user_custs1_def.h"
#include "user_custs1_impl.h"
#include "user_peripheral.h"
#include "user_periph_setup.h"

/*
 * BBPG SET
 ****
 */
#include "bbpg_alarm.h"
#include "oled_draw.h"
#include "bbpg_user_setup.h"

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

//ke_msg_id_t timer_used;

/**
  *  BBPG, STATE MACHINE
  */

BBPG_STATE_T BBPG_STATE = BBPG_NO_CONNECTION;
BBPG_UI_STATE_T BBPG_UI_STATE = BBPG_UI_LAUNCH;
BBPG_UNDO_DETECT_STATE_T BBPG_UNDO_STATE = BBPG_UNDO_DETECT_NO_WORK;
BBPG_BATTERY_STATE_T BBPG_BAT_STATE = BBPG_BAT_UNINIT;
BBPG_ACCEL_METER_STATE_T BBPG_ACCEL_STATE;

BBPG_OPTION_T BBPG_OPTION =
{
    .UNDO_ALARM_ENABLE = 0,
    .MPU_WORK_ENABLE = 1,
};

uint32_t UNIX_TIMESTAMP = 0;

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */


/* for roll display */
//ke_msg_id_t oled_timer;

//void oled_timer_callback(void)
//{
//    oledDrawHorizontalStringRefresh();
//    
//    oled_timer = app_easy_timer(30, oled_timer_callback);
//}

void bbpg_control_wr_ind_handler(ke_msg_id_t const msgid,
                                 struct custs1_val_write_ind const *param,
                                 ke_task_id_t const dest_id,
                                 ke_task_id_t const src_id)
{
    uint8_t val[32];
    memcpy(&val, &param->value[0], param->length);
    
    uint16_t checksum;
    
    // decoder process
    if((val[0]  == '#') &&
       (val[1]  == '#') &&
       (val[13] == '%') &&
       (val[14] == '%'))
    {
        checksum = (((val[2] + val[3] + val[4] + val[5] + val[6] + val[7] + 
                      val[8] + val[9] + val[10] + val[11]) %26) +97);
        if(((uint8_t)checksum) == val[12])
        {
            // correct timestamp
            UNIX_TIMESTAMP = (uint32_t)(val[2]<<24 | val[3]<<16 | val[4]<<8 | val[5]) + BBPG_TIMEZONE_SET(8);
            
            switch(val[6]) // case command type
            {
                case BBPG_COM_TYPE_IDENTIFY:
                    switch(BBPG_STATE)
                    {
                        case(BBPG_IDENTIFY):
                            identifySuccessUiUpdateCallBack();

                            BBPG_STATE = BBPG_HAVE_CONNECTION;
                            break;
                            
                        default:
                            break;
                    }
                    break;
                    
                case BBPG_COM_TYPE_UNDO_ALARM_ENABLE:
                    
                    BBPG_OPTION.UNDO_ALARM_ENABLE = val[7];
                    break;
                
                case BBPG_COM_TYPE_JUST_UPDATE_TIME:
                    break;
                
                default:
                    break;
            }
        }
    }
     
    
    if(val[0] == 1)
    {
        switch(BBPG_STATE)
        {
            case(BBPG_IDENTIFY):
                /*
                switch(BBPG_UI_STATE)
                {
                    case(BBPG_UI_IDENTIFY_1):
                    case(BBPG_UI_IDENTIFY_2):
                        BBPG_UI_STATE = BBPG_UI_IDENTIFY_SUCCESS;
                        break;
                    default:
                        break;
                }
                */
                identifySuccessUiUpdateCallBack();

                BBPG_STATE = BBPG_HAVE_CONNECTION;
                break;
            
            default:
                break;
        }
    }
}

void bbpg_loss_check_wr_ind_handler(ke_msg_id_t const msgid,
                                 struct custs1_val_write_ind const *param,
                                 ke_task_id_t const dest_id,
                                 ke_task_id_t const src_id)
{
    uint8_t val[32];
    
    memcpy(&val, &param->value[0], param->length);
    
    if(val[0] == 1)
    {
        if((BBPG_STATE != BBPG_NO_CONNECTION) && (BBPG_STATE != BBPG_IDENTIFY))
        {
            struct custs1_val_ntf_req* req;
 
            sprintf(val, "%d", UNIX_TIMESTAMP);
                    
            req = KE_MSG_ALLOC_DYN(CUSTS1_VAL_NTF_REQ,
                                   TASK_CUSTS1,
                                   TASK_APP,
                                   custs1_val_ntf_req,
                                   DEF_CUST1_BBPG_LOSS_CHECK_CHAR_LEN);

            req->conhdl = app_env->conhdl;
            req->handle = CUST1_IDX_BBPG_LOSS_CHECK_VAL;
            req->length = DEF_CUST1_BBPG_LOSS_CHECK_CHAR_LEN;
               
            memcpy(req->value, &val, DEF_CUST1_BBPG_LOSS_CHECK_CHAR_LEN);
                   
            ke_msg_send(req);
        }        
        
    }    
}

void undo_detect_irq_handler(void)
{
    switch(BBPG_UNDO_STATE)
    {
        case(BBPG_UNDO_DETECT_SAFE):
            BBPG_UNDO_STATE = BBPG_UNDO_DETECT_ALARM;
            setUndoDetectTheConnection();
        
            undoDetectUiUpdateCallBack(false);

            undo_detect_timer = app_easy_timer(1/*10ms*/, undo_detect_timer_cb_handler); //launch alarm          
            break;

        case(BBPG_UNDO_DETECT_ALARM):
            BBPG_UNDO_STATE = BBPG_UNDO_DETECT_SAFE;
            setUndoDetectTheDisconnection();

            undoDetectUiUpdateCallBack(true);
        
            setPwm2Duty(100); 
            setPwm4Duty(0); 
        
            if (undo_detect_timer != 0xFFFF)
            {
                app_easy_timer_cancel(undo_detect_timer);
                undo_detect_timer = 0xFFFF;
            }                      
            break;
    }
}

void touch_detect_irq_handler(void)
{
    uint32_t motor_delay;
    
    switch(BBPG_STATE)
    {         
        case(BBPG_NO_CONNECTION):
            {
                switch(BBPG_UI_STATE)
                {
                    case(BBPG_UI_LAUNCH):
                        BBPG_UI_STATE = BBPG_UI_WORK;
                    
                        enableMotorGPIO();
                        motor_delay = BBPG_MOTOR_DELAY_PAR;
                        while(motor_delay --);
                        disableMotorGPIO();                  
                        break;   
                    
                    default:
                        break;
                }
                break;
            }
 
        case(BBPG_HAVE_CONNECTION):
            switch(BBPG_UI_STATE)
            {                    
                case(BBPG_UI_WORK):
                    switch(BBPG_UNDO_STATE)
                    {
                        case(BBPG_UNDO_DETECT_SAFE):
                            BBPG_UI_STATE = BBPG_UI_SLEEP;
                            BBPG_STATE = BBPG_HAVE_CONNECTION_OLED_SLEEP;

                            enableMotorGPIO();
                            motor_delay = BBPG_MOTOR_DELAY_PAR;
                            while(motor_delay --);
                            disableMotorGPIO();         
                            break;
                        
                        default:
                            break;
                    }                        
                    break;
                
                default:
                    break;
            }
            break;
                    
        case(BBPG_HAVE_CONNECTION_OLED_SLEEP):
             switch(BBPG_UI_STATE)
             {
                case(BBPG_UI_SLEEP):
                    BBPG_UI_STATE = BBPG_UI_WAKE_UP;
                    BBPG_STATE = BBPG_HAVE_CONNECTION;
                
                    enableMotorGPIO();
                    motor_delay = BBPG_MOTOR_DELAY_PAR;
                    while(motor_delay --);
                    disableMotorGPIO();  
                
                    ui_refresh_timer = app_easy_timer(1, ui_refresh_timer_cb_handler);  // oled wake up first callui_refresh_timer_cb_handler();  // oled wake up first call
                    
                    break;
                 
                default:
                    break;
             }
             break;
             
        default:
            break;
     }
}

void app_disconnect(void)
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


