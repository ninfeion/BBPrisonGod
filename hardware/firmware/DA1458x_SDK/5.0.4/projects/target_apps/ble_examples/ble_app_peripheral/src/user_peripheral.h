/**
 ****************************************************************************************
 *
 * @file user_peripheral_proj.h
 *
 * @brief Peripheral project header file.
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

#ifndef _USER_PERIPHERAL_H_
#define _USER_PERIPHERAL_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwble_config.h"
#include "app_task.h"                  // application task
#include "gapc_task.h"                 // gap functions and messages
#include "gapm_task.h"                 // gap functions and messages
#include "app.h"                       // application definitions
#include "co_error.h"                  // error code definitions
#include "smpc_task.h"                 // error code definitions
#include "arch_wdg.h"

#include "app_callback.h"
#include "app_default_handlers.h"


/*
 * DEFINES
 ****************************************************************************************
 */

/* Duration of timer for connection parameter update request */
#define APP_PARAM_UPDATE_REQUEST_TO         (1000)   // 500*10ms = 5sec, The maximum allowed value is 30000 = 300 sec

/* Advertising data update timer */
#define APP_ADV_DATA_UPDATE_TO              (1000)  // 1000*10ms = 10sec

/* Manufacturer specific data constants */
#define APP_AD_MSD_COMPANY_ID       (0xABCD)
#define APP_AD_MSD_COMPANY_ID_LEN   (2)
#define APP_AD_MSD_DATA_LEN         (sizeof(uint16_t))

#define APP_PERIPHERAL_CTRL_TIMER_DELAY 100

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Application initialization function.
 * @return void
 ****************************************************************************************
*/
void user_app_init(void);

/**
 ****************************************************************************************
 * @brief Advertising function.
 * @return void
 ****************************************************************************************
*/
void user_app_adv_start(void);

/**
 ****************************************************************************************
 * @brief Connection function.
 * @param[in] connection_idx Connection Id index
 * @param[in] param Pointer to GAPC_CONNECTION_REQ_IND message
 * @return void
 ****************************************************************************************
*/
void user_app_connection(uint8_t connection_idx, 
                         struct gapc_connection_req_ind const *param);

/**
 ****************************************************************************************
 * @brief Undirect advertising completion function.
 * @param[in] status Command complete event message status
 * @return void
 ****************************************************************************************
*/
void user_app_adv_undirect_complete(uint8_t status);

/**
 ****************************************************************************************
 * @brief Disconnection function.
 * @param[in] param Pointer to GAPC_DISCONNECT_IND message
 * @return void
 ****************************************************************************************
*/
void user_app_disconnect(struct gapc_disconnect_ind const *param);

/**
 ****************************************************************************************
 * @brief Handles the messages that are not handled by the SDK internal mechanisms.
 * @param[in] msgid   Id of the message received.
 * @param[in] param   Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance.
 * @param[in] src_id  ID of the sending task instance.
 * @return void
 ****************************************************************************************
*/
void user_catch_rest_hndl(ke_msg_id_t const msgid,
                          void const *param,
                          ke_task_id_t const dest_id,
                          ke_task_id_t const src_id);

/// @} APP

/*BBPG*/
#include "bbpg_user_setup.h"                          
                          
void start_advertise_user_define_cb(void);
void connection_build_user_define_cb(void);
void disconnection_occur_user_define_sb(void);                          
 
extern ke_msg_id_t ui_refresh_timer;                          
void ui_refresh_timer_cb_handler(void);       

extern ke_msg_id_t undo_detect_timer;
void undo_detect_timer_cb_handler(void);
                          
void battery_state_update_timer_cb_handler(void);                          
                             
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

void identifySuccessUiUpdateCallBack(void);

void uiRegisterFrame(UI_FRAME_CLASS* tarFrame);
void uiUnregisterFrame(uint8_t frameNum);
void undoDetectUiUpdateCallBack(bool isCancel); 
 
#endif // _USER_PERIPHERAL_H_
