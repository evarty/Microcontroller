#General compiler and linker stuff
CC=avr-gcc
CFLAGS=-Os -g -std=gnu99 -Wall -ffunctions-sections -fdata-sections
OBJCOPY=avr-objcopy
TARGET_ARCH=-mmcu=$(MCU)
LDFLAGS=-Wl,-Map,$(TARGET).map -Wl,--gc-sections

#Project files and output
SRC=main.c
TARGET=Blink
OBJECTS=

#Chip and project global definitions
MCU=atmega328p
F_CPU=1000000UL
BAUD=9600UL
CPPFLAGS=-DF_CPU$(F_CPU) -DBAUD$(BAUD) -I

#Flashing options
FLASHMETHOD=arduino
FLASHPORT=/dev/ttyACM0
FLASHBAUD=19200
CHIP=m328p

#Rules
all: flash

flash: hex
	avrdude -c $(FLASHMETHOD) -P $(FLASHPORT) -b $(FLASHBAUD) -p $(CHIP) -U flash:w:$(TARGET).hex

hex: elf
	$(OBJCOPY) -j .text -j .data -O ihex $(TARGET).elf $(TARGET).hex

elf: $(OBJECTS) $(SRC)
	$(CC) $(LDFLAGS) $(TARGET_ARCH) $^ -o $(TARGET).elf

clean:
	rm -f $(TARGET).elf $(TARGET).hex $(TARGET).map
