#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/wdt.h>

void USART_Init() {
    UBRR0 = 103; 
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0); 
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); 
}

void perform_full_reset() {
    cli();
    wdt_disable();
    asm volatile("jmp 0");
}

volatile char received_data[255]; // Buffer for received data
volatile int data_index = 0;
volatile bool data_ready = false;
volatile bool error_flag = false;
char my_adr[] = {'0', '1'};
long data_length;
volatile int data_counter = 0;

void USART_Transmit(char data) {
    while (!(UCSR0A & (1 << UDRE0))); // Wait until UDR0 is ready for transmission
    UDR0 = data;
}

ISR(USART_RX_vect) {
    char receivedByte = UDR0;
    if (data_index == 0) {
		if (receivedByte != 'F') {
			error_flag = true;
			perform_full_reset();
			return;
		}
    } else if (data_index == 1) {
		if (receivedByte != 'F') {
			error_flag = true;
			perform_full_reset();
			return;
		}
    } else if (data_index == 2) {
        if (receivedByte != my_adr[0]) {
            error_flag = true;
            perform_full_reset();
			return;
        } 
    } else if (data_index == 3) {
		if (receivedByte != my_adr[1]) {
			error_flag = true;
			perform_full_reset();
			return;
		}
	} else if (data_index == 6) {
		data_length = strtol(&receivedByte, NULL, 16) << 4; 
	} else if (data_index == 7) {
		data_length |= strtol(&receivedByte, NULL, 16);  
		data_length *= 2;
    } else if (data_index >= 8 && data_counter < data_length) {
        received_data[data_counter] = receivedByte;
        data_counter++;
		data_index = 9;	
    } else if ( data_index == 10) {
		if (receivedByte != '8') {
			error_flag = true;
			perform_full_reset();
			return;
		}
	} else if (data_index == 11) {
		if (receivedByte != '0') {
			error_flag = true;
			perform_full_reset();
			return;
		} else {
			data_ready = true;
		}
	}
	data_index++;
}


int main() {
    DDRB |= (1 << DDB5);
    USART_Init();
    sei();
    while (1) {
        if (data_ready && !error_flag) {
			for (int i = 0; i < data_length; i++) {
				USART_Transmit(received_data[i]);
			}
			USART_Transmit('\n');
			data_index = 0;
            data_ready = false;
		}
	}

    return 0;
}
