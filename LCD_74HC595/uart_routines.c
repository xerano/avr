#include "uart_routines.h"

void uart_init(void) {
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	/* evtl. verkuerzt falls Register aufeinanderfolgen (vgl. Datenblatt)
	 UBRR = UBRR_VALUE;
	 */

	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0A = (1 << UDRE0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);    // Set frame: 8data, 1 stop

#if USE_2X
	/* U2X-Modus erforderlich */
	UCSRA |= (1 << U2X);
#else
	/* U2X-Modus nicht erforderlich */
	UCSR0A &= ~(1 << U2X0);
#endif

	// hier weitere Initialisierungen (TX und/oder RX aktivieren, Modus setzen
}

int uart_putc(unsigned char c) {
	while (!(UCSR0A & (1 << UDRE0))) /* warten bis Senden moeglich */
	{
	}

	UDR0 = c; /* sende Zeichen */
	return 0;
}

/* puts ist unabhaengig vom Controllertyp */
void uart_puts(char *s) {
	while (*s) { /* so lange *s != '\0' also ungleich dem "String-Endezeichen(Terminator)" */
		uart_putc(*s);
		s++;
	}
}

char *binbin(unsigned char n) {
	static char bin[9];
	int x;
	for (x = 0; x < 8; x++) {
		bin[x] = n & 0x80 ? '1' : '0';
		n <<= 1;
	}
	bin[x] = '\0';
	return (bin);
}

void uart_print_byte(unsigned char byte) {
	uart_puts(binbin(byte));
}
