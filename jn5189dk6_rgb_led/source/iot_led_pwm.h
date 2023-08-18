#ifndef __IOT_LED_PWM_H__
#define __IOT_LED_PWM_H__

#include "fsl_pwm.h"
#include <math.h>

#define MAX_RGB 255.0
#define MAX_PWM 1000.0
#define GAMMA 2.2
#define LED_CHANGE_STEP 1000.0
#define FLT_EPSILON  3

enum rgb {
	red,
	green,
	blue
};

void initializeRgbLed(void);
void changeRGB(uint8_t *RGB, uint8_t value);

void initializePWM0(void);
void initializePWM3(void);
void initializePWM6(void);

/* Optional function to control the LED brightness linearly */
uint8_t gammaCorrection(uint8_t linear);

/* Red LED of RGB */
void PWM0_IRQHandler(void);
/* Green LED of RGB */
void PWM3_IRQHandler(void);
/* Blue LED of RGB */
void PWM6_IRQHandler(void);


#endif
