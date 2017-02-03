#ifndef _BBPG_ALARM_H_
#define _BBPG_ALARM_H_

#include "gpio.h"
#include "datasheet.h"
#include "stdbool.h"

void timerAndLedSounderMotorPwmInit(GPIO_PORT red_port, GPIO_PIN red_pin, 
                                    GPIO_PORT green_port, GPIO_PIN green_pin,
                                    GPIO_PORT sounder_port, GPIO_PIN sounder_pin, 
                                    GPIO_PORT motor_port, GPIO_PIN motor_pin,
                                    uint16_t timer_m_val, uint16_t timer_n_val,
                                    uint16_t pwm234_frequency);
void timer0Start(void);
void timer0Stop(void);
void setPwm2Duty(uint8_t val);
void setPwm3Duty(uint8_t val);
void setPwm4Duty(uint8_t val);
void pwm234Pause(void);
void pwm234Continue(void);

void breakDetectInit(GPIO_PORT undo_port, GPIO_PIN undo_pin,
                     GPIO_PORT cut_port, GPIO_PIN cut_pin);
bool isUndoDetectAlarm(void);
void setUndoDetectTheConnection(void);
void setUndoDetectTheDisconnection(void);

#endif

