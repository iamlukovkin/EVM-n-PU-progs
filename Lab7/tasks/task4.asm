.include "m328Pdef.inc"
.equ CLK=16000000
.equ BAUD=9600
.equ UBRR0_value = (CLK/(BAUD*16)) - 1
.equ VOLTAGE_THRESHOLD = 128 ; Пример порогового значения (может потребоваться настройка)

.org 0
    jmp Reset

Reset:
    ; Настройка UART
    ; Установка частоты приема/передачи
    ldi r16, high(UBRR0_value)
    sts UBRR0H, r16
    ldi r16, low(UBRR0_value)
    sts UBRR0L, r16
    ; Разрешение передачи
    ldi r16, (1 << TXEN0)
    sts UCSR0B, R16
    ; Длина слова 8 бит
    ldi r16, (1 << UCSZ00) | (1 << UCSZ01)
    sts UCSR0C, R16
    sei ; Разрешение всех прерываний

main:
    ldi zl, low(NorV << 1); Метка указывает на слово (16 бит) в памяти,
                           ; для правильного чтения необходим сдвиг на 1 бит влево,
                           ; младший бит указывает на часть слова
    ldi zh, high(NorV << 1); 0 - младший байт, 1 - старший
    ldi r18, 0x07; Количество байт в сообщении

send:
    lpm r16, Z+ ; Чтение символа слова из таблицы (ПЗУ)
    sts UDR0, r16 ; Отправка символа по UART
    subi r18, 0x01 ; Вычитание 1 из счетчика букв

repeat:
    lds r17, UCSR0A; Чтение содержания регистра
    bst r17, 5; Запись 5 бита (UDRE) регистра (UCSR0A) в бит T регистра SREG
    brtc repeat; Если T = 0

    ; Проверка порога напряжения
    lds r16, ADMUX ; Чтение регистра ADMUX (регистр настройки аналогового мультиплексора)
    cpi r16, VOLTAGE_THRESHOLD ; Сравнение с пороговым значением
    brcs low_voltage ; Если ниже порога, переходим к обработке "Low Voltage"

normal_voltage:
    ; Если выше порога, отправляем сообщение "Normal"
    cpi r18, 0x00 ; r18 = 0?
    breq main ; Если да, то повторяем сообщение
    jmp send ; Если нет, то следующий символ

low_voltage:
    ; Если ниже порога, отправляем сообщение "Low Voltage"
    ldi zl, low(LowV << 1); Переключаем указатель на сообщение "Low Voltage"
    ldi zh, high(LowV << 1);
    rjmp send ; Переходим к отправке сообщения

.cseg
; Работа с ПЗУ
LowV: .db "Low Voltage", '\n' ; Размещение сообщения в памяти ПЗУ
NorV: .db "Normal", '\n' ; Размещение сообщения в памяти ПЗУ
