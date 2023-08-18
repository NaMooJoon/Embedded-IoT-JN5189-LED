#include "iot_led_pwm.h"
#include "fsl_pwm.h"
#include "math.h"

#include "fsl_debug_console.h"

static volatile pwm_setup_t pwmChan;

volatile float pwm0Duty = 0;
volatile float pwm3Duty = 0;
volatile float pwm6Duty = 0;

volatile float pwm0DutyInterval = 0;
volatile float pwm3DutyInterval = 0;
volatile float pwm6DutyInterval = 0;

void initializeRgbLed(void)
{
    initializePWM0();
    initializePWM3();
    initializePWM6();
}

void changeRGB(uint8_t *RGB, uint8_t value)
{
	static float prevPwm0Duty = 0;
	static float prevPwm3Duty = 0;
	static float prevPwm6Duty = 0;

	float intensityRate = (RGB[red] + RGB[green] + RGB[blue])? (float) value / (RGB[red] + RGB[green] + RGB[blue]) : 0;

	pwm0Duty = (RGB[red]   / MAX_RGB) * MAX_PWM * intensityRate;
	pwm3Duty = (RGB[green] / MAX_RGB) * MAX_PWM * intensityRate;
	pwm6Duty = (RGB[blue]  / MAX_RGB) * MAX_PWM * intensityRate;

	PRINTF("\r\nRGB(power): { %d, %d, %d }\r\n", (int)pwm0Duty, (int)pwm3Duty, (int)pwm6Duty);
	PRINTF("\t=> power: %d (R+G+B) /* MAX: 1000 */\r\n", (int)(pwm0Duty + pwm3Duty + pwm6Duty));

	pwm0DutyInterval = (pwm0Duty - prevPwm0Duty) / LED_CHANGE_STEP;
	pwm3DutyInterval = (pwm3Duty - prevPwm3Duty) / LED_CHANGE_STEP;
	pwm6DutyInterval = (pwm6Duty - prevPwm6Duty) / LED_CHANGE_STEP;

    prevPwm0Duty = pwm0Duty;
    prevPwm3Duty = pwm3Duty;
    prevPwm6Duty = pwm6Duty;

    /* Start the PWM generation channel */
    PWM_StartTimer(PWM, kPWM_Pwm0);
    PWM_StartTimer(PWM, kPWM_Pwm3);
    PWM_StartTimer(PWM, kPWM_Pwm6);
}

void initializePWM0(void)
{
    pwm_config_t pwmConfig;
    uint32_t pwmSourceClockFreq;
    uint32_t pwmChannelClockFreq;

    PWM_GetDefaultConfig(&pwmConfig);

    PWM_Init(PWM, &pwmConfig);

    pwmSourceClockFreq = CLOCK_GetFreq(kCLOCK_Pwm);

    pwmChan.pol_ctrl		= kPWM_SetHighOnMatchLowOnPeriod;
    pwmChan.dis_out_level	= kPWM_SetLow;
    pwmChan.prescaler_val	= 0;
    pwmChannelClockFreq		= pwmSourceClockFreq / (1 + pwmChan.prescaler_val);
    pwmChan.period_val    = USEC_TO_COUNT(100000, pwmChannelClockFreq);

    /* Compare value starts same as period value to give a 100% starting duty cycle */
    pwmChan.comp_val = pwmChan.period_val;
    PWM_SetupPwm(PWM, kPWM_Pwm0, (pwm_setup_t *)&pwmChan);

    /* Clear interrupt status for PWM channel */
    PWM_ClearStatusFlags(PWM, kPWM_Pwm0);

    /* Enable IRQ in NVIC for PWM channel 0 */
    EnableIRQ(PWM0_IRQn);

    /* Enable PWM channel interrupt */
    PWM_EnableInterrupts(PWM, kPWM_Pwm0);

}

