/*
 * steering_throttle.c 
 *
 * Created: 2017-05-04 9:58:13 PM
 *  Author: Oluwatoni
 */ 
#define ADC_PIN 0
#define ADC_RANGE 10
#define ADC_SAMPLE_NO 8

void setupADC(){
  //enables the ADC  ADEN = 1;  //right adjusted  ADLAR = 0;  //free running
  
}

void steer(uint16_t steering_angle){
  uint16_t current_angle = 0;
  
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
  }
}