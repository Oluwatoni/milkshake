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

int main(void)
{
    setupSteeringThrottle();
    //set up the timer interrupts for the steering and encoder
    //set up the SPI slave to receive data from the BBB
    //set up the UART for debugging purposes and GPS
    UART_Init(MYUBRR);

    while (1) 
    {
      UART_Transmit( ReadADC() & 0xFF);
    }
}

