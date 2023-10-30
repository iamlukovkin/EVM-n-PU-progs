#include <avr/io.h>
#include <avr/interrupt.h>

char x, y, temp, count;

ISR(PCINT0_vect) {
    x = PINB;
    y = x ^ temp;
    temp = x;
}

ISR(PCINT2_vect) {
    x = PIND;
    y = x ^ temp;
    temp = x;
}

int main(void) {
    x=0;
    PCICR = 0x07;
    PCMSK0 = 0xFF;
    PCMSK2 = 0xFF;
    sei();
    while (1)
    {
        PORTC = count;
        count++;
    }
}
