/*
 * steering_throttle.h
 *
 * Created: 2017-05-07 7:53:35 PM
 *  Author: funmi
 */ 


#ifndef STEERING_THROTTLE_H_
#define STEERING_THROTTLE_H_

#define ADC_PIN 0
#define ADC_RANGE 10
#define ADC_SAMPLE_NO 8
#define VREF 5
#define ADC_CHANNEL 0

#include <stdio.h>
#include <avr/io.h>

void setupADC();
uint16_t ReadADC();
void setupSteeringThrottle();
void steer(uint16_t steering_angle);

#endif /* STEERING_THROTTLE_H_ */