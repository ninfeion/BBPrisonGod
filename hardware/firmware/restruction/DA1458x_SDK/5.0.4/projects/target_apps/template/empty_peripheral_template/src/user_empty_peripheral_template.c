/**
 ****************************************************************************************
 *
 * @file user_empty_peripheral_template.c
 *
 * @brief Empty peripheral template project source code.
 *
 * Copyright (C) 2012. Dialog Semiconductor Ltd, unpublished work. This computer 
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
#include "app_api.h"
#include "user_empty_peripheral_template.h"
#include "user_custs1_impl.h"
#include "arch_api.h"
#include "user_custs1_def.h"
#include "gap.h"

#include "bbpg_event_schedule.h"

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
*/

void user_on_connection(uint8_t connection_idx, struct gapc_connection_req_ind const *param)
{
    default_app_on_connection(connection_idx, param);
    
    /**
     * connecting complete, send message to queue
     * BBPG
     */    
    BBPGEventInputSend(BBPG_EVENT_INPUT_BLE_CONNECT_COMPLETE);
}

void user_on_disconnect( struct gapc_disconnect_ind const *param )
{
    default_app_on_disconnect(param);
    
    /**
     * disconnect, send message to queue
     * BBPG
     */    
    BBPGEventInputSend(BBPG_EVENT_INPUT_BLE_DISCONNECT);
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
    }
}



/// @} APP
