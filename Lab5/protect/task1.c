#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void USART_Init() {
    // Настройка скорости передачи (9600 бит/с при F_CPU=16000000)
    UBRR0 = 103;
    
    // Включение приема и разрешение прерывания по приему
    UCSR0B = (1 << RXEN0) | (1 << RXCIE0);
    
    // Формат кадра: 8 бит данных, 1 стоп-бит
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void USART_Transmit(char data) {
    while (!(UCSR0A & (1 << UDRE0))); // Ожидание, пока буфер передачи пуст
    UDR0 = data; // Запись данных в буфер передачи
}

void LED_On() {
    PORTB |= (1 << PB5); // Включить светодиод L (PortB5)
}

void LED_Off() {
    PORTB &= ~(1 << PB5); // Выключить светодиод L (PortB5)
}

ISR(USART_RX_vect) {
    char receivedByte = UDR0; // Получить принятый байт
    const char message[] = "FF0101010001020480\n";
    // Определите условие, при котором нужно включить или выключить светодиод L
    if (receivedByte == 'e') {
        USART_Transmit(message);
        LED_On(); // Включить светодиод L
    } else {
        LED_Off(); // Выключить светодиод L
    }
}

int main() {
    DDRB |= (1 << DDB5);
    USART_Init();
    sei();

    while (1) {
    }

    return 0;
}
