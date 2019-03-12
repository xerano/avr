#include <avr/io.h>
#include <util/delay.h>
#include "spi-lcd-routines.h"
#include "uart_routines.h"


int main(int argc, char **argv) {

	uart_init();

	uart_puts("\n----------\nLCD TEST\n----------\n");

	// Initialisierung des LCD
	  // Nach der Initialisierung müssen auf dem LCD vorhandene schwarze Balken
	  // verschwunden sein
	  lcd_init();

	  // Text in einzelnen Zeichen ausgeben
	  lcd_data( 't' );
	  lcd_data( 'e' );
	  lcd_data( 's' );
	  lcd_data( 't' );

	  // Die Ausgabemarke in die 2te Zeile setzen
	  lcd_setcursor(0, 1);

	  // erneut Text ausgeben, aber diesmal komfortabler als String
	  lcd_string("Hello World!");




	while (1) {

	}

	return 0;

}
