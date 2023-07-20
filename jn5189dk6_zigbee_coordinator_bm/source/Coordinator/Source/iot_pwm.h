#ifndef __IOT_PWM_H__
#define __IOT_PWM_H__

#include "fsl_pwm.h"

extern volatile pwm_setup_t pwmChan;

/* Initialize the PMW */
void initializePwm(void);

/* Modulate the pulse width */
void pwm_tune_pulse(uint8_t cycle);

#endif
