#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int ledState = 0;

int main(void) {
    // Настройка портов и переменных
    DDRD = 0x03; // Установить PD0 и PD1 как выходы
    PORTD |= (1 << PD2); // Включить внутренний pull-up резистор для PD2 (кнопка)
    int ledState = 0; // Состояние светодиодов

    // Настройка прерывания INT0 (PD2)
    EICRA = (1 << ISC00) | (1 << ISC01); // Прерывание по фронту (переднему фронту)
    EIMSK = (1 << INT0); // Разрешить прерывание INT0

    sei(); // Разрешить глобальные прерывания

    while (1) {
        // Отобразить текущее состояние светодиодов на PD0 и PD1
        PORTD = (ledState << PD0);
    }

    return 0;
}

// Прерывание INT0
ISR(INT0_vect) {
    // Переключение состояния светодиодов
    ledState = 1 - ledState;
}
