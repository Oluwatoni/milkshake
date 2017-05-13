/*
 * uart.h
 *
 * Created: 2017-05-07 7:29:37 PM
 *  Author: funmi
 * just a UART library to speed up debugging on the atmega328p dw it was tested!
 */ 

#ifndef UART_H_
#define UART_H_

#define F_CPU 16000000
#define FOSC F_CPU
#define BAUD 19200
#define MYUBRR FOSC/16/BAUD-1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint-gcc.h>
#include <math.h>

void UART_Init( unsigned int);
void UART_Transmit( unsigned char data );
unsigned char UART_Receive( void );
void printCharArray(char array[], uint8_t size);
void printNumber(uint16_t value, uint8_t digits);

#endif /* UART_H_ */