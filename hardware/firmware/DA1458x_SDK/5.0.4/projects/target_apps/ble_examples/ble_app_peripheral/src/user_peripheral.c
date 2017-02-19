/**
 ****************************************************************************************
 *
 * @file user_peripheral.c
 *
 * @brief Peripheral project source code.
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

/**
 ****************************************************************************************
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"             // SW configuration
#include "user_peripheral.h"
#include "user_custs1_impl.h"
#include "arch_api.h"
#include "user_custs1_def.h"
#include "gap.h"

#include "bbpg_adc.h"
#include "bbpg_alarm.h"
#include "oled_draw.h"
#include "bbpg_user_setup.h"
#include "mpu9250.h"


/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

// Manufacturer Specific Data ADV structure type
struct mnf_specific_data_ad_structure
{
    uint8_t ad_structure_size;
    uint8_t ad_structure_type;
    uint8_t company_id[APP_AD_MSD_COMPANY_ID_LEN];
    uint8_t proprietary_data[APP_AD_MSD_DATA_LEN];
};

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

uint8_t app_connection_idx;
timer_hnd app_adv_data_update_timer_used;
timer_hnd app_param_update_request_timer_used;

/*
 * BBPG VARIABLE DEFINITIONS
 ****************************
 */
ke_msg_id_t ui_refresh_timer;
ke_msg_id_t identify_timer;
ke_msg_id_t undo_detect_timer;
ke_msg_id_t battery_monitor_timer;
ke_msg_id_t timer_counter_timer;
ke_msg_id_t accel_refresh_timer;

// Manufacturer Specific Data
struct mnf_specific_data_ad_structure mnf_data __attribute__((section("retention_mem_area0"),zero_init)); //@RETENTION MEMORY

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
*/

/**
 ****************************************************************************************
 * @brief Initialize Manufacturer Specific Data
 * @return void
 ****************************************************************************************
 */
static void mnf_data_init()
{
    mnf_data.ad_structure_size = sizeof(struct mnf_specific_data_ad_structure ) - sizeof(uint8_t); // minus the size of the ad_structure_size field
    mnf_data.ad_structure_type = GAP_AD_TYPE_MANU_SPECIFIC_DATA;
    mnf_data.company_id[0] = APP_AD_MSD_COMPANY_ID & 0xFF; // LSB
    mnf_data.company_id[1] = (APP_AD_MSD_COMPANY_ID >> 8 )& 0xFF; // MSB
    mnf_data.proprietary_data[0] = 0;
    mnf_data.proprietary_data[1] = 0;
}

/**
 ****************************************************************************************
 * @brief Update Manufacturer Specific Data
 * @return void
 ****************************************************************************************
 */
static void mnf_data_update()
{
    uint16_t data;

    data = mnf_data.proprietary_data[0] | (mnf_data.proprietary_data[1] << 8);
    data += 1;
    mnf_data.proprietary_data[0] = data & 0xFF;
    mnf_data.proprietary_data[1] = (data >> 8) & 0xFF;

    if (data == 0xFFFF) {
         mnf_data.proprietary_data[0] = 0;
         mnf_data.proprietary_data[1] = 0;
    }
}

/**
 ****************************************************************************************
 * @brief Advertisement data timer update callback function.
 * @return void
 ****************************************************************************************
*/
static void adv_data_update_timer_cb()
{
    app_easy_gap_advertise_stop();
}

/**
 ****************************************************************************************
 * @brief Parameter update request timer callback function.
 * @return void
 ****************************************************************************************
*/
static void param_update_request_timer_cb()
{
    app_easy_gap_param_update_start(app_connection_idx);
    app_param_update_request_timer_used = EASY_TIMER_INVALID_TIMER;
}

void user_app_init(void)
{
    app_param_update_request_timer_used = EASY_TIMER_INVALID_TIMER;
    
    // Initialize Manufacturer Specific Data
    mnf_data_init();

    default_app_on_init();
}

/**
 * @brief Add an AD structure in the Advertising or Scan Response Data of the GAPM_START_ADVERTISE_CMD parameter struct.
 * @param[in] cmd               GAPM_START_ADVERTISE_CMD parameter struct
 * @param[in] ad_struct_data    AD structure buffer
 * @param[in] ad_struct_len     AD structure length
 * @return void
 */
