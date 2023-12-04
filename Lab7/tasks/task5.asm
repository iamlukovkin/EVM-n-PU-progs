.include "m328pdef.inc"
.def temp=r16			;объявление логического имени temp для регистра r16
.def adcvL=r17			;объявление логического имени adcvL для регистра r17
.def adcvH=r18			;объявление логического имени adcvH для регистра r18
.equ CLK=16000000		;тактовая частота генератора 
.equ BAUD=9600 		;скорость передачи данных UART
.equ UBRR0_value = (CLK/(BAUD*16)) - 1 ;подготовка константы для UBRR
.org 0				;вектор прерывания reset		
	jmp Reset		;переход к метке reset
.org 0x002A			; вектор прерывания АЦП
	jmp ADC_conv		;переход к метке ADC_conv
Reset:				;точка входа в программу
	ldi temp, (1<<REFS0)|(1<<ADLAR) ;Vопорное = Vпитания, выравнивание результата 
;преобразования по «левому краю» старшие 8бит будут записаны ADCH, канал ADC0
	sts ADMUX,temp		;запись значения в ADMUX
	ldi temp,(1<<ADEN)|(1<<ADSC)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0) ;
;включение АЦП, циклическое преобразование, разрешение прерывания, частота - CLK/128
	sts ADCSRA,temp	; запись значения в ADCSRA
	ldi temp, high(UBRR0_value)	;старшая часть константы для UBBR в temp	
	sts UBRR0H, temp 	;загрузка значения в UBRR0H
	ldi temp, low(UBRR0_value) 	; младшая часть константы для UBBR в temp
	sts UBRR0L, temp	;загрузка значения в UBRR0L
	ldi temp, (1<<TXEN0)	;разрешение передачи UART        
	sts UCSR0B,temp	;запись значения в UCSR0B
	ldi temp,(1<< UCSZ00)|(1<< UCSZ01)	;размер пакета данных 
	sts UCSR0C,temp	; запись значения в UCSR0С             
	sei			;глобальное разрешение прерываний
main: jmp main
;---обработчик прерывания АЦП ------
ADC_conv:
	lds adcvH, ADCH	;чтение значения регистра ADCH в регистр r18
repeat: 		;метка repeat обеспечивает цикл ожидания готовности UART к передаче
	lds r17, UCSR0A	;чтение содержания регистра
	bst r17, 5		;запись 5 бита (UDRE) регистра UCSR0A в бит t регистра SREG  
	brtc repeat 		;если t=0 – возврат к метке repeat
	sts UDR0, adcvH	;передача старших 8бит АЦП по UART
	reti			;возврат из прерывания
;---обработчик прерывания АЦП ------
