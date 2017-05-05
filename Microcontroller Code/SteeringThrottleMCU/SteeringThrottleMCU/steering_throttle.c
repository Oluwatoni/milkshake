/*
 * steering_throttle.c 
 *
 * Created: 2017-05-04 9:58:13 PM
 *  Author: Oluwatoni
 */ 
#define ADC_RANGE 10
#define ADC_SAMPLE_NO 8
//Macro to read from the ADC

//Macro to set the output logic

void steer(uint16_t steering_angle){
  uint16_t current_angle = ;
  
  if (abs(steering_angle - current_angle) > ADC_RANGE){
    //correct
    if (steering_angle > current_angle)
      //steer left
    else
      //steer right
  }
}