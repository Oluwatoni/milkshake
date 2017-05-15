/*
 * SteeringThrottleMCU.c
 *
 * Created: 2017-05-04 9:57:38 PM
 * Author : Oluwatoni
 */ 

#include <avr/io.h>
#include <stdint.h>

#include "uart.h"
#include "steering_throttle.h"

#include <util/delay.h>

int main(void){
    setupSteeringThrottle();
    //set up the SPI slave to receive data from the BBB
    UART_Init(MYUBRR);
    setSteeringAngle(0);
    sei();
    while (1) {
    }
}

