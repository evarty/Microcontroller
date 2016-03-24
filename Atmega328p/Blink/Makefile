#General compiler and linker stuff
CC=avr-gcc#which compiler to use
CFLAGS=-Os -g -std=gnu99 -Wall -ffunctions-sections -fdata-sections#compiler flags to use
OBJCOPY=avr-objcopy#what version of objcopy to use to change formats
TARGET_ARCH=-mmcu=$(MCU)#compiler flag for target uC name come from MCU below
LDFLAGS=-Wl,-Map,$(TARGET).map -Wl,--gc-sections#linker flags

#Project files and output
SRC=main.c#source files
TARGET=Blink#name of output
OBJECTS=#object files to include

#Chip and project global definitions
MCU=atmega328p#compiler name of target chip
F_CPU=1000000UL#uC clock
BAUD=9600UL#uC baud
CPPFLAGS=-DF_CPU$(F_CPU) -DBAUD$(BAUD) -I#Flags

#Flashing options
FLASHMETHOD=arduino#name of programmer in avrdude
FLASHPORT=/dev/ttyACM0#what port avrdude should use
FLASHBAUD=19200#baudrate that avrdude will flash at
CHIP=m328p#avrdude name of target chip

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
