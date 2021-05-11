#ifndef _WS2812_H_
#define _WS2812_H_

#ifndef NUM_PIXELS
	#define NUM_PIXELS 8
#endif

#define RESET 60
#define PIN_LOW  { PORTB &= ~(1<<DDB0); } // approximately ~ 250ns
#define PIN_HIGH { PORTB |= (1<<DDB0); }  // approximately ~ 250ns

typedef struct LED {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} Pixel;

void low();
void high();
void reset();
void sendByte(uint8_t byte);
void setPixel(uint8_t number, Pixel p);
void setPixelRGB(uint8_t number, uint8_t r, uint8_t g, uint8_t b);
void render();

#endif
