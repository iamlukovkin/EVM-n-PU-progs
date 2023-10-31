#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VAL ((F_CPU / (16UL * BAUD)) - 1)

void USART_Init() {
    // Установка скорости передачи
    UBRR0H = (uint8_t)(UBRR_VAL >> 8);
    UBRR0L = (uint8_t)UBRR_VAL;
    
    // Разрешение передачи и разрешение прерывания UDRE
    UCSR0B = (1 << TXEN0) | (1 << UDRIE0);

    // Установка формата кадра: 8 бит данных, 1 стоп-бит
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void USART_Transmit(char data) {
    while (!(UCSR0A & (1 << UDRE0))); // Ожидание, пока буфер передачи пуст
    UDR0 = data; // Запись данных в буфер передачи
}

ISR(USART_UDRE_vect) {
    // Обработчик прерывания UDRE
    // После передачи байта, это прерывание сработает
    // Вы можете здесь отправить следующий байт или отключить прерывание UDRE
}

int main() {
    USART_Init();
    sei(); // Глобальное разрешение прерываний
    
    const char message[] = "Atmega328P:ReadyToWork\n";
    uint8_t index = 0;

    while (1) {
        if (message[index] != '\0') {
            USART_Transmit(message[index]);
            index++;
        }
    }

    return 0;
}
