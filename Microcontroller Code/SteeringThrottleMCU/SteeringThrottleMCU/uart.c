/*
 * uart.c
 *
 * Created: 2017-05-07 7:29:53 PM
 *  Author: funmi
 */ 
#include "uart.h"

void UART_Init( unsigned int ubrr){
  /*Set baud rate */
  UBRR0H = (unsigned char)(ubrr>>8);
  UBRR0L = (unsigned char)ubrr;
  
  /*Enable receiver and transmitter and RX interrupt */
  UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
  /* Set frame format: 8data, 1stop bit */
  UCSR0C = (3<<UCSZ00);
}

void UART_Transmit( unsigned char data ){
  /* Wait for empty transmit buffer */
  while ( !( UCSR0A & (1<<UDRE0)) );
  /* Put data into buffer, sends the data */
  UDR0 = data;
}

unsigned char UART_Receive( void ){
  /* Wait for data to be received */
  while ( !(UCSR0A & (1<<RXC0)) );
  /* Get and return received data from buffer */
  return UDR0;
}

void printCharArray(char array[], uint8_t size){
  for (uint8_t i = 0;i < size; i++)
    UART_Transmit(*(array++));
  UART_Transmit('\n');
}

ISR(USART_RX_vect){
  cli();
  char temp = UDR0;
  sei();
}