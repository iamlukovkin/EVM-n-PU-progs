#include <avr/io.h>
#include <util/delay.h>

#define LED_L (1 << PB0)
#define LED_RX (1 << PB1)
#define LED_TX (1 << PB2)

void setupADC() {
    ADMUX = (1 << REFS0); // Vопорное = Vпитания
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    // Включение АЦП, частота - CLK/128
}

uint8_t readADC() {
    ADCSRA |= (1 << ADSC); // Запуск преобразования
    while (ADCSRA & (1 << ADSC))
        ; // Ожидание завершения преобразования
    return ADCH; // Возвращаем старший байт результата
}

void turnOnLEDs(uint8_t value) {
    PORTB &= ~(LED_L | LED_RX | LED_TX); // Выключаем все светодиоды

    if (value >= 21 && value <= 100) {
        PORTB |= LED_L; // Зажигаем светодиод L
    } else if (value >= 101 && value <= 180) {
        PORTB |= LED_L | LED_RX; // Зажигаем светодиоды L, RX
    } else if (value >= 181 && value <= 255) {
        PORTB |= LED_L | LED_RX | LED_TX; // Зажигаем все светодиоды
    }
}

int main(void) {
    DDRC &= ~(1 << PC0); // Настройка PC0 (ADC0) как входа для потенциометра
    DDRB |= LED_L | LED_RX | LED_TX; // Настройка PB0, PB1, PB2 как выходы для светодиодов

    setupADC();

    while (1) {
        uint8_t adcValue = readADC(); // Читаем значение с потенциометра
        turnOnLEDs(adcValue); // Управляем светодиодами в соответствии с прочитанным значением
        _delay_ms(100); // Задержка для стабилизации отображения
    }

    return 0;
}
