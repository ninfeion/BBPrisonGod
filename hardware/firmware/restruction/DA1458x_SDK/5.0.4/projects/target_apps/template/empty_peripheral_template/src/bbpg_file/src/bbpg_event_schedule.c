#include "bbpg_event_schedule.h"
#include "bbpg_interaction.h"

#include "bbpg_alarm.h"
#include "bbpg_interaction.h" // include global variable "BBPG_OLED_DISPLAY_MODE"

//static BBPG_IDENTIFY_STATE_T    BBPG_IDENTIFY_STATE = BBPG_IDENTIFY_NO;
static BBPG_OLED_SLEEP_STATE_T  BBPG_OLED_SLEEP_STATE = BBPG_WAKE_UP;
BBPG_CONNECTION_STATE_T         BBPG_CONNECTION_STATE = BBPG_LAUNCH;   

void BBPGEventInputSend(BBPG_EVENT_INPUT_TYPE_T event)
{
    switch(event)
    {
        case BBPG_EVENT_INPUT_BLE_CONNECT_COMPLETE:
            {
                if(BBPG_CONNECTION_STATE == BBPG_LAUNCH)
                {
                    BBPG_CONNECTION_STATE = BBPG_WAIT_CONNECT;
                    
                    BBPGInteractInputSend(BBPG_INTERACT_WAIT_IDENTIFY_DIS);
                }
                else
                {
                    BBPG_CONNECTION_STATE = BBPG_WAIT_CONNECT;
                    
                    BBPGInteractInputSend(BBPG_INTERACT_WAIT_IDENTIFY_DIS);
                }
            }
            break;
            
        case BBPG_EVENT_INPUT_BLE_DISCONNECT:
            {
                if(BBPG_OLED_SLEEP_STATE == BBPG_SLEEP)
                {
                    BBPG_OLED_SLEEP_STATE = BBPG_WAKE_UP;
                    
                    BBPGInteractInputSend(BBPG_INTERACT_OLED_WAKEUP_LOOP_DIS);
                }
                
                if(BBPG_CONNECTION_STATE == BBPG_WAIT_CONNECT)
                {
                    BBPGInteractInputSend(BBPG_INTERACT_IDENTIFY_FAIL);
                }
                else
                {
                    BBPGInteractInputSend(BBPG_INTERACT_DISCONNECT_DIS);
                }
                
                if(BBPG_OLED_DISPLAY_MODE == BBPG_OLED_ORDER_DISPLAY)
                {
                    BBPG_OLED_DISPLAY_MODE = BBPG_OLED_LOOP_DISPLAY;
                    
                    BBPGInteractInputSend(BBPG_INTERACT_DISCONNECT_DIS);
                    
                    BBPGInteractInputSend(BBPG_INTERACT_OLED_ORDER_TRANS_LOOP);
                }
                
                BBPG_CONNECTION_STATE = BBPG_DISCONNECTED;
            }
            break;
            
        case BBPG_EVENT_INPUT_INIT_OVER_START_ADV:
            {
                // UNDO DETECT INIT
                switch(BBPG_UNDO_STATE)
                {   
                    case(BBPG_UNDO_DETECT_NO_WORK):
                        // will only execute once
                        if(isUndoDetectAlarm())
                        {
                            BBPG_UNDO_STATE = BBPG_UNDO_DETECT_ALARM;
                            setUndoDetectTheConnection();
                            
                            BBPGInteractInputSend(BBPG_INTERACT_UNDO_ALARM);
                        }
                        else
                        {
                            BBPG_UNDO_STATE = BBPG_UNDO_DETECT_SAFE;
                            setUndoDetectTheDisconnection();
                        }
                        break;
                        
                    default:
                        break;
                }
                
                // BATTERY ADC INIT
                BBPGInteractInputSend(BBPG_INTERACT_BATTERY_MONITOR_INIT);
                
                // TIME TIKTOC INIT
                BBPGInteractInputSend(BBPG_INTERACT_TIME_TIKTOC_INIT);
                
                // STEP COUNT INIT
                //BBPGInteractINputSend(BBPG_INTERACT_ACCEL_TIMER_INIT);
            }
            break;
            
        case BBPG_EVENT_INPUT_IDENTIFY_SUCCESS:
            {
                if(BBPG_CONNECTION_STATE == BBPG_WAIT_CONNECT)
                {
                    BBPG_CONNECTION_STATE = BBPG_CONNECTED;
                
                    BBPGInteractInputSend(BBPG_INTERACT_CONN_OK_DIS);
                }
            }
            break;
            
        case BBPG_EVENT_INPUT_UNDO_ALARM:
            {
                BBPGInteractInputSend(BBPG_INTERACT_UNDO_ALARM);
                
                if(BBPG_OLED_SLEEP_STATE == BBPG_SLEEP)
                {
                    BBPG_OLED_SLEEP_STATE = BBPG_WAKE_UP;
                    
                    BBPGInteractInputSend(BBPG_INTERACT_OLED_WAKEUP_LOOP_DIS);
                    
                    break;
                }     

                if(BBPG_OLED_SLEEP_STATE == BBPG_WAKE_UP)
                {
                    if(BBPG_OLED_DISPLAY_MODE == BBPG_OLED_ORDER_DISPLAY)
                    {
                        BBPG_OLED_DISPLAY_MODE = BBPG_OLED_LOOP_DISPLAY;
                        
                        BBPGInteractInputSend(BBPG_INTERACT_OLED_ORDER_TRANS_LOOP);
                    }
                    break;
                }                    
            }
            break;
            
        case BBPG_EVENT_INPUT_UNDO_SAFE:
            {
                BBPGInteractInputSend(BBPG_INTERACT_UNDO_ALARM_RELIEVE);
            }
            break;
            
        case BBPG_EVENT_INPUT_TOUCH_DETECT:
            {            
                if(BBPG_CONNECTION_STATE == BBPG_LAUNCH)
                {
                    BBPG_CONNECTION_STATE = BBPG_WAIT_CONNECT;
                        
                    BBPGInteractInputSend(BBPG_INTERACT_LEFT_FROM_LAUNCH_PIC);                   
                    break;
                }
                    
                if(BBPG_OLED_SLEEP_STATE == BBPG_WAKE_UP)
                {
                    if(BBPG_OLED_DISPLAY_MODE == BBPG_OLED_LOOP_DISPLAY)
                    {
                        if(BBPG_CONNECTION_STATE == BBPG_CONNECTED)
                        {
                            if(BBPG_UNDO_STATE == BBPG_UNDO_DETECT_SAFE)
                            {
                                BBPG_OLED_SLEEP_STATE = BBPG_SLEEP;
                                   
                                BBPGInteractInputSend(BBPG_INTERACT_OLED_GOTO_SLEEP);
                            }
                        }
                    }
                    else if(BBPG_OLED_DISPLAY_MODE == BBPG_OLED_ORDER_DISPLAY)
                    {
                        BBPGInteractInputSend(BBPG_INTERACT_OLED_ORDER_CHANGE_FRAME);
                    }
                            
                    break;
                }
                    
                if(BBPG_OLED_SLEEP_STATE == BBPG_SLEEP)
                {
                    BBPG_OLED_SLEEP_STATE = BBPG_WAKE_UP;
                        
                    BBPGInteractInputSend(BBPG_INTERACT_OLED_WAKEUP_ORDER_DIS);
                       
                    break;
                }
            }
            break;
            
        case BBPG_EVENT_INPUT_ORDER_DISPLAY_TIMEOUT:
            {
                if(BBPG_OLED_SLEEP_STATE == BBPG_WAKE_UP)
                {
                    BBPG_OLED_SLEEP_STATE = BBPG_SLEEP;
                            
                    BBPGInteractInputSend(BBPG_INTERACT_OLED_GOTO_SLEEP);
                }                
            }
            break;
            
        default:
            break;
    }
}

