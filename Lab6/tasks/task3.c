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

void LED_On() {
    PORTB |= (1 << PB5); // Включить светодиод L (PortB5)
}

void LED_Off() {
    PORTB &= ~(1 << PB5); // Выключить светодиод L (PortB5)
}

ISR(USART_RX_vect) {
    char receivedByte = UDR0; // Получить принятый байт

    // Определите условие, при котором нужно включить или выключить светодиод L
    if (receivedByte == 'e') {
        LED_On(); // Включить светодиод L
		_delay_ms(333);
    } else {
        LED_Off(); // Выключить светодиод L
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
        // Основной код, можно оставить пустым
    }

    return 0;
}
