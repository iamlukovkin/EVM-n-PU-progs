#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

volatile uint8_t received_data[10]; // ������ ��� �������� �������� ������
volatile uint8_t data_index = 0;
volatile bool data_ready = false;
volatile uint8_t data_length = 0;
volatile int data_counter = 0;

void USART_Init() {
	UBRR0 = 103; // ��������� �������� �������� (9600 ���/� ��� F_CPU=16000000)

	UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0); // ��������� ���������, ����������� � ���������� �� ������
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // ������ �����: 8 ��� ������, 1 ����-���
}

void USART_Transmit(uint8_t data) {
	while (!(UCSR0A & (1 << UDRE0))); // �������� ���������� UDR0 ��� ��������
	UDR0 = data;
}

ISR(USART_RX_vect) {
	uint8_t receivedByte = UDR0;
	if (data_index == 0) {
        if (receivedByte != '8') {
            error_flag = true;
            perform_full_reset();
        }
	} else if (data_index == 1) {
		if (receivedByte != '0') { 
			error_flag = true;
            perform_full_reset();
        }
    } else if (data_index == 2) {
        data_length = strtol(&receivedByte, NULL, 16) << 4; 
    } else if (data_index == 3) {
        data_length |= strtol(&receivedByte, NULL, 16);  
        if (data_length < 1 || data_length > 10) {
            error_flag = true;
            perform_full_reset();
        } else {
            data_length *= 2;
        }
    } else if (data_index >= 4 && data_counter < data_length) {
        received_data[data_counter] = receivedByte;
        data_counter++;
        data_index = 5;
    } else if (data_index == 6) {
		if (receivedByte != "8") {
			error_flag = true;
            perform_full_reset();
			return;
        } 
	} else if (data_index == 7) {
        if (receivedByte != "F") {
            error_flag = true;
            perform_full_reset();
            return;
        } else {
			data_index = 0;
			data_ready = true;
		}
    }
    data_index++;
}

int main() {
	DDRB |= (1 << DDB5); // ��������� ����� B5 (�����, ��� � �������)

	USART_Init();

	sei();

	while (1) {

        if (data_ready && !error_flag) {
			for (int i = 0; i < data_length; i++) {
				USART_Transmit(received_data[i]);
			}
			USART_Transmit('\n');
            data_ready = false;
		}
	}

	return 0;
}
