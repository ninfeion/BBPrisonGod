#include "bbpg_command_decoder.h"
#include "bbpg_event_schedule.h"
#include "bbpg_user_setup.h"

#include "app_api.h"
#include "app.h"
#include "custs1_task.h"
#include "user_custs1_def.h"

void BBPGControlCommandDecoder(uint8_t* command)
{
    uint16_t checksum;
    
    // decoder process
    if((command[0]  == BBPG_PHONE2BAND_MESS_HEAD) &&
       (command[1]  == BBPG_PHONE2BAND_MESS_HEAD) &&
       (command[13] == BBPG_PHONE2BAND_MESS_REAR) &&
       (command[14] == BBPG_PHONE2BAND_MESS_REAR))
    {
        checksum = (((command[2] + command[3] + command[4] + command[5] + command[6] + command[7] + 
                      command[8] + command[9] + command[10] + command[11]) %26) +97);
        if(((uint8_t)checksum) == command[12])
        {
            // correct timestamp
            UNIX_TIMESTAMP = (uint32_t)(command[2]<<24 | command[3]<<16 | command[4]<<8 | command[5]) + BBPG_TIMEZONE_SET(8);
            
            switch(command[6]) // case command type
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
                    
                    BBPG_OPTION.UNDO_ALARM_ENABLE = command[7];
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
                        
                        //return_mess[2] = (uint8_t)(UNIX_TIMESTAMP-BBPG_TIMEZONE_SET(8)>>24); // just for test, because the before code had updated timestamp
                        //return_mess[3] = (uint8_t)(UNIX_TIMESTAMP-BBPG_TIMEZONE_SET(8)>>16);
                        //return_mess[4] = (uint8_t)(UNIX_TIMESTAMP-BBPG_TIMEZONE_SET(8)>>8);
                        //return_mess[5] = (uint8_t)(UNIX_TIMESTAMP-BBPG_TIMEZONE_SET(8));
                        return_mess[2] = command[2]; // just for test
                        return_mess[3] = command[3];
                        return_mess[4] = command[4];
                        return_mess[5] = command[5];
                        
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
     
    
    if(command[0] == 1)
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



