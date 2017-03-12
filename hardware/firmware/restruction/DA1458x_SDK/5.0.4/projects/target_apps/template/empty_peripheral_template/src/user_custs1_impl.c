#include "user_custs1_impl.h"

#include "app_api.h"
#include "app.h"
#include "user_custs1_def.h"

#include "bbpg_user_setup.h"
#include "bbpg_alarm.h"
#include "bbpg_adc.h"
#include "bbpg_event_schedule.h"

uint32_t UNIX_TIMESTAMP = 0;
uint32_t MOTION_TIMESTAMP_MS = 0;
BBPG_UNDO_DETECT_STATE_T BBPG_UNDO_STATE = BBPG_UNDO_DETECT_NO_WORK;

BBPG_OPTION_T BBPG_OPTION =
{
    .UNDO_ALARM_ENABLE = 0,
    .MPU_WORK_ENABLE = 1,
};

void bbpg_control_wr_ind_handler(ke_msg_id_t const msgid,
                                 struct custs1_val_write_ind const *param,
                                 ke_task_id_t const dest_id,
                                 ke_task_id_t const src_id)
{
    uint8_t val[32];
    memcpy(&val, &param->value[0], param->length);
    
    //BBPGControlCommandDecoder(val);
    uint16_t checksum;
    
    // decoder process
    if((val[0]  == BBPG_PHONE2BAND_MESS_HEAD) &&
       (val[1]  == BBPG_PHONE2BAND_MESS_HEAD) &&
       (val[13] == BBPG_PHONE2BAND_MESS_REAR) &&
       (val[14] == BBPG_PHONE2BAND_MESS_REAR))
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
                    if(BBPG_CONNECTION_STATE != BBPG_CONNECTED)
                    {                        
                        BBPGEventInputSend(BBPG_EVENT_INPUT_IDENTIFY_SUCCESS);
                    }
                    break;
                    
                case BBPG_COM_TYPE_UNDO_ALARM_ENABLE:
                    
                    BBPG_OPTION.UNDO_ALARM_ENABLE = val[7];
                    break;
                
                case BBPG_COM_TYPE_JUST_UPDATE_TIME:
                    break;
                
                case BBPG_COM_TYPE_GET_BATTERY_VAL:
                    {
                        struct custs1_val_ntf_req* req;
                        uint8_t return_mess[32];
                        
                        float bat_adc_sample;
                        
                        bat_adc_sample = adcGetP00VbatVal() *BBPG_BAT_CAL_K;     
                        
                        return_mess[0] = BBPG_BAND2PHONE_MESS_HEAD;
                        return_mess[1] = BBPG_BAND2PHONE_MESS_HEAD;
                        
                        return_mess[2] = val[2]; // just for test
                        return_mess[3] = val[3];
                        return_mess[4] = val[4];
                        return_mess[5] = val[5];
                        
                        if((bat_adc_sample-BBPG_BAT_OMG_VAL) >= 0.0)
                        {
                            return_mess[6] = (uint8_t)(((bat_adc_sample-BBPG_BAT_OMG_VAL) /(BBPG_BAT_OK_VAL-BBPG_BAT_OMG_VAL)) *100);
                        }
                        else
                        {
                            return_mess[6] = 0;
                        }
                        
                        return_mess[7] = (uint8_t)(((uint32_t)bat_adc_sample)>>8);
                        return_mess[8] = (uint8_t)(bat_adc_sample);
                       
                        return_mess[9]  = BBPG_BAND2PHONE_MESS_REAR;
                        return_mess[10] = BBPG_BAND2PHONE_MESS_REAR;
                                
                        req = KE_MSG_ALLOC_DYN(CUSTS1_VAL_NTF_REQ,
                                               TASK_CUSTS1,
                                               TASK_APP,
                                               custs1_val_ntf_req,
                                               DEF_CUST1_BBPG_CONTROL_CHAR_LEN);

                        req->conhdl = app_env->conhdl;
                        req->handle = CUST1_IDX_BBPG_CONTROL_VAL;
                        req->length = DEF_CUST1_BBPG_CONTROL_CHAR_LEN;
                           
                        memcpy(req->value, &return_mess, DEF_CUST1_BBPG_CONTROL_CHAR_LEN);
                               
                        ke_msg_send(req);
                    }
                    break;
                
                default:
                    break;
            }
        }
    }
     
    /*
    if(val[0] == 1)
    {
        BBPGEventInputSend(BBPG_EVENT_INPUT_IDENTIFY_SUCCESS);
    }
    */
}

void bbpg_loss_check_wr_ind_handler(ke_msg_id_t const msgid,
                                 struct custs1_val_write_ind const *param,
                                 ke_task_id_t const dest_id,
                                 ke_task_id_t const src_id)
{
}

void undo_detect_irq_handler(void)
{
    switch(BBPG_UNDO_STATE)
    {
        case(BBPG_UNDO_DETECT_SAFE):
            BBPG_UNDO_STATE = BBPG_UNDO_DETECT_ALARM;
            setUndoDetectTheConnection();
    
            BBPGEventInputSend(BBPG_EVENT_INPUT_UNDO_ALARM);
            break;

        case(BBPG_UNDO_DETECT_ALARM):
            BBPG_UNDO_STATE = BBPG_UNDO_DETECT_SAFE;
            setUndoDetectTheDisconnection(); 
            
            BBPGEventInputSend(BBPG_EVENT_INPUT_UNDO_SAFE);
            break;
    }    
}

static ke_msg_id_t touch_interrupt_enable_timer = 0xFFFF;

void touch_interrupt_enable_timer_cb_handler(void)
{
    GPIO_EnableIRQ(TOUCH_IO_PORT, TOUCH_IO_PIN, GPIO2_IRQn, false, true, 1);
    
    touch_interrupt_enable_timer = 0xFFFF;
}

void touch_detect_irq_handler(void)
{
    NVIC_DisableIRQ(GPIO2_IRQn); // DISABLE TOUCH DETECT INTERRUPT
            
    BBPGEventInputSend(BBPG_EVENT_INPUT_TOUCH_DETECT);
    
    touch_interrupt_enable_timer = app_easy_timer(100, touch_interrupt_enable_timer_cb_handler); // after 1s enable interrupt
}


