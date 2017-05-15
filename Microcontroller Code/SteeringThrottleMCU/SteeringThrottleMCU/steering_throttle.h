/*
 * steering_throttle.h
 *
 * Created: 2017-05-07 7:53:35 PM
 *  Author: funmi
 */ 


#ifndef STEERING_THROTTLE_H_
#define STEERING_THROTTLE_H_

#define ADC_PIN 0
#define ADC_RANGE 13
#define ADC_SAMPLE_NO 10
#define VREF 5
#define ADC_CHANNEL 0
#define MIN_STEERING_ANGLE -30
#define MAX_STEERING_ANGLE 30
#define UPPER_STEERING_BOUND (618 - ADC_RANGE)
#define LOWER_STEERING_BOUND (418 + ADC_RANGE)
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

uint16_t steering_value;
uint16_t direction;
void setupADC();
void setupSteeringTimer();
int getSteeringAngle();
uint16_t ReadADC();
void setupSteeringThrottle();
void steer();
void setSteeringAngle(int8_t angle);//-30 to 30 degrees

#endif /* STEERING_THROTTLE_H_ */