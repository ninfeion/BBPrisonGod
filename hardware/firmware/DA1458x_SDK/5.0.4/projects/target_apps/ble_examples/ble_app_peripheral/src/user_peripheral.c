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

#include "bbpg_alarm.h"
#include "oled_draw.h"

#include "bbpg_user_setup.h"

#include "adc.h"

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
                    break;

                default:
                    break;
            }
        } break;
        
        /*
        case CUSTS1_VAL_NTF_CFM:
        {
            struct custs1_val_ntf_cfm const *msg_param = (struct custs1_val_ntf_cfm const *)(param);

            switch (msg_param->handle)
            {
                case CUST1_IDX_ADC_VAL_1_VAL:
                    break;

                case CUST1_IDX_BUTTON_STATE_VAL:
                    break;

                case CUST1_IDX_LONG_VALUE_VAL:
                    break;

                default:
                    break;
            }
        } break;

        case CUSTS1_VAL_IND_CFM:
        {
            struct custs1_val_ind_cfm const *msg_param = (struct custs1_val_ind_cfm const *)(param);

            switch (msg_param->handle)
            {
                case CUST1_IDX_INDICATEABLE_VAL:
                    break;

                default:
                    break;
             }
         } break;
        */
        
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

void undo_detect_timer_cb_handler(void)
{
    switch(BBPG_UNDO_STATE)
    {
        case(BBPG_UNDO_DETECT_NO_WORK):
            if(isUndoDetectAlarm())
            {
                BBPG_UNDO_STATE = BBPG_UNDO_DETECT_ALARM;
                setUndoDetectTheConnection();
                undo_detect_timer = app_easy_timer(50/*0.5s*/, undo_detect_timer_cb_handler);
            }
            else
            {
                BBPG_UNDO_STATE = BBPG_UNDO_DETECT_SAFE;
                setUndoDetectTheDisconnection();
            }
            break;
        //case(BBPG_UNDO_DETECT_SAFE):
        case(BBPG_UNDO_DETECT_ALARM):
            struct custs1_val_ntf_req* req = KE_MSG_ALLOC_DYN(CUSTS1_VAL_NTF_REQ,
                                                              TASK_CUSTS1,
                                                              TASK_APP,
                                                              custs1_val_ntf_req,
                                                              DEF_CUST1_BBPG_BREAK_CHECK_CHAR_LEN);
            uint8_t breakNotify[32];
            static uint8_t ledCount;

            breakNotify[0] = 0x01;

            req->conhdl = app_env->conhdl;
            req->handle = CUST1_IDX_BBPG_BREAK_CHECK_VAL;
            req->length = DEF_CUST1_BBPG_BREAK_CHECK_CHAR_LEN;
            
            memcpy(req->value, &breakNotify, DEF_CUST1_BBPG_BREAK_CHECK_CHAR_LEN);

            if((BBPG_STATE != BBPG_NO_CONNECTION) && (BBPG_STATE != BBPG_IDENTIFY))
            {
                ke_msg_send(req);
            }

            undo_detect_timer = app_easy_timer(100/*1s*/, undo_detect_timer_cb_handler);

            if(ledCount)
            {
                ledCount = ~ledCount;

                setPwm2Duty(0); // flash the led
                setPwm4Duty(100); // disable the sounder
            }
            else
            {
                ledCount = ~ledCount;

                setPwm2Duty(100);
                setPwm4Duty(100);                 
            }

            break;

        default:
            break;
    }
}

