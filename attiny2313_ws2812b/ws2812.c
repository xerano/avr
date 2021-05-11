/*
 * ws2812.c
 *
 *  Created on: 11.05.2021
 *      Author: matthias
 */

#include <avr/io.h>
#include <util/delay.h>
#include "ws2812.h"

Pixel pixel[NUM_PIXELS];

void high(){
	PIN_HIGH;						// 250
	asm volatile("		nop\n");    // 125
	asm volatile("		nop\n");    // 125
	asm volatile("		nop\n");    // 125
	asm volatile("		nop\n");    // 125
	asm volatile("		nop\n");    // 125 ~ 875ns
	PIN_LOW;						// 250
	asm volatile ("		nop	\n");	// 125
	asm volatile ("		nop	\n");	// 125 ~ 500ns

}

void low(){
	PIN_HIGH;						// 250
	asm volatile ("		nop	\n");	// 125 ~ 375ns
	PIN_LOW;						// 250
	asm volatile ("		nop	\n");	// 125
	asm volatile ("		nop	\n");	// 125
	asm volatile ("		nop	\n");	// 125
	asm volatile ("		nop	\n");	// 125
	asm volatile("		nop\n");    // 125 ~ 875ns
}

void reset(){
	PIN_LOW;
	_delay_us(RESET);
}

void sendByte(uint8_t byte){
	for(int i = 0; i < NUM_PIXELS; i++){
		if((byte >> (7 - i)) & 0x01){
			high();
		}else{
			low();
		}
	}
}

// GRB; MSB first
void setColor(uint8_t r, uint8_t g, uint8_t b){
	sendByte(g);
	sendByte(r);
	sendByte(b);
	reset();
}

void setPixel(uint8_t number, Pixel p){
	pixel[number] = p;
	render();
}

void setPixelRGB(uint8_t number, uint8_t r, uint8_t g, uint8_t b){
	pixel[number].r = r;
	pixel[number].g = g;
	pixel[number].b = b;
	render();
}

void render(){
	for(int i = 0; i < NUM_PIXELS; i++){
		setColor(pixel[i].r, pixel[i].g, pixel[i].b);
	}
}
