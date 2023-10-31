#include <avr/io.h> // ����������� ���������� � ���������������� (�������) ���������
#include <avr/interrupt.h> // ����������� ���������� ��� ������ � ������������
#include <util/delay.h>

int x;
ISR(INT1_vect) {
	PORTC = 0x00;
	_delay_ms(333);  // �������� 333 ����������� (1/3 �������)
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