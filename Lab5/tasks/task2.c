#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int ledState = 0;

// Прерывание INT0
ISR(INT0_vect) {
    // Переключение состояния светодиода AT1
    ledState = 1 - ledState;
}

// Прерывание INT1
ISR(INT1_vect) {
    // Переключение состояния светодиода AT2
    ledState = 1 - ledState;
}

int main(void) {
    // Настройка портов и переменных
    DDRD = 0x03; // Установить PD0 и PD1 как выходы
    PORTD |= (1 << PD2); // Включить внутренний pull-up резистор для PD2 (кнопка)

    // Настройка прерывания INT0 (PD2)
    EICRA = (1 << ISC01); // Прерывание по переднему фронту
    EIMSK = (1 << INT0); // Разрешить прерывание INT0

    // Настройка прерывания INT1 (PD3)
    EICRA |= (1 << ISC11); // Прерывание по переднему фронту
    EIMSK |= (1 << INT1); // Разрешить прерывание INT1

    sei(); // Разрешить глобальные прерывания

    while (1) {
        // Отобразить текущее состояние светодиодов AT1 и AT2 на PD0 и PD1
        PORTD = (ledState << PD0);
    }

    return 0;
}
