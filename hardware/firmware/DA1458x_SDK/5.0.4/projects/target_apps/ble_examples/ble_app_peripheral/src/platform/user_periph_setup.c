/**
 ****************************************************************************************
 *
 * @file user_periph_setup.c
 *
 * @brief Peripherals setup and initialization.
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

#include "rwip_config.h"             // SW configuration
#include "user_periph_setup.h"       // peripheral configuration
#include "global_io.h"
#include "gpio.h"
#include "uart.h"                    // UART initialization

/**
 * BBPG INCLUDE *
 */
#include "hw_i2c.h"
#include "mpu9250.h"
#include "oled_dis.h"
#include "touch_detect.h"
#include "bbpg_uart.h"
#include "bbpg_alarm.h"
#include "bbpg_adc.h"

#include "oled_draw.h"

#include "bbpg_user_setup.h"
#include "user_custs1_impl.h"

//#include "app_api.h"
//#include "app.h"


/**
 ****************************************************************************************
 * @brief Each application reserves its own GPIOs here.
 *
 * @return void
 ****************************************************************************************
 */

#ifdef CFG_DEVELOPMENT_DEBUG

void GPIO_reservations(void)
{
/*
* Globally reserved GPIOs reservation
*/

/*
* Application specific GPIOs reservation. Used only in Development mode (#if DEVELOPMENT_DEBUG)

i.e.
    RESERVE_GPIO(DESCRIPTIVE_NAME, GPIO_PORT_0, GPIO_PIN_1, PID_GPIO);    //Reserve P_01 as Generic Purpose I/O
*/
#ifdef CFG_PRINTF_UART2
    RESERVE_GPIO(UART2_TX, UART2_TX_GPIO_PORT, UART2_TX_GPIO_PIN, PID_UART2_TX);
    RESERVE_GPIO(UART2_RX, UART2_RX_GPIO_PORT, UART2_RX_GPIO_PIN, PID_UART2_RX);
#endif
    RESERVE_GPIO(LED, GPIO_LED_PORT, GPIO_LED_PIN, PID_GPIO);
}
#endif // CFG_DEVELOPMENT_DEBUG

void set_pad_functions(void)        // set gpio port function mode
{
#ifdef CFG_PRINTF_UART2
    GPIO_ConfigurePin(UART2_TX_GPIO_PORT, UART2_TX_GPIO_PIN, OUTPUT, PID_UART2_TX, false);
    GPIO_ConfigurePin(UART2_RX_GPIO_PORT, UART2_RX_GPIO_PIN, INPUT, PID_UART2_RX, false);
#endif
    GPIO_ConfigurePin(GPIO_LED_PORT, GPIO_LED_PIN, OUTPUT, PID_GPIO, false);
}


void periph_init(void)
{    
    // Power up peripherals' power domain
    SetBits16(PMU_CTRL_REG, PERIPH_SLEEP, 0);
    while (!(GetWord16(SYS_STAT_REG) & PER_IS_UP));

    SetBits16(CLK_16M_REG, XTAL16_BIAS_SH_ENABLE, 1);

    //rom patch
    patch_func();
    
    timerAndLedSounderMotorPwmInit(LED_PORT, LED_RED_PIN, 
                                   LED_PORT, LED_GREEN_PIN,
                                   SOUNDER_PORT, SOUNDER_PIN, 
                                   MOTOR_PORT, MOTOR_PIN,
                                   499, 499,/*M,N*/
                                   2000/*timer2,pwm 2khz*/); 

    pwm234Continue();
    setPwm2Duty(100); // disable red led
    setPwm3Duty(100); // disable green led
    setPwm4Duty(0); // disable sounder
    
    timer0Stop();     // disable motor, but now use gpio to drive motor
    
    //uart1Init(UART1_BAUDRATE, UART1_DATALENGTH);    
    
    mpu9250Init(MPU_I2C_GPIO_PORT, 
                MPU_I2C_SCL_PIN, 
			    MPU_I2C_SDA_PIN, 
				MPU9250_ADDRESS, I2C_STANDARD, I2C_7BIT_ADDR, I2C_1BYTE_ADDR);    
    
    oledIOInit(OLED_POWER_GPIO_PORT, OLED_POWER_GPIO_PIN,
               OLED_I2C_GPIO_PORT, 
               OLED_I2C_SCL_PIN, OLED_I2C_SDA_PIN,
               OLED_RES_PORT, OLED_RES_PIN);
             
    oledSSD1306Initialization();

    breakDetectInit(CUT_DETECT_PORT, CUT_DETECT_PIN,
                    UNDO_DETECT_PORT, UNDO_DETECT_PIN);
                    
    touchModuleInit(TOUCH_IO_PORT, TOUCH_IO_PIN);    
    
    adcIOInit();
                    
    GPIO_RegisterCallback(GPIO0_IRQn, undo_detect_irq_handler);          
    GPIO_RegisterCallback(GPIO2_IRQn, touch_detect_irq_handler); 
    //GPIO_RegisterCallback(GPIO1_IRQn, cut_detect_irq_handler);      
        
    oledDrawPicture(LAUNCH_PIC);

    //
    //
    //
    //
    //
    //  
    //
    //oledDrawHorizontalStringSet("  Hello World     ", 9, 5);
    
    // Init pads
    //set_pad_functions();

    // (Re)Initialize peripherals
    // i.e.
    //  uart_init(UART_BAUDRATE_115K2, 3);

//#ifdef CFG_PRINTF_UART2
//    SetBits16(CLK_PER_REG, UART2_ENABLE, 1);
//    uart2_init(UART_BAUDRATE_115K2, 3);
//#endif

    // Enable the pads
    //SetBits16(SYS_CTRL_REG, PAD_LATCH_EN, 1);
}