void initializePWM3(void)
{
    pwm_config_t pwmConfig;
    uint32_t pwmSourceClockFreq;
    uint32_t pwmChannelClockFreq;

    PWM_GetDefaultConfig(&pwmConfig);

    PWM_Init(PWM, &pwmConfig);

    pwmSourceClockFreq = CLOCK_GetFreq(kCLOCK_Pwm);

    pwmChan.pol_ctrl		= kPWM_SetHighOnMatchLowOnPeriod;
    pwmChan.dis_out_level	= kPWM_SetLow;
    pwmChan.prescaler_val	= 0;
    pwmChannelClockFreq		= pwmSourceClockFreq / (1 + pwmChan.prescaler_val);
    pwmChan.period_val    = USEC_TO_COUNT(100000, pwmChannelClockFreq);

    /* Compare value starts same as period value to give a 100% starting duty cycle */
    pwmChan.comp_val = pwmChan.period_val;
    PWM_SetupPwm(PWM, kPWM_Pwm3, (pwm_setup_t *)&pwmChan);

    /* Clear interrupt status for PWM channel */
    PWM_ClearStatusFlags(PWM, kPWM_Pwm3);

    /* Enable IRQ in NVIC for PWM channel 3 */
    EnableIRQ(PWM3_IRQn);

    /* Enable PWM channel interrupt */
    PWM_EnableInterrupts(PWM, kPWM_Pwm3);

}
void initializePWM6(void)
{
    pwm_config_t pwmConfig;
    uint32_t pwmSourceClockFreq;
    uint32_t pwmChannelClockFreq;
	PWM_GetDefaultConfig(&pwmConfig);

    PWM_Init(PWM, &pwmConfig);

    pwmSourceClockFreq = CLOCK_GetFreq(kCLOCK_Pwm);

    pwmChan.pol_ctrl		= kPWM_SetHighOnMatchLowOnPeriod;
    pwmChan.dis_out_level	= kPWM_SetLow;
    pwmChan.prescaler_val	= 0;
    pwmChannelClockFreq		= pwmSourceClockFreq / (1 + pwmChan.prescaler_val);
    pwmChan.period_val    = USEC_TO_COUNT(100000, pwmChannelClockFreq);

    /* Compare value starts same as period value to give a 100% starting duty cycle */
    pwmChan.comp_val = pwmChan.period_val;
    PWM_SetupPwm(PWM, kPWM_Pwm6, (pwm_setup_t *)&pwmChan);

    /* Clear interrupt status for PWM channel */
    PWM_ClearStatusFlags(PWM, kPWM_Pwm6);

    /* Enable IRQ in NVIC for PWM channel 6 */
    EnableIRQ(PWM6_IRQn);

    /* Enable PWM channel interrupt */
    PWM_EnableInterrupts(PWM, kPWM_Pwm6);

}

/* Red */
void PWM0_IRQHandler(void)
{
	static float count = 0;

	count = count + pwm0DutyInterval;
	if (abs(count - pwm0Duty) < FLT_EPSILON)
	{
		if (count < FLT_EPSILON){
			PWM_StopTimer(PWM, kPWM_Pwm0);
		}
		pwm0DutyInterval = 0;
	}

	/* Change the compare value in the channel setup structure to change the duty cycle, ramping up over 2000 steps */
	pwmChan.comp_val = (count * pwmChan.period_val) / 1000;

	/* Re-apply the channel setup to update the compare value */
	PWM_SetupPwm(PWM, kPWM_Pwm0, (pwm_setup_t *)&pwmChan);

	/* Handle PWM channel interrupt, clear interrupt status */
	PWM_ClearStatusFlags(PWM, kPWM_Pwm0);
}

/* Green */
void PWM3_IRQHandler(void)
{
	static float count = 0;

	count = count + pwm3DutyInterval;
	if (abs(count - pwm3Duty) < FLT_EPSILON)
	{
		if (count < FLT_EPSILON){
			PWM_StopTimer(PWM, kPWM_Pwm3);
		}
		pwm3DutyInterval = 0;
	}

	/* Change the compare value in the channel setup structure to change the duty cycle, ramping up over 2000 steps */
	pwmChan.comp_val = (count * pwmChan.period_val) / 1000;

	/* Re-apply the channel setup to update the compare value */
	PWM_SetupPwm(PWM, kPWM_Pwm3, (pwm_setup_t *)&pwmChan);

	/* Handle PWM channel interrupt, clear interrupt status */
	PWM_ClearStatusFlags(PWM, kPWM_Pwm3);
}

/* Blue */
void PWM6_IRQHandler(void)
{
	static float count = 0;

	count = count + pwm6DutyInterval;
	if (abs(count - pwm6Duty) < FLT_EPSILON)
	{
		if (count < FLT_EPSILON){
			PWM_StopTimer(PWM, kPWM_Pwm6);
		}
		pwm6DutyInterval = 0;
	}

	/* Change the compare value in the channel setup structure to change the duty cycle, ramping up over 2000 steps */
	pwmChan.comp_val = (count * pwmChan.period_val) / 1200;

	/* Re-apply the channel setup to update the compare value */
	PWM_SetupPwm(PWM, kPWM_Pwm6, (pwm_setup_t *)&pwmChan);

	/* Handle PWM channel interrupt, clear interrupt status */
	PWM_ClearStatusFlags(PWM, kPWM_Pwm6);
}

uint8_t gammaCorrection(uint8_t linear)
{
    return (uint8_t)(pow((float)linear / (float)MAX_PWM, 1.0 / GAMMA) * MAX_PWM + 0.5);
}


