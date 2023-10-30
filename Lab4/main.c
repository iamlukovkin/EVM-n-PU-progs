#include <avr/io.h>
#include <tasks/tasks.h>

int main(void)
{
	DDRB = 1 << PB5;
	func();

	return 0;
}