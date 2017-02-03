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
BBPG_UNDO_DETECT_STATE_T BBPG_UNDO_STATE;

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */
/*
void user_custs1_ctrl_wr_ind_handler(ke_msg_id_t const msgid,
                                      struct custs1_val_write_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    uint8_t val = 0;
    memcpy(&val, &param->value[0], param->length);

    if (val != CUSTS1_CP_ADC_VAL1_DISABLE)
    {
        timer_used = app_easy_timer(APP_PERIPHERAL_CTRL_TIMER_DELAY, app_adcval1_timer_cb_handler);
    }
    else
    {
        if (timer_used != 0xFFFF)
        {
            app_easy_timer_cancel(timer_used);
            timer_used = 0xFFFF;
        }
    }
}

void user_custs1_led_wr_ind_handler(ke_msg_id_t const msgid,
                                     struct custs1_val_write_ind const *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id)
{
    uint8_t val = 0;
    memcpy(&val, &param->value[0], param->length);

    if (val == CUSTS1_LED_ON)
        GPIO_SetActive(GPIO_LED_PORT, GPIO_LED_PIN);
    else if (val == CUSTS1_LED_OFF)
        GPIO_SetInactive(GPIO_LED_PORT, GPIO_LED_PIN);
    else if (val == 2)
        ledRedOn();
    else if (val == 3)
        ledRedOff();
}

void app_adcval1_timer_cb_handler()
{
    struct custs1_val_ntf_req* req = KE_MSG_ALLOC_DYN(CUSTS1_VAL_NTF_REQ,
                                                      TASK_CUSTS1,
                                                      TASK_APP,
                                                      custs1_val_ntf_req,
                                                      DEF_CUST1_ADC_VAL_1_CHAR_LEN);

    // ADC value to be sampled
    static uint16_t sample;
    sample = (sample <= 0xffff) ? (sample + 1) : 0;

    req->conhdl = app_env->conhdl;
    req->handle = CUST1_IDX_ADC_VAL_1_VAL;
    req->length = DEF_CUST1_ADC_VAL_1_CHAR_LEN;
    memcpy(req->value, &sample, DEF_CUST1_ADC_VAL_1_CHAR_LEN);

    ke_msg_send(req);

    if (ke_state_get(TASK_APP) == APP_CONNECTED)
    {
        // Set it once again until Stop command is received in Control Characteristic
        timer_used = app_easy_timer(APP_PERIPHERAL_CTRL_TIMER_DELAY, app_adcval1_timer_cb_handler);
    }

}
*/

ke_msg_id_t oled_timer;

void oled_timer_callback(void)
{
    oledDrawHorizontalStringRefresh();
    
    oled_timer = app_easy_timer(30, oled_timer_callback);
}


void bbpg_control_wr_ind_handler(ke_msg_id_t const msgid,
                                 struct custs1_val_write_ind const *param,
                                 ke_task_id_t const dest_id,
                                 ke_task_id_t const src_id)
{
    uint8_t val[32];
    memcpy(&val, &param->value[0], param->length);
  
    if(val[0] == 1)
    {
        switch(BBPG_STATE)
        {
            case(BBPG_IDENTIFY):
                switch(BBPG_UI_STATE)
                {
                    case(BBPG_UI_IDENTIFY_1):
                    case(BBPG_UI_IDENTIFY_2):
                        BBPG_UI_STATE = BBPG_UI_IDENTIFY_SUCCESS;
                        break;
                    default:
                        break;
                }
                
                BBPG_STATE = BBPG_HAVE_CONNECTION;
                break;
            
            default:
                break;
        }
        //oledDrawHorizontalStringRefresh();
        //oled_timer = app_easy_timer(30, oled_timer_callback);
    }
    /*
    if (val != CUSTS1_CP_ADC_VAL1_DISABLE)
    {
        timer_used = app_easy_timer(APP_PERIPHERAL_CTRL_TIMER_DELAY, app_adcval1_timer_cb_handler);
    }
    else
    {
        if (timer_used != 0xFFFF)
        {
            app_easy_timer_cancel(timer_used);
            timer_used = 0xFFFF;
        }
    }
    */
}

void undo_detect_irq_handler(void)
{
    switch(BBPG_UNDO_STATE)
    {
        case(BBPG_UNDO_DETECT_SAFE):
            BBPG_UNDO_STATE = BBPG_UNDO_DETECT_ALARM;
            setUndoDetectTheConnection();

            undo_detect_timer = app_easy_timer(1/*10ms*/, undo_detect_timer_cb_handler); //launch alarm
            break;

        case(BBPG_UNDO_DETECT_ALARM):
            BBPG_UNDO_STATE = BBPG_UNDO_DETECT_SAFE;
            setUndoDetectTheDisconnection();

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

/*
void cut_detect_irq_handler(void)
{
    struct custs1_val_ntf_req* req = KE_MSG_ALLOC_DYN(CUSTS1_VAL_NTF_REQ,
                                                      TASK_CUSTS1,
                                                      TASK_APP,
                                                      custs1_val_ntf_req,
                                                      DEF_CUST1_BBPG_BREAK_CHECK_CHAR_LEN);

    static uint8_t sample;
    sample = (sample <= 0xff) ? (sample + 1) : 0;

    req->conhdl = app_env->conhdl;
    req->handle = CUST1_IDX_BBPG_BREAK_CHECK_VAL;
    req->length = DEF_CUST1_BBPG_BREAK_CHECK_CHAR_LEN;
    
    memcpy(req->value, &sample, DEF_CUST1_BBPG_BREAK_CHECK_CHAR_LEN);

    ke_msg_send(req);
}
*/

void touch_detect_irq_handler(void)
{
    switch(BBPG_STATE)
    {         
        case(BBPG_NO_CONNECTION):
            {
                switch(BBPG_UI_STATE)
                {
                    case(BBPG_UI_LAUNCH):
                        BBPG_UI_STATE = BBPG_UI_WAIT_CONNECT_1;
                        break;
                    default:
                        break;
                }
                break;
            }
 
        case(BBPG_HAVE_CONNECTION):
            switch(BBPG_UI_STATE)
            {                    
                case(BBPG_UI_CONNECT_OK_1):
                case(BBPG_UI_IDENTIFY_SUCCESS):
                    BBPG_UI_STATE = BBPG_UI_SLEEP;
                break;
            }
            BBPG_STATE = BBPG_HAVE_CONNECTION_OLED_SLEEP;
            break;
                    
        case(BBPG_HAVE_CONNECTION_OLED_SLEEP):
             switch(BBPG_UI_STATE)
             {
                 case(BBPG_UI_SLEEP):
                     BBPG_UI_STATE = BBPG_UI_WAKE_UP;
                     ui_refresh_timer = app_easy_timer(1, ui_refresh_timer_cb_handler);  // oled wake up first callui_refresh_timer_cb_handler();  // oled wake up first call
                     break;
                 
                 default:
                     break;
             }
             BBPG_STATE = BBPG_HAVE_CONNECTION;
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


