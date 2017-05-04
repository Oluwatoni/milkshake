/*
 * GccApplication1.cpp
 *
 * Created: 2016-12-01 10:27:29 PM
 * Author : Kevin Huo
 */ 

#define F_CPU	16000000UL
#define PIN_ON		PORTB |= (1<<PORTB0)
#define PIN_OFF		PORTB &= ~(1<<PORTB0)
#define PIN_TOGGLE	PINB |= (1<<PINB0)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#if 0
void USART_Transmit( unsigned char data ){
	// Wait for empty transmit buffer
	while ( !( UCSR0A & (1<<UDRE0)) );
	// Put data into buffer, sends the data
	UDR0 = data;
}
#endif

void pwm_init(void) {
	TCCR1A |= (1<<COM1A1) | (1<<WGM10);
	TCCR1B |= (1<<CS10) | (1<<WGM12);
	OCR1A = 0;
}

void set_pwm(float percent){
	uint16_t duty = uint16_t(percent*255);
	OCR1A = duty;
}

int main(void) {
    DDRB |= (1<<DDB1);
	
	pwm_init(); // only works for PB1 (pin 9 arduino), setting OCR1A 0-255
	
	sei();
	
	while(1){
		for(float i=0.0; i < 1.0; i+= 0.01){
			set_pwm(i);
			_delay_ms(10);
		}	
		for(float i=1.0; i > 0.0; i-= 0.01){
			set_pwm(i);
			_delay_ms(10);
		}

		
	}
}
