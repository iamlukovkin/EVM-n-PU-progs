# Имя программы и собранного бинарника
TARGET = habr_2

# путь к каталогу с GCC
AVRCCDIR = /usr/bin/

#само название компилятора, мало ли, вдруг оно когда-нибудь поменяется
CC = avr-gcc
OBJCOPY = avr-objcopy

# каталог в который будет осуществляться сборка, что бы не засерать остальные каталоги
BUILD_DIR = build

# название контроллера для компилятора
MCU = atmega328p

#флаги для компилятора 
OPT = -Os
C_FLAGS = -mmcu=$(MCU) $(OPT) -Wall

# параметры для AVRDUDE
DUDE_MCU = m328p
PORT = /dev/ttyUSB0
PORTSPEED = 115200

# DEFINы
DEFINES = \
-D__AVR_ATmega328P__ \
-DF_CPU=16000000UL

# пути к заголовочным файлам
C_INCLUDES =  \
-I/usr/lib/avr/include \
-Iinc

# файлы программы
C_SOURCES = \
src/habr_2.c \
src/functions.c 

# служебные переменные
OBJ_FILES = $(C_SOURCES:.c=.o)
ASM_FILES = $(C_SOURCES:.c=.s)
OUT_OBJ = $(addprefix $(BUILD_DIR)/, $(notdir $(OBJ_FILES)))

# правила для сборки

all: $(TARGET).hex

$(TARGET).hex: $(TARGET).elf
	$(AVRCCDIR)$(OBJCOPY) -j .text -j .data -O ihex $(BUILD_DIR)/$< $(BUILD_DIR)/$@

$(TARGET).elf: $(OBJ_FILES) $(ASM_FILES)
	mkdir -p $(BUILD_DIR)
	$(AVRCCDIR)$(CC) $(C_FLAGS) $(DEFINES) $(OUT_OBJ) -o $(BUILD_DIR)/$@

%.o: %.c
	echo $^
	$(AVRCCDIR)$(CC) -c $(C_FLAGS) $(DEFINES) $(C_INCLUDES) $< -o $(BUILD_DIR)/$(@F)

%.s: %.c
	echo $^
	$(AVRCCDIR)$(CC) -S -g3 $(C_FLAGS) $(DEFINES) $(C_INCLUDES) $< -o $(BUILD_DIR)/$(@F)

clean:
	rm -f $(BUILD_DIR)/*

prog: $(TARGET).hex
	avrdude -p $(DUDE_MCU) -c arduino -P $(PORT) -b $(PORTSPEED) -U flash:w:$(BUILD_DIR)/$(TARGET).hex

read_eeprom:
	avrdude -p $(DUDE_MCU) -c arduino -P $(PORT) -b $(PORTSPEED) -U eeprom:r:eeprom.hex:i

write_eeprom: eeprom.hex
	avrdude -p $(DUDE_MCU) -c arduino -P $(PORT) -b $(PORTSPEED) -U eeprom:w:eeprom.hex