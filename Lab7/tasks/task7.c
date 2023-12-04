#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#define CLK 16000000
#define BAUD 9600
#define UBRR0_VALUE ((CLK / (BAUD * 16)) - 1)

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

    // Настройка ADC для датчика температуры
    ADMUX = (1 << REFS1) | (1 << REFS0) | (1 << MUX3);
    ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    while (ADCSRA & (1 << ADSC))
        ; // Ожидание завершения преобразования

    while (1) {
        // Считывание значения температуры
        uint16_t adcValue = ADC;
        
        // Конвертация ADC в температуру (в примере использованы константы, потребуется корректировка)
        double temperature = ((adcValue - 273) * 0.1);

        // Отправка температуры по UART
        printf_P(PSTR("Temperature: %.1f degrees Celsius\n"), temperature);

        _delay_ms(1000); // Задержка для стабилизации отображения
    }

    return 0;
}
