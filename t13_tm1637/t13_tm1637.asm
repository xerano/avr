.nolist
.include "tn13adef.inc"
.list

.equ DTA_CMD = 0x40
.equ DSP_CRTL_CMD = 0x80
.equ ADDR_CMD = 0xC0
.equ DIO = 1
.equ CLK = 0

.org 0x0000 
	rjmp RESET

RESET:
  ldi r16, 0x00
  out DDRB, r16
  out PORTB, r16

  rcall delay
  
  rcall start
  ldi r16, DTA_CMD
  rcall write_byte
  rcall stop

  rcall start
  ldi r16, ADDR_CMD
  rcall write_byte
  
  ldi ZL, LOW(2*segments)+1
;  ldi ZH, HIGH(2*segments)
  lpm r16, Z
  rcall write_byte

  ldi ZL, LOW(2*segments)+6
  lpm r16, Z
  rcall write_byte
  ldi ZL, LOW(2*segments)+6
  lpm r16, Z
  rcall write_byte
  ldi r16, 0b11111101 ; 6
  rcall write_byte

  rcall stop

  ldi r16, (DSP_CRTL_CMD | 0x08 | 0x02)
  rcall start
  rcall write_byte
  rcall stop

LOOP:
  rjmp LOOP

start:
  sbi DDRB, DIO
  rcall delay
  ret

stop:
  sbi DDRB, DIO
  rcall delay
  cbi DDRB, CLK
  rcall delay
  cbi DDRB, DIO
  rcall delay
  ret

write_byte:
  ldi r18, 8
write_byte_loop:
  ldi r17, 0x01

  sbi DDRB, CLK		; CLK 0
  rcall delay
  and r17, r16		; mask lsb
  brne dio_high		; if 0, set DIO input -> 1
  sbi DDRB, DIO		; if 1, set output -> 0
  rjmp next
dio_high:
  cbi DDRB, DIO
next:
  rcall delay
  cbi DDRB, CLK		; CLK 1
  rcall delay
  ror r16		; move next bit to LSB
  dec r18		; decrement loop counter
  brne write_byte_loop

  ; acknowledge
  sbi DDRB, CLK
  cbi DDRB, DIO
  rcall delay
  
  cbi DDRB, CLK
  rcall delay
  sbic PINB, DIO
  sbi DDRB, DIO

  rcall delay
  sbi DDRB, CLK
  rcall delay

  ret

delay:
    ldi  r20, 2
  L1: 
    dec  r20
    brne L1
    ret

segments: .db 0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110, 0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01101111, 0b01110111, 0b01111100, 0b00111001, 0b01011110, 0b01111001, 0b01110001 