static void app_add_ad_struct(struct gapm_start_advertise_cmd *cmd, void *ad_struct_data, uint8_t ad_struct_len)
{
    if ( (APP_ADV_DATA_MAX_SIZE - cmd->info.host.adv_data_len) >= ad_struct_len)
    {
        // Copy data
        memcpy(&cmd->info.host.adv_data[cmd->info.host.adv_data_len], ad_struct_data, ad_struct_len);

        // Update Advertising Data Length
        cmd->info.host.adv_data_len += ad_struct_len;
    }
    else if ( (APP_SCAN_RESP_DATA_MAX_SIZE - cmd->info.host.scan_rsp_data_len) >= ad_struct_len)
    {
        // Copy data
        memcpy(&cmd->info.host.scan_rsp_data[cmd->info.host.scan_rsp_data_len], ad_struct_data, ad_struct_len);

        // Update Scan Responce Data Length
        cmd->info.host.scan_rsp_data_len += ad_struct_len;
    }
    else
    {
        // Manufacturer Specific Data do not fit in either Advertising Data or Scan Response Data
        ASSERT_ERROR(0);
    }
}

void user_app_adv_start(void)
{
    // Schedule the next advertising data update
    app_adv_data_update_timer_used = app_easy_timer(APP_ADV_DATA_UPDATE_TO, adv_data_update_timer_cb);

    struct gapm_start_advertise_cmd* cmd;
    cmd = app_easy_gap_undirected_advertise_get_active();

    // add manufacturer specific data dynamically
    mnf_data_update();
    app_add_ad_struct(cmd, &mnf_data, sizeof(struct mnf_specific_data_ad_structure));

    app_easy_gap_undirected_advertise_start();
}

void user_app_connection(uint8_t connection_idx, struct gapc_connection_req_ind const *param)
{
    if (app_env[connection_idx].conidx != GAP_INVALID_CONIDX)
    {
        app_connection_idx = connection_idx;

        // Stop the advertising data update timer
        app_easy_timer_cancel(app_adv_data_update_timer_used);

        // Check if the parameters of the established connection are the preferred ones.
        // If not then schedule a connection parameter update request.
        if ((param->con_interval < user_connection_param_conf.intv_min) ||
            (param->con_interval > user_connection_param_conf.intv_max) ||
            (param->con_latency != user_connection_param_conf.latency) ||
            (param->sup_to != user_connection_param_conf.time_out))
        {
            // Connection params are not these that we expect
            app_param_update_request_timer_used = app_easy_timer(APP_PARAM_UPDATE_REQUEST_TO, param_update_request_timer_cb);
        }
    }
    else
    {
        // No connection has been established, restart advertising
        user_app_adv_start();
    }

    default_app_on_connection(connection_idx, param);
    
    //setPwm2Duty(0); //RED LED ON
    connection_build_user_define_cb();
}

void user_app_adv_undirect_complete(uint8_t status)
{
    // If advertising was canceled then update advertising data and start advertising again
    if (status == GAP_ERR_CANCELED)
    {
        user_app_adv_start();
    }
}

void user_app_disconnect(struct gapc_disconnect_ind const *param)
{
    // Cancel the parameter update request timer
    if (app_param_update_request_timer_used != EASY_TIMER_INVALID_TIMER)
    {
        app_easy_timer_cancel(app_param_update_request_timer_used);
        app_param_update_request_timer_used = EASY_TIMER_INVALID_TIMER;
    }
    
    uint8_t state = ke_state_get(TASK_APP);

    if ((state == APP_SECURITY) ||
        (state == APP_CONNECTED) ||
        (state == APP_PARAM_UPD))
    {
        // Restart Advertising
        user_app_adv_start();
    }
    else
    {
        // We are not in a Connected State
        ASSERT_ERR(0);
    }
    
    //setPwm2Duty(100); // RED LED OFF
    disconnection_occur_user_define_sb();
}