void ui_refresh_timer_cb_handler(void)
{
    switch(BBPG_UI_STATE)
    {
        case(BBPG_UI_LAUNCH):
            ui_refresh_timer = app_easy_timer(50/*0.5ms*/, ui_refresh_timer_cb_handler);
            // wait for touch trigger wait connect ui of 0.5ms frequency
            break;   
        
        case(BBPG_UI_WAIT_CONNECT_1):
            BBPG_UI_STATE = BBPG_UI_WAIT_CONNECT_2;
            oledDrawClear();
            oledDrawCnChar(0, 8, "等"); 
            oledDrawCnChar(16,8, "待");
            oledDrawCnChar(32,8, "配");
            oledDrawCnChar(48,8, "对");
            ui_refresh_timer = app_easy_timer(200, ui_refresh_timer_cb_handler);
            break;
        
        case(BBPG_UI_WAIT_CONNECT_2):
            BBPG_UI_STATE = BBPG_UI_WAIT_CONNECT_1;
            //oledDrawClear();
            oledDrawPicture(WAIT_CONNECT_PIC);
            ui_refresh_timer = app_easy_timer(200, ui_refresh_timer_cb_handler);
            break;
        
        case(BBPG_UI_IDENTIFY_1):
            BBPG_UI_STATE = BBPG_UI_IDENTIFY_2;
            oledDrawPicture(INDENTIFY_1_PIC);
            ui_refresh_timer = app_easy_timer(200, ui_refresh_timer_cb_handler);
            break;
        
        case(BBPG_UI_IDENTIFY_2):
            BBPG_UI_STATE = BBPG_UI_IDENTIFY_1;
            oledDrawPicture(INDENTIFY_2_PIC);
            ui_refresh_timer = app_easy_timer(200, ui_refresh_timer_cb_handler);
            break;
        
        case(BBPG_UI_IDENTIFY_FAIL):
            BBPG_UI_STATE = BBPG_UI_WAIT_CONNECT_1;
            oledDrawClear();
            oledDrawCnChar(0, 8, "设"); 
            oledDrawCnChar(16,8, "备");
            oledDrawCnChar(32,8, "异");
            oledDrawCnChar(48,8, "常");
            ui_refresh_timer = app_easy_timer(500, ui_refresh_timer_cb_handler);
            break;
        
        case(BBPG_UI_IDENTIFY_SUCCESS):
            BBPG_UI_STATE = BBPG_UI_CONNECT_OK_1;
            oledDrawClear();
            oledDrawCnChar(0, 8, "配"); 
            oledDrawCnChar(16,8, "对");
            oledDrawCnChar(32,8, "成");
            oledDrawCnChar(48,8, "功");
            ui_refresh_timer = app_easy_timer(200, ui_refresh_timer_cb_handler);
            break;
        
        case(BBPG_UI_CONNECT_OK_1):
            BBPG_UI_STATE = BBPG_UI_IDENTIFY_SUCCESS;
            oledDrawPicture(CONNECT_OK_PIC);
            ui_refresh_timer = app_easy_timer(200, ui_refresh_timer_cb_handler);
            break;
        
        case(BBPG_UI_CONNECT_LOSS_1):
            BBPG_UI_STATE = BBPG_UI_CONNECT_LOSS_2;
            oledDrawClear();
            oledDrawCnChar(0, 8, "配"); 
            oledDrawCnChar(16,8, "对");
            oledDrawCnChar(32,8, "丢");
            oledDrawCnChar(48,8, "失");
            ui_refresh_timer = app_easy_timer(200, ui_refresh_timer_cb_handler);    
            break;
        
        case(BBPG_UI_CONNECT_LOSS_2):
            BBPG_UI_STATE = BBPG_UI_CONNECT_LOSS_1;
            oledDrawPicture(CONNECT_LOSS_PIC);
            ui_refresh_timer = app_easy_timer(200, ui_refresh_timer_cb_handler);    
            break;         
        
        case(BBPG_UI_SLEEP):
            oledEnterSleep();
            break;
        
        case(BBPG_UI_WAKE_UP):
            BBPG_UI_STATE = BBPG_UI_SLEEP;
            BBPG_STATE = BBPG_HAVE_CONNECTION_OLED_SLEEP; //
            
            oledExitingSleep();
            oledDrawClear();
            oledDrawAsciiChar(2,4,0x31);
            oledDrawAsciiChar(14,4,0x39);
            oledDrawAsciiChar(26,4,0x3a);
            oledDrawAsciiChar(38,4,0x35);
            oledDrawAsciiChar(50,4,0x37);
            ui_refresh_timer = app_easy_timer(1000, ui_refresh_timer_cb_handler); 
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
            
            switch(BBPG_UI_STATE)
            {
                case(BBPG_UI_IDENTIFY_1):
                case(BBPG_UI_IDENTIFY_2):
                    BBPG_UI_STATE = BBPG_UI_IDENTIFY_FAIL;
                    break;
                default:
                    break;
            }
        
            app_disconnect();
            //app_easy_timer_cancel(identify_timer); can not cancel itself that execute in its own body??
            break;
        default:
            break;
    }
}
 
