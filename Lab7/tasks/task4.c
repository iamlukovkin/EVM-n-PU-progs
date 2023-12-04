#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#define CLK 16000000
#define BAUD 9600
#define UBRR0_VALUE ((CLK / (BAUD * 16)) - 1)
#define VOLTAGE_THRESHOLD 128 // Пример порогового значения (может потребоваться настройка)

// Обработчик прерывания UART
ISR(USART_UDRE_vect) {
    // Пустая реализация, необходима для корректной работы библиотеки
}

int main(void) {
    // Настройка UART
    UBRR0H = (uint8_t)(UBRR0_VALUE >> 8);
    UBRR0L = (uint8_t)UBRR0_VALUE;
    UCSR0B = (1 << TXEN0); // Разрешение передачи
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01); // Длина слова 8 бит

    sei(); // Разрешение всех прерываний

    while (1) {
        // Проверка порога напряжения
        ADMUX = 0; // Установка входа ADC0 (младшие 4 бита)
        ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
        while (ADCSRA & (1 << ADSC))
            ; // Ожидание завершения преобразования

        if (ADC < VOLTAGE_THRESHOLD) {
            // Если ниже порога, отправляем сообщение "Low Voltage"
            printf_P(PSTR("Low Voltage\n"));
        } else {
            // Если выше порога, отправляем сообщение "Normal"
            printf_P(PSTR("Normal\n"));
        }
    }

    return 0;
}
