/*
 * steering_throttle.c 
 *
 * Created: 2017-05-04 9:58:13 PM
 *  Author: Oluwatoni
 */
#include "steering_throttle.h"

void setupADC(){
  // Select Vref=AVcc
  ADMUX |= (1<<REFS0);
  //set prescaler to 128 and enable ADC
  ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);
}

uint16_t ReadADC(){
  //select ADC channel with safety mask
  ADMUX = (ADMUX & 0xF0) | (ADC_CHANNEL & 0x0F);
  //single conversion mode
  ADCSRA |= (1<<ADSC);
  // wait until ADC conversion is complete
  while( ADCSRA & (1<<ADSC) );
  return ADC;
}

void setupSteeringThrottle(){
  setupADC();
}

void steer(uint16_t steering_angle){
  /*uint16_t current_angle = 0;
  
  //end of a an ADC conversion
  current_angle = ADCL;
  //because the lower one must be read first
  current_angle += (ADCH << 8);
  
  if (abs(steering_angle - current_angle) > ADC_RANGE){
    //correct
    if (steering_angle > current_angle)
      //steer left
    else
      //steer right
  }*/
}