void user_catch_rest_hndl(ke_msg_id_t const msgid,
                        void const *param,
                        ke_task_id_t const dest_id,
                        ke_task_id_t const src_id)
{
    switch(msgid)
    {
        case CUSTS1_VAL_WRITE_IND:
        {
            struct custs1_val_write_ind const *msg_param = (struct custs1_val_write_ind const *)(param);

            switch (msg_param->handle)
            {
                case CUST1_IDX_BBPG_CONTROL_VAL:
                    bbpg_control_wr_ind_handler(msgid, msg_param, dest_id, src_id);
                    break;
                
                case CUST1_IDX_BBPG_LOSS_CHECK_VAL:
                    bbpg_loss_check_wr_ind_handler(msgid, msg_param, dest_id, src_id);
                    break;

                default:
                    break;
            }
        } break;
              
        case GAPC_PARAM_UPDATED_IND:
        {
            // Cast the void pointer to the appropriate message structure
            struct gapc_param_updated_ind const *msg_param = (struct gapc_param_updated_ind const *)(param);

            // Check if updated Conn Params filled to preffered ones
            if ((msg_param->con_interval >= user_connection_param_conf.intv_min) &&
                (msg_param->con_interval <= user_connection_param_conf.intv_max) &&
                (msg_param->con_latency == user_connection_param_conf.latency) &&
                (msg_param->sup_to == user_connection_param_conf.time_out))
            {
            }
        } break;

        default:
            break;
    }
}

/*
 ***** BBPG *****
 */

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
                    
    oledDrawClear();
    oledDrawAsciiChar(2 ,4,((uint8_t)(time_ptr->tm_hour/10)+0x30));
    oledDrawAsciiChar(14,4,((uint8_t)(time_ptr->tm_hour%10)+0x30));
    oledDrawAsciiChar(26,4,0x3a);
    oledDrawAsciiChar(38,4,((uint8_t)(time_ptr->tm_min/10)+0x30));
    oledDrawAsciiChar(50,4,((uint8_t)(time_ptr->tm_min%10)+0x30));
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


// TODO: UI frame list check initialization
static UI_FRAME_CLASS* ui_frame_list[BBPG_UI_MAX_FRAME] = {0};

void uiRegisterFrame(UI_FRAME_CLASS* tarFrame)
{
    ui_frame_list[tarFrame->priority] = tarFrame;
}

void uiUnregisterFrame(uint8_t frameNum)
{
    ui_frame_list[frameNum] = NULL;
}
 
void ui_refresh_timer_cb_handler(void)
{
    UI_FRAME_CLASS* frame_ptr_temp;
    static uint8_t frame_count = 0;    
    uint8_t frame_empty_check_count;
    
    switch(BBPG_UI_STATE)
    {
        case(BBPG_UI_LAUNCH):
            uiRegisterFrame(&frame_waitConnect_1);
            uiRegisterFrame(&frame_waitConnect_2);

            ui_refresh_timer = app_easy_timer(50/*0.5s*/, ui_refresh_timer_cb_handler);
            
            // refister first two frame
            // wait for touch trigger ui start fresh frame of 0.5ms frequency
            break;   

        case(BBPG_UI_WORK):
            
            frame_ptr_temp = ui_frame_list[frame_count]; // for bug that the undo alarm cancel and display incorrectly ##DEBUG.1702191936 
            if(ui_frame_list[frame_count] != NULL)
            {   
                //ui_frame_list[frame_count]->ui_frame_cb(); ##DEBUG.1702191936 
                //ui_refresh_timer = app_easy_timer(ui_frame_list[frame_count]->showTime, ui_refresh_timer_cb_handler); ##DEBUG.1702191936 
                frame_ptr_temp->ui_frame_cb();
                ui_refresh_timer = app_easy_timer(frame_ptr_temp->showTime, ui_refresh_timer_cb_handler);
                
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
                        ui_refresh_timer = app_easy_timer(frame_ptr_temp->showTime, ui_refresh_timer_cb_handler);
                        
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
                            uiRegisterFrame(&frame_list_empty_show);
                            
                            ui_refresh_timer = app_easy_timer(1, ui_refresh_timer_cb_handler);
                            
                            break;
                        }
                    }
                }
            }
            break;
            
        case(BBPG_UI_SLEEP):
            oledEnterSleep();
            
            uiUnregisterFrame(0);
            uiUnregisterFrame(1);
            //uiUnregisterFrame(4);
            uiUnregisterFrame(5);
            uiUnregisterFrame(6);
        
            BBPG_STATE = BBPG_HAVE_CONNECTION_OLED_SLEEP; //
            break;
        
        case(BBPG_UI_WAKE_UP):
            //BBPG_UI_STATE = BBPG_UI_SLEEP;
            BBPG_UI_STATE = BBPG_UI_WORK;
            uiRegisterFrame(&frame_wake_up_time_show); 
            oledExitingSleep();
        
            ui_refresh_timer = app_easy_timer(1, ui_refresh_timer_cb_handler); 
            break;
        
        default:
            break;
     }
 }

