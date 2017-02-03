#include "bbpg_alarm.h"
/*
volatile struct __CLK_PER_REG clk_per_reg __attribute__((at(CLK_PER_REG)));
volatile struct __TIMER0_CTRL_REG timer0_ctrl_reg __attribute__((at(TIMER0_CTRL_REG)));
volatile struct __TRIPLE_PWM_CTRL_REG triple_pwm_ctrl_reg __attribute__((at(TRIPLE_PWM_CTRL_REG)));
*/

static uint16_t timerMVal;
static uint16_t timerNVal;
static uint16_t pwm234Frequency;

static GPIO_PORT undoPort;
static GPIO_PIN undoPin;

void timerAndLedSounderMotorPwmInit(GPIO_PORT red_port, GPIO_PIN red_pin, 
                                    GPIO_PORT green_port, GPIO_PIN green_pin,
                                    GPIO_PORT sounder_port, GPIO_PIN sounder_pin, 
                                    GPIO_PORT motor_port, GPIO_PIN motor_pin,
                                    uint16_t timer_m_val, uint16_t timer_n_val,
                                    uint16_t pwm234_frequency)
{
    timerMVal = timer_m_val;
    timerNVal = timer_n_val;
    pwm234Frequency = pwm234_frequency;
    
    RESERVE_GPIO(SOUNDER_PWM, sounder_port, sounder_pin, PID_PWM4);
    RESERVE_GPIO(RED_LED, red_port, red_pin, PID_PWM2);
    RESERVE_GPIO(GREEN_LED, green_port, green_pin, PID_PWM3);
    RESERVE_GPIO(MOTOR_PWM, motor_port, motor_pin, PID_PWM0);
    
    GPIO_ConfigurePin(sounder_port, sounder_pin, OUTPUT, PID_PWM4, false);
    GPIO_ConfigurePin(red_port, red_pin, OUTPUT, PID_PWM2, true);
    GPIO_ConfigurePin(green_port, green_pin, OUTPUT, PID_PWM3, true);  
    GPIO_ConfigurePin(motor_port, motor_pin, OUTPUT, PID_PWM0, false);    

    // global clock set
    SetBits16(CLK_PER_REG, TMR_DIV, 0x03); // divided by 8, 16MHZ/8 = 2MHZ
    SetBits16(CLK_PER_REG, TMR_ENABLE, 0x01); // enable timer0,timer2 clock
    
    /***************TIMER0,TIMER2 PARAMETERS SET *************************
    timer0: frequency = {[sys_clock(fast_16mhz)/CLK_PER_REG->TMR_DIV] OR [sys_clock(slow_32khz)]}/[(M+1)+(N+1)]
            duty      = (M+1)/[(M+1)+(N+1)]
           
    timer2: frequency_in  = sys_clock(fast_16mhz or slow_32khz)/CLK_PER_REG->TMR_DIV
            frequency_out = frequency_in/TRIPLE_PWM_FREQUENCY->FREQ ,frequency_out=(2,2^14 -1)
    
    **********************************************************************/
    
    // timer0 set
    SetBits16(TIMER0_CTRL_REG, PWM_MODE, 0x01);
    //SetBits16(TIMER0_CTRL_REG, TIM0_CLK_DIV, 0x00); // selected clock divided by 10, 2MHZ/10= 8KHZ, but this only appliesto on-counter, no affect tp pwm
    SetBits16(TIMER0_CTRL_REG, TIM0_CLK_SEL, 0x01); // select fast clock

    //SetWord16(TIMER0_ON_REG, timer_on_val); // on-counter used to interrupt, no pwm
    
    SetWord16(TIMER0_RELOAD_M_REG, timer_m_val);
    SetWord16(TIMER0_RELOAD_N_REG, timer_n_val);
    
    SetBits16(TRIPLE_PWM_FREQUENCY, FREQ, pwm234_frequency);
    
    SetBits16(TRIPLE_PWM_CTRL_REG, TRIPLE_PWM_ENABLE, 0x01); // PWM2,3,4 ENABLE   
    SetBits16(TRIPLE_PWM_CTRL_REG, SW_PAUSE_EN, 0x01); // PWM2,3,4 PAUSE
}

void timer0Start(void)
{
    SetBits16(TIMER0_CTRL_REG, TIM0_CTRL, 0x01); // timer0 on
}

void timer0Stop(void)
{
    SetBits16(TIMER0_CTRL_REG, TIM0_CTRL, 0x00);
}

void setPwm2Duty(uint8_t val)
{
    // val=(0-100)%
    SetBits16(PWM2_DUTY_CYCLE, DUTY_CYCLE, ((uint16_t)val)*pwm234Frequency/100 +1);
}

void setPwm3Duty(uint8_t val)
{
    SetBits16(PWM3_DUTY_CYCLE, DUTY_CYCLE, ((uint16_t)val)*pwm234Frequency/100 +1);
}

void setPwm4Duty(uint8_t val)
{
    SetBits16(PWM4_DUTY_CYCLE, DUTY_CYCLE, ((uint16_t)val)*pwm234Frequency/100 +1);
}

void pwm234Pause(void)
{
    SetBits16(TRIPLE_PWM_CTRL_REG, SW_PAUSE_EN, 0x01);
}

void pwm234Continue(void)
{
    SetBits16(TRIPLE_PWM_CTRL_REG, SW_PAUSE_EN, 0x00);
}   


void breakDetectInit(GPIO_PORT undo_port, GPIO_PIN undo_pin,
                     GPIO_PORT cut_port, GPIO_PIN cut_pin)
{
    undoPort = undo_port;
    undoPin = undo_pin;

    RESERVE_GPIO(UNDO_DETECT, undo_port, undo_pin, PID_GPIO);
    RESERVE_GPIO(CUT_DETECT, cut_port, cut_pin, PID_GPIO);
    
    GPIO_ConfigurePin(undo_port, undo_pin, INPUT, PID_GPIO, false);
    //GPIO_ConfigurePin(cut_port, cut_pin, INPUT, PID_GPIO, false);

    GPIO_EnableIRQ(undo_port, undo_pin, GPIO0_IRQn, true, true, 10);
    //GPIO_EnableIRQ(cut_port, cut_pin, GPIO1_IRQn, true, true, 10);
    
    //GPIO_RegisterCallback(GPIO0_IRQn, undoDetectIRQ);
    //GPIO_RegisterCallback(GPIO1_IRQn, cutDetectIRQ);
}

bool isUndoDetectAlarm(void)
{
    return ~GPIO_GetPinStatus(undoPort, undoPin);
}

void setUndoDetectTheConnection(void)
{
    // IRQ occur when pin go high
    GPIO_EnableIRQ(undoPort, undoPin, GPIO0_IRQn, false, true, 10);
}

void setUndoDetectTheDisconnection(void)
{
    // IRQ occur when pin go low
    GPIO_EnableIRQ(undoPort, undoPin, GPIO0_IRQn, true, true, 10);
}

