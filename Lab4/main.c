#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h> // �������� ��������� ��� ������� strtol
#include <stdbool.h>


void USART_Init() {
	UBRR0 = 103; // ��������� �������� �������� (9600 ���/� ��� F_CPU=16000000)
	
	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0); // ��������� ������, �������� � ���������� �� ������
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // ������ �����: 8 ��� ������, 1 ����-���
}

volatile char received_data[5]; // ����� ��� ����������� ������
volatile uint8_t data_index = 0;
volatile bool data_ready = false;

void USART_Transmit(char data) {
	while (!(UCSR0A & (1 << UDRE0))); // ����, ���� UDR0 ����� ��� ��������
	UDR0 = data;
}

ISR(USART_RX_vect) {
	char receivedByte = UDR0; // �������� �������� ����
	
	if (receivedByte == '!') {
		data_index = 0; // ����� ������� ��� ������ ������ ������
		data_ready = false; // ����� �����
		} else if (receivedByte == '\n') { // ���������� '\n' ��� ���������� ������
		data_ready = true; // ����� ��������
		} else {
		received_data[data_index] = receivedByte;
		data_index++;
		if (data_index >= 4) {
			data_ready = true; // ������� 4 �����
		}
	}
}

int main() {
	// ��������� ����� B5 (����������) ��� ������
	DDRB |= (1 << DDB5);

	// ������������� USART
	USART_Init();

	// ���������� ���������� ����������
	sei();

	while (1) {
		if (data_ready) {
			// ������� 4 ����� � ������� a,b,c,d, ���������� ��
			data_ready = false; // ����� �����
			char* ptr;
			long a = strtol(received_data, &ptr, 10);
			long b = strtol(ptr + 1, &ptr, 10);
			long c = strtol(ptr + 1, &ptr, 10);
			long d = strtol(ptr + 1, NULL, 10);

			// �������� ����� � ������� b,a,d,c
			char send_buffer[20];
			sprintf(send_buffer, "!%ld,%ld,%ld,%ld\n", b, a, d, c);

			// �������� ������
			for (int i = 0; send_buffer[i] != '\0'; i++) {
				USART_Transmit(send_buffer[i]);
			}
		}
	}

	return 0;
}
