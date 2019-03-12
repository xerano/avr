
#include <avr/io.h>
#include "spi-lcd-routines.h"
#include <util/delay.h>
#include "uart_routines.h"

uint8_t _data;

void init_spi(){
	DDRB |= (1 << SCK) | (1 << MOSI) | (1 << SS);
	//PORTB |= (1 << SCK) | (1 << MOSI) | (1 << SS);
	SPCR |= (1<<SPE) | (1 << MSTR) | (1 << SPR0);
	//SPCR |= (1 << CPOL) | (1 << CPHA);
}

void spi_send_byte(uint8_t data) {
	PORTB &= ~(1 << SS);
	SPDR = data;
	while (bit_is_clear(SPSR, SPIF))
		;
	PORTB |= (1 << SS);
}

////////////////////////////////////////////////////////////////////////////////
// Erzeugt einen Enable-Puls
void lcd_enable( void )
{
	spi_send_byte(_data | LCD_EN);
    _delay_us( LCD_ENABLE_US );  // kurze Pause
    spi_send_byte(_data &  ~LCD_EN);
}

////////////////////////////////////////////////////////////////////////////////
// Sendet eine 4-bit Ausgabeoperation an das LCD
void lcd_out(uint8_t data)
{
    data &= 0xF0;                       // obere 4 Bit maskieren

    _data &= ~(0xF0>>4);    // Maske löschen
    _data |= (data>>4);     // Bits setzen

    lcd_enable();
}

////////////////////////////////////////////////////////////////////////////////
// Initialisierung: muss ganz am Anfang des Programms aufgerufen werden.
void lcd_init( void )
{

	init_spi();
    // initial alle Ausgänge auf Null
	_data = 0x00;

    // warten auf die Bereitschaft des LCD
    _delay_ms( LCD_BOOTUP_MS );

    // Soft-Reset muss 3mal hintereinander gesendet werden zur Initialisierung
    lcd_out( LCD_SOFT_RESET );
    _delay_ms( LCD_SOFT_RESET_MS1 );

    lcd_enable();
    _delay_ms( LCD_SOFT_RESET_MS2 );

    lcd_enable();
    _delay_ms( LCD_SOFT_RESET_MS3 );

    // 4-bit Modus aktivieren
    lcd_out( LCD_SET_FUNCTION |
             LCD_FUNCTION_4BIT );
    _delay_ms( LCD_SET_4BITMODE_MS );

    // 4-bit Modus / 2 Zeilen / 5x7
    lcd_command( LCD_SET_FUNCTION |
                 LCD_FUNCTION_4BIT |
                 LCD_FUNCTION_2LINE |
                 LCD_FUNCTION_5X7 );

    // Display ein / Cursor aus / Blinken aus
    lcd_command( LCD_SET_DISPLAY |
                 LCD_DISPLAY_ON |
                 LCD_CURSOR_OFF |
                 LCD_BLINKING_OFF);

    // Cursor inkrement / kein Scrollen
    lcd_command( LCD_SET_ENTRY |
                 LCD_ENTRY_INCREASE |
                 LCD_ENTRY_NOSHIFT );

    lcd_clear();
}

////////////////////////////////////////////////////////////////////////////////
// Sendet ein Datenbyte an das LCD
void lcd_data( uint8_t data )
{
	_data |= LCD_RS;    // RS auf 1 setzen

    lcd_out( data );            // zuerst die oberen,
    lcd_out( data<<4 );         // dann die unteren 4 Bit senden

    _delay_us( LCD_WRITEDATA_US );
}

////////////////////////////////////////////////////////////////////////////////
// Sendet einen Befehl an das LCD
void lcd_command( uint8_t data )
{
	_data &= ~LCD_RS;    // RS auf 0 setzen

    lcd_out(data);             // zuerst die oberen,
    lcd_out(data<<4);           // dann die unteren 4 Bit senden

    _delay_us( LCD_COMMAND_US );
}

////////////////////////////////////////////////////////////////////////////////
// Sendet den Befehl zur Löschung des Displays
void lcd_clear( void )
{
    lcd_command( LCD_CLEAR_DISPLAY );
    _delay_ms( LCD_CLEAR_DISPLAY_MS );
}

////////////////////////////////////////////////////////////////////////////////
// Sendet den Befehl: Cursor Home
void lcd_home( void )
{
    lcd_command( LCD_CURSOR_HOME );
    _delay_ms( LCD_CURSOR_HOME_MS );
}

////////////////////////////////////////////////////////////////////////////////
// Setzt den Cursor in Spalte x (0..15) Zeile y (1..4)

void lcd_setcursor( uint8_t x, uint8_t y )
{
    uint8_t data;

    switch (y)
    {
        case 0:    // 1. Zeile
            data = LCD_SET_DDADR + LCD_DDADR_LINE1 + x;
            break;

        case 1:    // 2. Zeile
            data = LCD_SET_DDADR + LCD_DDADR_LINE2 + x;
            break;

        case 2:    // 3. Zeile
            data = LCD_SET_DDADR + LCD_DDADR_LINE3 + x;
            break;

        case 3:    // 4. Zeile
            data = LCD_SET_DDADR + LCD_DDADR_LINE4 + x;
            break;

        default:
            return;                                   // für den Fall einer falschen Zeile
    }

    lcd_command( data );
}

////////////////////////////////////////////////////////////////////////////////
// Schreibt einen String auf das LCD

void lcd_string( const char *data )
{
    while( *data != '\0' )
        lcd_data( *data++ );
}

////////////////////////////////////////////////////////////////////////////////
// Schreibt ein Zeichen in den Character Generator RAM

void lcd_generatechar( uint8_t startadresse, const uint8_t *data )
{
    // Startposition des Zeichens einstellen
    lcd_command( LCD_SET_CGADR | (startadresse<<3) ); //Startadressen: 0;1;2;3;4;5;6;7

    // Bitmuster übertragen
    for ( uint8_t i=0; i<8; i++ )
    {
        lcd_data( data[i] );
    }
    lcd_command(LCD_SET_DDADR); //DRAM auf 0 setzen
}
