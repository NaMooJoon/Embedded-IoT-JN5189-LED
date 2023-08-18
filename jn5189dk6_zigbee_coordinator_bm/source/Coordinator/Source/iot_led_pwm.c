#include "iot_led_pwm.h"
#include "fsl_pwm.h"
#include "math.h"

#include "fsl_debug_console.h"

static volatile pwm_setup_t pwmChan;

float pwm0Duty = 0;
float pwm3Duty = 0;
float pwm6Duty = 0;

volatile float count0 = 0;
volatile float count3 = 0;
volatile float count6 = 0;

volatile float pwm0DutyInterval = 0;
volatile float pwm3DutyInterval = 0;
volatile float pwm6DutyInterval = 0;

uint16_t g_step = 0;
bool isChange = false;

void initializeRgbLed(void)
{
    initializePWM0();
    initializePWM3();
    initializePWM6();
}

void changeHSV(uint8_t hue, uint8_t sat, uint8_t val)
{
	uint8_t RGB[3];
	convertHsvToRgb(RGB, hue, sat, 100);
	changeRGB(RGB, val);
}

void convertHsvToRgb(uint8_t *RGB, uint16_t hue, uint8_t sat,  uint8_t val)
{
	float C = (val / 100.0) * (sat/ 100.0);
	float X = C * (1-fabs(fmodf((hue/60.0), 2.0) - 1));
	float m = (val / 100.0) - C;

	if (0 <= hue && hue < 60) {
		RGB[RED]	= (C+m) * 255;
		RGB[GREEN]	= (X+m) * 255;
		RGB[BLUE]	= (0+m) * 255;
	}
	else if (hue < 120) {
		RGB[RED]	= (X+m) * 255;
		RGB[GREEN]	= (C+m) * 255;
		RGB[BLUE]	= (0+m) * 255;
	}
	else if (hue < 180) {
		RGB[RED]	= (0+m) * 255;
		RGB[GREEN]	= (C+m) * 255;
		RGB[BLUE]	= (X+m) * 255;
	}
	else if (hue < 240) {
		RGB[RED]	= (0+m) * 255;
		RGB[GREEN]	= (X+m) * 255;
		RGB[BLUE]	= (C+m) * 255;
	}
	else if (hue < 300) {
		RGB[RED]	= (X+m) * 255;
		RGB[GREEN]	= (0+m) * 255;
		RGB[BLUE]	= (C+m) * 255;
	}
	else if (hue <= 360) {
		RGB[RED]	= (C+m) * 255;
		RGB[GREEN]	= (0+m) * 255;
		RGB[BLUE]	= (X+m) * 255;
	}

	PRINTF("\r\n===> R: %d, G: %d, B:%d\r\n", RGB[RED], RGB[GREEN], RGB[BLUE]);
}

void changeRGB(uint8_t *RGB, uint8_t value)
{
	float intensityRate = (RGB[RED] + RGB[GREEN] + RGB[BLUE])? (float) (value/100.0) * 255.0 / (RGB[RED] + RGB[GREEN] + RGB[BLUE]) : 0;

	pwm0Duty = (RGB[RED]   / MAX_RGB) * MAX_PWM * intensityRate;
	pwm3Duty = (RGB[GREEN] / MAX_RGB) * MAX_PWM * intensityRate;
	pwm6Duty = (RGB[BLUE]  / MAX_RGB) * MAX_PWM * intensityRate;

	PRINTF("      RGB(power): { %d, %d, %d }\r\n", (int)pwm0Duty, (int)pwm3Duty, (int)pwm6Duty);
	PRINTF("      \t=> power: %d (R+G+B) /* MAX: 1000 */\r\n", (int)(pwm0Duty + pwm3Duty + pwm6Duty));

	pwm0DutyInterval = (pwm0Duty - count0) / LED_CHANGE_STEP;
	pwm3DutyInterval = (pwm3Duty - count3) / LED_CHANGE_STEP;
	pwm6DutyInterval = (pwm6Duty - count6) / LED_CHANGE_STEP;

    g_step = 0;
    isChange = true;

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
	static volatile int time = 0;

	count0 = count0 + pwm0DutyInterval;

	if (fabs(count0 - pwm0Duty) < FLT_EPSILON)// || time > 1.5*LED_CHANGE_STEP)
	{
		if (count0 <= FLT_EPSILON){
			PWM_StopTimer(PWM, kPWM_Pwm0);
		}
		count0 = pwm0Duty;
		pwm0DutyInterval = 0;
		time = 0;
	}
	else {
		++time;
	}

	/* Change the compare value in the channel setup structure to change the duty cycle, ramping up over 2000 steps */
	pwmChan.comp_val = (count0 * pwmChan.period_val) / 1000;

	/* Re-apply the channel setup to update the compare value */
	PWM_SetupPwm(PWM, kPWM_Pwm0, (pwm_setup_t *)&pwmChan);

	/* Handle PWM channel interrupt, clear interrupt status */
	PWM_ClearStatusFlags(PWM, kPWM_Pwm0);

}

/* Green */
void PWM3_IRQHandler(void)
{
	static volatile int time = 0;

	count3 = count3 + pwm3DutyInterval;

	if (fabs(count3 - pwm3Duty) < FLT_EPSILON)//  || time > 1.5*LED_CHANGE_STEP)
	{
		if (count3 <= FLT_EPSILON){
			PWM_StopTimer(PWM, kPWM_Pwm3);
		}
		count3 = pwm3Duty;
		pwm3DutyInterval = 0;
		time = 0;
	}
	else {
		++time;
	}

	/* Change the compare value in the channel setup structure to change the duty cycle, ramping up over 2000 steps */
	pwmChan.comp_val = (count3 * pwmChan.period_val) / 1000;

	/* Re-apply the channel setup to update the compare value */
	PWM_SetupPwm(PWM, kPWM_Pwm3, (pwm_setup_t *)&pwmChan);

	/* Handle PWM channel interrupt, clear interrupt status */
	PWM_ClearStatusFlags(PWM, kPWM_Pwm3);

}

/* Blue */
void PWM6_IRQHandler(void)
{
	static volatile int time = 0;

	count6 = count6 + pwm6DutyInterval;

	if (fabs(count6 - pwm6Duty) < FLT_EPSILON)//  || time > 1.5*LED_CHANGE_STEP)
	{
		if (count6 <= FLT_EPSILON){
			PWM_StopTimer(PWM, kPWM_Pwm6);
		}
		count6 = pwm6Duty;
		pwm6DutyInterval = 0;
		time = 0;
	}
	else {
		++time;
	}

	/* Change the compare value in the channel setup structure to change the duty cycle, ramping up over 2000 steps */
	pwmChan.comp_val = (count6 * pwmChan.period_val) / 1000;

	/* Re-apply the channel setup to update the compare value */
	PWM_SetupPwm(PWM, kPWM_Pwm6, (pwm_setup_t *)&pwmChan);

	/* Handle PWM channel interrupt, clear interrupt status */
	PWM_ClearStatusFlags(PWM, kPWM_Pwm6);

}

uint8_t gammaCorrection(uint8_t linear)
{
    return (uint8_t)(pow((float)linear / (float)MAX_PWM, 1.0 / GAMMA) * MAX_PWM + 0.5);
}


