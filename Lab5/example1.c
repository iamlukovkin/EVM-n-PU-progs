#include <avr/io.h> // Подключение библиотеки с идентификаторами (именами) регистров
#include <avr/interrupt.h> // Подключение библиотеки для работы с прерываниями
#include <util/delay.h>

int x;
ISR(INT1_vect) {
    PORTC = 0x00;
    _delay_ms(333);  // Задержка 333 миллисекунд (1/3 секунды)
}

int main(void) {
    x = 0;
    EICRA = 0x00;
    EIMSK = 0x02;
    DDRD = 0x00;
    PORTD = 0x08;
    DDRC = 0xFF;
    sei();
    while (1) {
        PORTC = x;
        x++;
    }
}