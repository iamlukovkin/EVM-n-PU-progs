#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h> // Добавлен заголовок для функции strtol
#include <stdbool.h>

char data_index, index_start, addr_dst, addr_src, length, data, end;
int data_length = 0;
char my_addr = 0x01;

void USART_Init() {
	UBRR0 = 103; // Настройка скорости передачи (9600 бит/с при F_CPU=16000000)
	
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0); // Включение приема, передачи и прерывания по приему
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Формат кадра: 8 бит данных, 1 стоп-бит
}

volatile char received_data[8]; // Буфер для принимаемых данных
volatile uint8_t data_index = 0;
volatile bool data_ready = false;

void USART_Transmit(char data) {
	while (!(UCSR0A & (1 << UDRE0))); // Ждем, пока UDR0 готов для передачи
	UDR0 = data;
}

ISR(USART_RX_vect) {
	char receivedByte = UDR0;
	if (data_index == 0 && receivedByte != 0xFF) {
		return;
		} else {
		data_ready = false;
		data_index ++;
	}
	if (data_index == 1) {
		if (receivedByte != my_addr) {
			return;
			} else {
			addr_dst = receivedByte;
			data_index ++;
		}
		} else if (data_index == 2) {
		addr_src = receivedByte;
		data_index ++;
		} else if (data_index == 3) {
		length = receivedByte;
		int arrayLength = (int)strtol(length, NULL, 16);
		int data_array[arrayLength] = {};
		array_length --;
		int counter = 0;
		data_index ++;
		} else if (data_index == 4) {
		if (counter != arrayLength) {
			data_array[counter] = receivedByte;
			counter ++;
			} else {
			data_index ++;
		}
		} else if (data_index == 5 && receivedByte != 0x80) {
		end = receivedByte;
		data_index = 0;
		data_ready = true;
		data_length = arrayLength + 5;
		} else {
		return;
	}
}

int main() {
	// Настройка порта B5 (светодиода) как вывода
	DDRB |= (1 << DDB5);

	// Инициализация USART
	USART_Init();

	// Разрешение глобальных прерываний
	sei();

	while (1) {
		if (data_ready) {
			data_ready = false;
			for (int i = 0; i < data_length; i++) {
				USART_Transmit(received_data[i]);
			}
		}

		return 0;
	}
}