void start_advertise_user_define_cb(void)
{
    switch(BBPG_STATE)
    {
        case(BBPG_NO_CONNECTION):
            switch(BBPG_UI_STATE)
            {
                case(BBPG_UI_LAUNCH):
                    ui_refresh_timer = app_easy_timer(50/*0.5s*/, ui_refresh_timer_cb_handler);
                    // wait for touch trigger wait connect ui of 0.5ms frequency
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
            undo_detect_timer = app_easy_timer(50/*0.5s*/, undo_detect_timer_cb_handler);
            break;
        default:
            break;
    }
}

void connection_build_user_define_cb(void)
{
    uint32_t sample;
    sample = adcGetP00VbatVal();
    switch(BBPG_UI_STATE)
    {
        case(BBPG_UI_CONNECT_LOSS_1):
        case(BBPG_UI_CONNECT_LOSS_2):
        case(BBPG_UI_LAUNCH):        
        case(BBPG_UI_WAIT_CONNECT_1):
        case(BBPG_UI_WAIT_CONNECT_2):
            BBPG_UI_STATE = BBPG_UI_IDENTIFY_1;
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
                    BBPG_UI_STATE = BBPG_UI_CONNECT_LOSS_1;
                     
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
                case(BBPG_UI_IDENTIFY_SUCCESS):
                case(BBPG_UI_CONNECT_OK_1):
                    BBPG_UI_STATE = BBPG_UI_CONNECT_LOSS_1;
                    break;
                
                default:
                    break;
            }
            break;
        
        case(BBPG_IDENTIFY):
            BBPG_STATE = BBPG_NO_CONNECTION;
            switch(BBPG_UI_STATE)
            {
                case(BBPG_UI_IDENTIFY_1):
                case(BBPG_UI_IDENTIFY_2):
                    BBPG_UI_STATE = BBPG_UI_IDENTIFY_FAIL;
                    break;

                default:
                    break;
            }
            break;

        default:
            break;
    }
}

static const uint8_t battery_filter_option=0;

static inline uint16_t battery_filter_value (uint16_t new_adc_value)
{
     switch(battery_filter_option)
     {
         case 0:
                 return(new_adc_value);
         case 1:
                 return(new_adc_value&0xFFF0);
         case 2:
                 return(battery_moving_average_filter(new_adc_value));
         case 3:
                 return(battery_moving_average_with_reaction_filter(new_adc_value));
     }
     return (new_adc_value);
}

uint32_t adcGetP00VbatVal(void)
{
    static uint32_t adc_sample, adc_sample2;
    
    adc_calibrate();

    adc_init(GP_ADC_SE, GP_ADC_SIGN, GP_ADC_ATTN3X);
    adc_usDelay(20);

    adc_enable_channel(ADC_CHANNEL_P00);

    adc_sample = adc_get_sample();
    adc_usDelay(1);
    adc_init(GP_ADC_SE, 0, GP_ADC_ATTN3X );
    
    adc_enable_channel(ADC_CHANNEL_P00);

    adc_sample2 = adc_get_sample();
    //We have to divide the following result by 2 if
    //the 10 bit accuracy is enough
    adc_sample = (adc_sample2 + adc_sample);
    adc_disable();

    adc_sample = battery_filter_value(adc_sample);
    
    return adc_sample;
}

/// @} APP
