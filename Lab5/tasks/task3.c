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
    x = 0;
    PCICR = (1 << PCIE0) | (1 << PCIE2);
    PCMSK0 = (1 << PCINT1);
    PCMSK2 = (1 << PCINT22); 
    sei();
    while (1) {
        char highBit = (count >> 7) & 1;
        PORTB = (PORTB & ~(1 << PORTB5)) | (highBit << PORTB5);
        count++;
    }
}
