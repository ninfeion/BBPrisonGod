#ifndef _USER_CUSTS1_IMPL_H_
#define _USER_CUSTS1_IMPL_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "gapc_task.h"                 // gap functions and messages
#include "gapm_task.h"                 // gap functions and messages
#include "custs1_task.h"

void bbpg_control_wr_ind_handler(ke_msg_id_t const msgid,
                                 struct custs1_val_write_ind const *param,
                                 ke_task_id_t const dest_id,
                                 ke_task_id_t const src_id);

void bbpg_loss_check_wr_ind_handler(ke_msg_id_t const msgid,
                                 struct custs1_val_write_ind const *param,
                                 ke_task_id_t const dest_id,
                                 ke_task_id_t const src_id);
                                 
void undo_detect_irq_handler(void);                                 
void touch_detect_irq_handler(void);
     
                                 
#endif

