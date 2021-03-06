#ifndef _BBPG_USER_SETUP_H_
#define _BBPG_USER_SETUP_H_

#define UART1_BAUDRATE     	UART_BAUDRATE_115K2       // Baudrate in bits/s: {9K6, 14K4, 19K2, 28K8, 38K4, 57K6, 115K2}
#define UART1_DATALENGTH   	UART_CHARFORMAT_8         // Datalength in bits: {5, 6, 7, 8}
#define UART1_PARITY       	UART_PARITY_NONE          // Parity: {UART_PARITY_NONE, UART_PARITY_EVEN, UART_PARITY_ODD}
#define UART1_STOPBITS     	UART_STOPBITS_1           // Stop bits: {UART_STOPBITS_1, UART_STOPBITS_2} 
#define UART1_FLOWCONTROL  	UART_FLOWCONTROL_DISABLED // Flow control: {UART_FLOWCONTROL_DISABLED, UART_FLOWCONTROL_ENABLED}

#define UART1_GPIO_PORT    	GPIO_PORT_0
#define UART1_TX_PIN       	GPIO_PIN_2
#define UART1_RX_PIN       	GPIO_PIN_3

#define UART_PRINTF_BUFFER_SIZE 128

#define MPU_I2C_GPIO_PORT  	GPIO_PORT_0
#define MPU_I2C_SCL_PIN    	GPIO_PIN_4
#define MPU_I2C_SDA_PIN    	GPIO_PIN_5

#define LED_PORT			GPIO_PORT_2
#define LED_RED_PIN		    GPIO_PIN_5
#define LED_GREEN_PIN		GPIO_PIN_6

#define SOUNDER_PORT        GPIO_PORT_2
#define SOUNDER_PIN         GPIO_PIN_7

#define MOTOR_PORT          GPIO_PORT_2
#define MOTOR_PIN           GPIO_PIN_8

#define OLED_POWER_GPIO_PORT GPIO_PORT_1
#define OLED_POWER_GPIO_PIN  GPIO_PIN_2

#define OLED_I2C_GPIO_PORT 	GPIO_PORT_0
#define OLED_I2C_SCL_PIN	GPIO_PIN_6
#define OLED_I2C_SDA_PIN	GPIO_PIN_7

#define OLED_RES_PORT       GPIO_PORT_1
#define OLED_RES_PIN        GPIO_PIN_1

#define TOUCH_IO_PORT       GPIO_PORT_2
#define TOUCH_IO_PIN        GPIO_PIN_2

//INTC
#define UNDO_DETECT_PORT    GPIO_PORT_2   
#define UNDO_DETECT_PIN     GPIO_PIN_9

//INTA
#define CUT_DETECT_PORT     GPIO_PORT_2
#define CUT_DETECT_PIN      GPIO_PIN_3

#define BAT_ADC_PORT        GPIO_PORT_0
#define BAT_ADC_PIN         GPIO_PIN_0

#define SYSTEM_TIME_PERIOD  100     // unit ms

#define BBPG_IDENTIFY_TIME  1000    // 10s

#define BBPG_UI_MAX_FRAME   8

#define BBPG_TIMEZONE_SET(n) (n*3600)

#define BBPG_BAT_CAL_K      2.0
#define BBPG_BAT_OK_VAL     4200.0  // mv
#define BBPG_BAT_WORRY_VAL  4000.0  // mv
#define BBPG_BAT_OMG_VAL    3700.0  // mv

#define BBPG_MOTOR_DELAY_PAR 1000000

#define BBPG_PHONE2BAND_MESS_HEAD '#'
#define BBPG_PHONE2BAND_MESS_REAR '%'

#define BBPG_BAND2PHONE_MESS_HEAD '@'
#define BBPG_BAND2PHONE_MESS_REAR '$'

#define DATA_UNDO_ALARM_SEND_TO_PHONE "UNDO_ALARM"

#define BBPG_MPU9250_FIFO_MAX_SIZE 48 // 3*16

typedef enum
{
    BBPG_UI_LAUNCH = 0,

    BBPG_UI_WORK,

    //////////////////////

    //BBPG_UI_WAIT_CONNECT_1,
    //BBPG_UI_WAIT_CONNECT_2,
    //BBPG_UI_IDENTIFY_1,
    //BBPG_UI_IDENTIFY_2,
    //BBPG_UI_CONNECT_OK_1,
    //BBPG_UI_CONNECT_OK_2,
    
    //BBPG_UI_IDENTIFY_SUCCESS,
    //BBPG_UI_IDENTIFY_FAIL,
    
    //BBPG_UI_CONNECT_LOSS_1,
    //BBPG_UI_CONNECT_LOSS_2,
    
    BBPG_UI_SLEEP,
    BBPG_UI_WAKE_UP,
    
    BBPG_UI_MENU,
}BBPG_UI_STATE_T;

typedef enum
{
    BBPG_NO_CONNECTION = 0,
    BBPG_IDENTIFY,
    BBPG_HAVE_CONNECTION,
    
    BBPG_HAVE_CONNECTION_OLED_SLEEP,
    
    BBPG_HAVE_CONNECTION_UNDO_CHECK,
    BBPG_HAVE_CONNECTION_CUT_CHECK,
}BBPG_STATE_T;

typedef enum
{
    BBPG_UNDO_DETECT_NO_WORK,
    BBPG_UNDO_DETECT_SAFE,
    BBPG_UNDO_DETECT_ALARM,
}BBPG_UNDO_DETECT_STATE_T;

typedef enum
{
    BBPG_BAT_UNINIT,
    
    BBPG_BAT_OK,
    BBPG_BAT_WORRY, // 5%-15%
    
    BBPG_BAT_OMG,   // 0%-5%
}BBPG_BATTERY_STATE_T;

typedef enum
{
    BBPG_ACCEL_METER_UNINIT,
    
    BBPG_ACCEL_METER_WORK,
}BBPG_ACCEL_METER_STATE_T;

//typedef enum
//{
//    BBPG_OPTION_UNDO_ALARM,
//}BBPG_OPTION_STATE_T;

typedef void (* UI_FRAME_CALLBACK)(void);

typedef struct 
{
    uint8_t priority;
    uint16_t showTime;
    UI_FRAME_CALLBACK ui_frame_cb;
}UI_FRAME_CLASS;

enum
{
    BBPG_COM_TYPE_IDENTIFY = 1,
    BBPG_COM_TYPE_UNDO_ALARM_ENABLE,
    BBPG_COM_TYPE_JUST_UPDATE_TIME,
    
    BBPG_COM_TYPE_GET_BATTERY_VAL,
};

typedef struct 
{
    uint8_t UNDO_ALARM_ENABLE;
    uint8_t MPU_WORK_ENABLE;
}BBPG_OPTION_T;

extern BBPG_STATE_T BBPG_STATE;
extern BBPG_UI_STATE_T BBPG_UI_STATE;
extern BBPG_UNDO_DETECT_STATE_T BBPG_UNDO_STATE;
extern BBPG_BATTERY_STATE_T BBPG_BAT_STATE;
extern BBPG_ACCEL_METER_STATE_T BBPG_ACCEL_STATE;

extern BBPG_OPTION_T BBPG_OPTION;

extern uint32_t UNIX_TIMESTAMP;
extern uint32_t motionDetectTimestamp_ms;

#endif



