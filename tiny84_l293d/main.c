/*
 * main.c
 *
 *  Created on: 05.01.2018
 */

#include <avr/io.h>
#include <util/delay.h>

#define M0_EN PA5 // OC1B
#define M0_IN1 PA1
#define M0_IN2 PA2
#define M0_PORT PORTA
#define M0_DDR DDRA
#define LED PA0
#define LED_PORT PORTA
#define LED_DDR DDRA
#define BUTTON PB0
#define BUTTON_PORT PORTB
#define BUTTON_DDR DDRB

void setMotor(char speed, char reverse) {
	if (reverse == 0) {
		M0_PORT |= _BV(M0_IN1);
		M0_PORT &= ~_BV(M0_IN2);
	} else {
		M0_PORT |= _BV(M0_IN2);
		M0_PORT &= ~_BV(M0_IN1);
	}

	OCR1B = speed;
}

void blink(char times) {
	for (char i = 0; i < times; i++) {
		LED_PORT ^= (1 << LED);
		_delay_ms(100);
		LED_PORT ^= (1 << LED);
		_delay_ms(100);
	}
}

int main(int argc, char **argv) {


	// set outputs
	M0_DDR |= _BV(M0_EN) | _BV(M0_IN1) | _BV(M0_IN2);
	LED_DDR |= _BV(LED);

	// set inputs
	BUTTON_DDR &= ~_BV(BUTTON);
	BUTTON_PORT |= _BV(BUTTON); // internal pullup

	blink(6);

	// COM1B1 --> Clear on compare match, non inverting
	// WGM12 | WGM10 --> FAST PWM 8 BIT (TOP = 255)
	// CS10 --> Prescaler 1
	TCCR1A = _BV(COM1B1) | _BV(WGM10);
	TCCR1B = _BV(CS10) | _BV(WGM12);

	char direction = 0;

	while (1) {
		// Pegel am Button auf LOW?
		if(!(PINB & (1<<BUTTON))){
			blink(2);
			direction = !direction;
			setMotor(255, direction);
		}
	}

}
