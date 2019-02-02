#ifndef __PWM_OUTPUT_H
#define __PWM_OUTPUT_H
#include "main.h"
//Variable: ///////////////////////////////////////////////////////////////////
//Bien cac tham so dutycycle pwm1
extern float channel1Pulse;
extern float channel2Pulse;
extern float channel3Pulse;
extern float channel4Pulse;

//Function: //////////////////////////////////////////////////////////////////////

extern void InitialisePWM(void);
extern void SetTim3Channel1(float pulse);
extern void SetTim3Channel2(float pulse);
extern void SetTim3Channel3(float pulse);
extern void SetTim3Channel4(float pulse);
extern void UpdateTim3Channel1(float pulse);
extern void UpdateTim3Channel2(float pulse);
extern void UpdateTim3Channel3(float pulse);
extern void UpdateTim3Channel4(float pulse);
extern float ReadTim3Channel1Pulse(void);
extern float ReadTim3Channel2Pulse(void);
extern float ReadTim3Channel3Pulse(void);
extern float ReadTim3Channel4Pulse(void);
DutyCycle InitialisePWMChannel(GPIO_TypeDef* GPIOx, uint16_t pin, uint8_t pinSource, uint8_t channel);
#endif
