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

void setupSteeringTimer(){
  //http://www.atmel.com/Images/Atmel-2505-Setup-and-Use-of-AVR-Timers_ApplicationNote_AVR130.pdf 
  //set the prescaler to 128
  TCCR2B = (1<<2) | (1<<0);
  //clear the overflow flag
  TIFR2 = 1 << TOV2;
  //Enable interrupts
  TIMSK2 = (1 << TOIE2);
}

ISR(TIMER2_OVF_vect){
  cli();
  TCNT2 = 193;//2KHz
  steer();
  sei();
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
  setupSteeringTimer();
  //setup PD2 and PD3 as out-put pins
  DDRD |= (1 << DDD2);
  DDRD |= (1 << DDD3);
}

int getSteeringAngle(){
  return direction;
}

void steer(){
  uint16_t current_angle = 0;
  for(int i = 0; i < 4; i++)
    current_angle += ReadADC();
  current_angle /= 4;
  
  if (abs(steering_value - current_angle) > ADC_RANGE){
    if (steering_value > current_angle)
      PORTD = (1 << PORTD2);
    else
      PORTD = (1 << PORTD3);
  }
  else
    PORTD = 0; 
}

void setSteeringAngle(int8_t angle){
  steering_value = ((((float)((-angle) - MIN_STEERING_ANGLE))/(float)(MAX_STEERING_ANGLE - MIN_STEERING_ANGLE)) * (UPPER_STEERING_BOUND-LOWER_STEERING_BOUND)) + LOWER_STEERING_BOUND;
}