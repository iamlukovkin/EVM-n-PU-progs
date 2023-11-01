#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h> // Добавлен заголовок для функции strtol
#include <stdbool.h>


void USART_Init() {
	UBRR0 = 103; // Настройка скорости передачи (9600 бит/с при F_CPU=16000000)
	
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0); // Включение приема, передачи и прерывания по приему
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Формат кадра: 8 бит данных, 1 стоп-бит
}

volatile char received_data[5]; // Буфер для принимаемых данных
volatile uint8_t data_index = 0;
volatile bool data_ready = false;

void USART_Transmit(char data) {
	while (!(UCSR0A & (1 << UDRE0))); // Ждем, пока UDR0 готов для передачи
	UDR0 = data;
}

ISR(USART_RX_vect) {
	char receivedByte = UDR0; // Получить принятый байт
	
	if (receivedByte == '!') {
		data_index = 0; // Сброс индекса для приема нового пакета
		data_ready = false; // Сброс флага
	} else if (receivedByte == '\n') { // Используем '\n' для завершения пакета
		data_ready = true; // Пакет завершен
	} else {
		received_data[data_index] = receivedByte;
		data_index++;
		if (data_index >= 4) {
			data_ready = true; // Приняты 4 числа
		}
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
			// Приняты 4 числа в формате a,b,c,d, обработаем их
			data_ready = false; // Сброс флага
			char* ptr;
			long a = strtol(received_data, &ptr, 10);
			long b = strtol(ptr + 1, &ptr, 10);
			long c = strtol(ptr + 1, &ptr, 10);
			long d = strtol(ptr + 1, NULL, 10);

			// Отправим числа в порядке b,a,d,c
			char send_buffer[20];
			sprintf(send_buffer, "!%ld,%ld,%ld,%ld\n", b, a, d, c);

			// Отправка данных
			for (int i = 0; send_buffer[i] != '\0'; i++) {
				USART_Transmit(send_buffer[i]);
			}
		}
	}

	return 0;
}
