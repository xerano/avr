.include "../../include/m8def.inc"

.def temp = r16

.def delay_0 = r17
.def delay_1 = r18
.def counter = r19

.org INT_VECTORS_SIZE

init:
	ldi r24,high(RAMEND)     ;Stack Pointer setzen 
	out SPH,r24              ; "RAMEND" ist in m8def.inc (s.o.) festgelegt
	ldi r24,low(RAMEND)      ; 
	out SPL,r24              ;

	ldi	temp,	(1 << PB1)
	out	DDRB,	temp

	ldi	temp,	(1 << COM1A1) | (1 << WGM10)
	out	TCCR1A,	temp
	
	ldi	temp,	(1 << WGM12) | (1 << CS12) | (1 << CS10)
	out	TCCR1B,	temp

	ldi	temp,	0xff
	out	OCR1AH,	temp
	ldi	temp,	0x0f
	out	OCR1AL,	temp

	ldi	counter, 0x00

loop:
	count_up:
		ldi	delay_0, 228
		ldi	delay_1, 104
	
		out	OCR1AL, counter
		inc	counter

		rcall	delay
		cpi	counter, 0xff
		brne	count_up

	count_down:
		ldi	delay_0, 228
		ldi	delay_1, 104
		
		out	OCR1AL, counter
		dec	counter

		rcall	delay
		cpi	counter, 0x0
		brne	count_down
		

	rjmp	loop


delay:
	dec	delay_0
	brne	delay
	dec	delay_1
	brne	delay
	ret
