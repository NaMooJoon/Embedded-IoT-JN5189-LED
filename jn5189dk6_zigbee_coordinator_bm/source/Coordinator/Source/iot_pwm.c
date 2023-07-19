#include <stdio.h>
#include <stdbool.h>

#include "board.h"
#include "fsl_pwm.h"
#include "fsl_inputmux.h"
#include "fsl_flash.h"
#include "fsl_clock.h"

#include "pin_mux.h"
#include "iot_pwm.h"

volatile pwm_setup_t pwmChan;

void initializePwm(void)
{
    pwm_config_t pwmConfig;
    uint32_t pwmSourceClockFreq;
    uint32_t pwmChannelClockFreq;

    /* Get default configuration */
    PWM_GetDefaultConfig(&pwmConfig);

    /* PWM Init with default clock selected */
    PWM_Init(PWM, &pwmConfig);

    /* Get the default source clock frequency */
    pwmSourceClockFreq = CLOCK_GetFreq(kCLOCK_Pwm);

    /* Set up PWM channel to generate PWM pulse of 10ms with a starting 100% duty cycle */
    pwmChan.pol_ctrl      = kPWM_SetHighOnMatchLowOnPeriod;
    pwmChan.dis_out_level = kPWM_SetLow;
    pwmChan.prescaler_val = 0;
    pwmChannelClockFreq   = pwmSourceClockFreq / (1 + pwmChan.prescaler_val);
    pwmChan.period_val    = USEC_TO_COUNT(10000, pwmChannelClockFreq);
    /* Compare value starts same as period value to give a 100% starting duty cycle */
    pwmChan.comp_val = pwmChan.period_val;
    PWM_SetupPwm(PWM, kPWM_Pwm0, (pwm_setup_t *)&pwmChan);

    /* Start the PWM generation channel */
    PWM_StartTimer(PWM, kPWM_Pwm0);
}

void pwm_tune_pulse(uint8_t cycle)
{
    pwmChan.comp_val = ((100 - cycle) * pwmChan.period_val) / 100;
    PWM_SetupPwm(PWM, kPWM_Pwm0, (pwm_setup_t *)&pwmChan);
}
