#include <avr/io.h>
#include <util/delay.h>

#define NUM_PIXELS 8

#include "ws2812.h"

int main(void)
{

	DDRB |= (1 << DDB0);

	_delay_ms(100);

    while (1)
    {
		setPixelRGB(0, 0x44, 0, 0);
		render();
		_delay_ms(100);
		setPixelRGB(0, 0, 0x44, 0);
		render();
		_delay_ms(100);
		setPixelRGB(0, 0, 0, 0x44);
		render();
		_delay_ms(100);
    }
}
