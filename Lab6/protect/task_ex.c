#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdbool.h>

char data_index, addr_dst, addr_src, length, end;
int data_length = 0;
char my_addr = 0x01;

void USART_Init() {
    UBRR0 = 103;

    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

volatile char received_data[8];
volatile uint8_t data_index = 0;
volatile bool data_ready = false;

void USART_Transmit(char data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

ISR(USART_RX_vect) {
    char receivedByte = UDR0;
    if (data_index == 0 && receivedByte != 0xFF) {
        return;
    } else {
        data_ready = false;
        data_index++;
    }
    if (data_index == 1) {
        if (receivedByte != my_addr) {
            return;
        } else {
            addr_dst = receivedByte;
            data_index++;
        }
    } else if (data_index == 2) {
        addr_src = receivedByte;
        data_index++;
    } else if (data_index == 3) {
        length = receivedByte;
        int arrayLength = (int)strtol(&length, NULL, 16);
        int data_array[arrayLength];
        data_length = arrayLength + 4; // Длина данных, включая addr_dst, addr_src, length и end
        data_index++;
    } else if (data_index > 3 && data_index <= data_length) {
        received_data[data_index - 4] = receivedByte;
        data_index++;
    } else if (data_index == data_length + 1) {
        end = receivedByte;
        data_index = 0;
        data_ready = true;
    } else {
        return;
    }
}

int main() {
    DDRB |= (1 << DDB5);
    USART_Init();
    sei();

    while (1) {
        if (data_ready) {
            data_ready = false;
            // Вернуть данные в монитор порта
            for (int i = 0; i < data_length; i++) {
                USART_Transmit(received_data[i]);
            }
        }
    }

    return 0;
}