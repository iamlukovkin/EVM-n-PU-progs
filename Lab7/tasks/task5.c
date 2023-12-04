#include <avr/io.h>
#include <avr/interrupt.h>

#define CLK 16000000
#define BAUD 9600
#define UBRR0_VALUE ((CLK / (BAUD * 16)) - 1)

volatile uint8_t adcvH; // Объявление переменной для хранения старших 8 бит АЦП

ISR(ADC_vect) {
    adcvH = ADCH; // Сохранение значения регистра ADCH
    UDR0 = adcvH; // Передача старших 8 бит АЦП по UART
}

void setupADC() {
    ADMUX = (1 << REFS0) | (1 << ADLAR); // Vопорное = Vпитания, выравнивание результата
    ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADATE) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    // Включение АЦП, автоматическое запуск преобразования, разрешение прерывания, частота - CLK/128
}

void setupUART() {
    UBRR0H = (uint8_t)(UBRR0_VALUE >> 8);
    UBRR0L = (uint8_t)UBRR0_VALUE;
    UCSR0B = (1 << TXEN0); // Разрешение передачи UART
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01); // Размер пакета данных
}

int main(void) {
    setupADC();
    setupUART();
    sei(); // Глобальное разрешение прерываний

    while (1) {
        // Основной цикл программы
    }

    return 0;
}
