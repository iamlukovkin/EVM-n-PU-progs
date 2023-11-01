#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>

void USART_Init() {
    UBRR0 = 103; // Set the baud rate to 9600 (for F_CPU=16000000)

    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0); // Enable reception, transmission, and receive interrupt
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data, 1 stop bit
}

volatile char received_data[255]; // Buffer for received data
volatile int data_index = 0;
volatile bool data_ready = false;
char my_adr[] = {'0', '1'};
volatile bool data_getting = false;
volatile int data_length = 0x00;
volatile int counter = 0;

void USART_Transmit(char data) {
    while (!(UCSR0A & (1 << UDRE0))); // Wait until UDR0 is ready for transmission
    UDR0 = data;
}

void LED_On() {
    PORTB |= (1 << PB5); // Включить светодиод L (PortB5)
}
void LED_Off() {
    PORTB &= ~(1 << PB5); // Выключить светодиод L (PortB5)
}

ISR(USART_RX_vect) {
    char receivedByte = UDR0; // Get the received byte
	if (data_index == 0 && receivedByte != 'F') {
		return;
	} else if (data_index == 1 && receivedByte != 'F') {
		return;
	} else if (data_index == 2 && receivedByte != my_adr[0]) {
		return;
	} else if (data_index == 3 && receivedByte != my_adr[1]) {
		return;
	} else if (data_index == 6) {
		data_length += receivedByte * 0x10;
	} else if (data_index == 7) {
		data_length += receivedByte;
		char received_data[data_length];
		data_length --;
	} else if (data_index >= 8) {
		if (data_length >= 0) {
			received_data[data_length] = receivedByte;
			data_length --;
		} else {
			data_ready = true;
		}
	}

	data_index ++;
}

int main() {
    // Set up port B5 (LED) as an output
    DDRB |= (1 << DDB5);

    // Initialize USART
    USART_Init();

    // Enable global interrupts
    sei();

    
    while (1) {
        if (data_ready) {
            // Data is ready, send it
            for (int i = 0; i <= data_length; i++) {
                USART_Transmit(received_data[i]);
            }
            data_ready = false; // Reset the flag
        }
    }

    return 0;
}
