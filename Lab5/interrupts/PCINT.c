#include <avr/io.h>
#include <avr/interrupt.h>

void setupPCINT() {
    // Настройка прерываний на PCINT0 и PCINT2
    PCICR = 0x07;
    PCMSK0 = 0xFF;
    PCMSK2 = 0xFF;
    sei();
}