void undo_detect_timer_cb_handler(void)
{  
    static uint8_t ledCount;
    
    switch(BBPG_UNDO_STATE)
    {   
        case(BBPG_UNDO_DETECT_NO_WORK):
            if(isUndoDetectAlarm())
            {
                BBPG_UNDO_STATE = BBPG_UNDO_DETECT_ALARM;
                setUndoDetectTheConnection();
                undo_detect_timer = app_easy_timer(50/*0.5s*/, undo_detect_timer_cb_handler);

                uiRegisterFrame(&frame_undo_alarm_1);
                uiRegisterFrame(&frame_undo_alarm_2);
            }
            else
            {
                BBPG_UNDO_STATE = BBPG_UNDO_DETECT_SAFE;
                setUndoDetectTheDisconnection();
                
            }
            break;
            
        //case(BBPG_UNDO_DETECT_SAFE):
        case(BBPG_UNDO_DETECT_ALARM):
            
            if(BBPG_OPTION.UNDO_ALARM_ENABLE)
            {
                if((BBPG_STATE != BBPG_NO_CONNECTION) && (BBPG_STATE != BBPG_IDENTIFY))
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
            
            undo_detect_timer = app_easy_timer(100/*1s*/, undo_detect_timer_cb_handler);

            break;

        default:
            break;
    }
}

void identify_timer_cb_handler(void)
{
    switch(BBPG_STATE)
    {
        case(BBPG_IDENTIFY):
            BBPG_STATE = BBPG_NO_CONNECTION;
            
            uiRegisterFrame(&frame_identify_fail_1);
            uiRegisterFrame(&frame_identify_fail_2);
        
            app_disconnect();
            //app_easy_timer_cancel(identify_timer); can not cancel itself that execute in its own body??
            break;
        default:
            break;
    }
}

void timestamp_tictok_timer_cb_handler(void)
{
    UNIX_TIMESTAMP++;
    
    timer_counter_timer = app_easy_timer(100/*1s*/, timestamp_tictok_timer_cb_handler); // never stop
}

void accel_refresh_timer_cb_handler(void)
{
    //static uint16_t mpuFIFOCount;
    
    static int16_t accelRaw[3];
    
    //static uint8_t fifoTemp[48]; 
    
    switch(BBPG_ACCEL_STATE)
    {
        case(BBPG_ACCEL_METER_WORK):
            //mpuFIFOCount = readMPUFIFOCount();
            //readMPUFIFO(fifoTemp, 48);
        
            readMPU9250AccelRaw(accelRaw);
        
            accel_refresh_timer = app_easy_timer(100/*1s, sleep detect, and destory detect*/, accel_refresh_timer_cb_handler);
            break;
        
        default:
            break;
    }
}

void start_advertise_user_define_cb(void)
{
    static uint8_t isTimestampTimerInit = 0;
    
    if(!isTimestampTimerInit)
    {
        isTimestampTimerInit = 1;
        
        timer_counter_timer = app_easy_timer(100/*1s*/, timestamp_tictok_timer_cb_handler);
    }
    
    switch(BBPG_ACCEL_STATE)
    {
        case(BBPG_ACCEL_METER_UNINIT):
            BBPG_ACCEL_STATE = BBPG_ACCEL_METER_WORK;
        
            accel_refresh_timer = app_easy_timer(1, accel_refresh_timer_cb_handler);
            break;
        
        case(BBPG_ACCEL_METER_WORK):
            break;
    
        default:
            break;
    }
            
    
    switch(BBPG_STATE)
    {
        case(BBPG_NO_CONNECTION):
            switch(BBPG_UI_STATE)
            {
                case(BBPG_UI_LAUNCH):
                    ui_refresh_timer = app_easy_timer(1, ui_refresh_timer_cb_handler);
                    // launch ui
                    break;
                default:
                    break;
            }
            
            break;
        default:
            break;
    }
    
    switch(BBPG_UNDO_STATE)
    {
        case(BBPG_UNDO_DETECT_NO_WORK):
            undo_detect_timer = app_easy_timer(1, undo_detect_timer_cb_handler);
            // launch undo detect
            break;
        default:
            break;
    }
    
    switch(BBPG_BAT_STATE)
    {
        case(BBPG_BAT_UNINIT):
            battery_monitor_timer = app_easy_timer(1, battery_state_update_timer_cb_handler);
            break;
        default:
            break;
    }
}

void battery_state_update_timer_cb_handler(void)
{
    float bat_adc_sample;
    
    bat_adc_sample = adcGetP00VbatVal() *BBPG_BAT_CAL_K;
    
    if( bat_adc_sample > BBPG_BAT_WORRY_VAL )
    {
        BBPG_BAT_STATE = BBPG_BAT_OK;
        
        uiUnregisterFrame(4);
    }
    else if((BBPG_BAT_WORRY_VAL >= bat_adc_sample) && (bat_adc_sample> BBPG_BAT_OMG_VAL))
    {
        BBPG_BAT_STATE = BBPG_BAT_WORRY;
        
        uiUnregisterFrame(4);
    }
    else if( BBPG_BAT_OMG_VAL >= bat_adc_sample)
    {
        BBPG_BAT_STATE = BBPG_BAT_OMG;
        
        uiRegisterFrame(&frame_battery_alarm);
    }
        
    battery_monitor_timer = app_easy_timer(1000/*10s*/, battery_state_update_timer_cb_handler);
}

void connection_build_user_define_cb(void)
{
    switch(BBPG_UI_STATE)
    {
        case(BBPG_UI_LAUNCH):
            BBPG_UI_STATE = BBPG_UI_WORK;
            uiRegisterFrame(&frame_identify_1);
            uiRegisterFrame(&frame_identify_2);
            break;

        case(BBPG_UI_WORK):
            uiRegisterFrame(&frame_identify_1);
            uiRegisterFrame(&frame_identify_2);
            break;

        default:
            break;
    }
    BBPG_STATE = BBPG_IDENTIFY;
    identify_timer = app_easy_timer(BBPG_IDENTIFY_TIME, identify_timer_cb_handler);
}

void disconnection_occur_user_define_sb(void)
{
    switch(BBPG_STATE)
    {
        case(BBPG_HAVE_CONNECTION_OLED_SLEEP):
            oledExitingSleep();
            BBPG_STATE = BBPG_NO_CONNECTION;
            switch(BBPG_UI_STATE)
            {
                case(BBPG_UI_SLEEP):    
                    BBPG_UI_STATE = BBPG_UI_WORK;
                    uiRegisterFrame(&frame_connect_loss_1);
                    uiRegisterFrame(&frame_connect_loss_2);
                    
                    uiUnregisterFrame(5); // TODO: for disconnect but display time

                    ui_refresh_timer = app_easy_timer(1, ui_refresh_timer_cb_handler);  // oled wake up first call
                    break;
                
                default:
                    break;
            }
            break;
        
        case(BBPG_HAVE_CONNECTION):
            BBPG_STATE = BBPG_NO_CONNECTION;
            switch(BBPG_UI_STATE)
            {
                case(BBPG_UI_WORK):
                    uiRegisterFrame(&frame_connect_loss_1);
                    uiRegisterFrame(&frame_connect_loss_2);
                    
                    uiUnregisterFrame(5); // TODO: for disconnect but display time
                    break;

                default:
                    break;
            }
            break;
        
        case(BBPG_IDENTIFY):
            BBPG_STATE = BBPG_NO_CONNECTION;
            switch(BBPG_UI_STATE)
            {
                case(BBPG_UI_WORK):
                    uiRegisterFrame(&frame_connect_loss_1);
                    uiRegisterFrame(&frame_connect_loss_2);
                
                    uiUnregisterFrame(5); // TODO: maybe this also necessary
                    break;

                default:
                    break;
            }
            break;

        default:
            break;
    }
}

void identifySuccessUiUpdateCallBack(void)
{
    uiRegisterFrame(&frame_identify_success_1);
    uiRegisterFrame(&frame_identify_success_2);
}

void undoDetectUiUpdateCallBack(bool isCancel)
{
    if(isCancel)
    {
        uiUnregisterFrame(2);
        uiUnregisterFrame(3);
    }
    else
    {
        switch(BBPG_UI_STATE)
        {
            case(BBPG_UI_SLEEP):
                BBPG_UI_STATE = BBPG_UI_WORK;
            
                uiRegisterFrame(&frame_undo_alarm_1);
                uiRegisterFrame(&frame_undo_alarm_2);
                oledExitingSleep();    
            
                BBPG_STATE = BBPG_HAVE_CONNECTION;
            
                ui_refresh_timer = app_easy_timer(1, ui_refresh_timer_cb_handler);   
                break;
            
            case(BBPG_UI_WORK):
                uiRegisterFrame(&frame_undo_alarm_1);
                uiRegisterFrame(&frame_undo_alarm_2);
                break;
            
            default:
                break;
        }
    }
}


/// @} APP
