#ifndef __IOT_LED_PWM_H__
#define __IOT_LED_PWM_H__

#include "fsl_pwm.h"
#include <math.h>

#define MAX_RGB 255.0
#define MAX_PWM 1000.0
#define GAMMA 2.2
#define LED_CHANGE_STEP 500.0
#define FLT_EPSILON  6

enum rgb {
	RED,
	GREEN,
	BLUE
};

void initializeRgbLed(void);
void changeRGB(uint8_t *RGB, uint8_t value);
void changeHSV(uint8_t hue, uint8_t sat, uint8_t val);
void convertHsvToRgb(uint8_t *RGB, uint16_t hue, uint8_t saturation,  uint8_t brigntness);

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
