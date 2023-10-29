#include <avr/io.h>  // Подключение необходимых библиотек

uint32_t x = 0;      // Объявление переменной (счётчика) - x
int flag = 0;        // Флаг для анализа текущего состояния

int main(void) {     // Вход в программу
    DDRB = 0x20;     // Настройка порта D (0) на вывод информации

    while (1) {      // Начало бесконечного цикла (основной код программы)
        if (x == 800000 / 4) {  // Если счётчик досчитал до 800000
            if (flag == 0) {
                PORTB = 0x20;  // Включение светодиода, флаг = 1
                flag = 1;
            } else {
                PORTB = 0x00;  // Выключение светодиода, флаг = 0
                flag = 0;
            }
            x = 0;            // Обнуление x
        }
        x++;                 // Инкремент значения x
    }

    return 0;
}
