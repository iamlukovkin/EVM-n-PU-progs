#include <avr/io.h> // Подключение библиотеки с идентификаторами (именами) регистров
#include <avr/interrupt.h> // Подключение библиотеки для работы с прерываниями


ISR(PCINT2_vect) { // Объявление подпрограммы обработчика прерываний PCINT2
    // some actions
}

int main(void) { // начало основной программы
    PCMSK |= 0xAA; // Разрешение PCINT прерываний от выводов PD1, PD3, PD5, PD7
    PCICR |= (1 << PCIE2); // Разрешение прерываний PCINT2

    // some actions
 
    sei(); // глобальное разрешение прерываний
    while (1) { // бесконечный цикл, основная программа
        // some actions
    }
}

