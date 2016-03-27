#General compiler and linker stuff
CC=arm-none-eabi-gcc#which compiler to use
CFLAGS=-O$(OPT) -g -std=gnu99 -Wall -Wstrict-prototypes -ffunctions-sections -fdata-sections#compiler flags to use
OBJCOPY=arm-none-eabi-objcopy#what version of objcopy to use to change formats
TARGET_ARCH=-mmcu=$(MCU)#compiler flag for target uC name come from MCU below
LDFLAGS=-Wl,-Map,$(TARGET).map -Wl,--gc-sections#linker flags
OPT=s#optimization level: s (size), 0 (off), 1, 2, 3
AR=arm-none-eabi-ar

#Project files and output
SRC=main.c#C source files, including files in other folders, "library" method suported
TARGET=Blink#name of output, change for project, or dont, doesnt matter
OBJECTS=#object files to include, for precompiled libraries
INC=#extra directories to search for headers, used for library headers
$(foreach d, $(INC), -I$d)#prepends a -I to each item in INC
LIBS=#Libraries to include
$(foreach d, $(LIBS), -l$d)#prepends a -l to each item in LIBS
LIBPATHS=#Paths to libraries to include
$(foreach d, $(LIBPATHS), -L$d)#prepends a -L to each item in LIBPATHS

#Chip and project global definitions
MCU=atmega328p#compiler name of target chip, avr-gcc take atmega328p, avrdude does not. Also used for avr-size
F_CPU=1000000UL#uC clock, for macros, sent to preprocessor
BAUD=9600UL#uC baud, for macros, sent to preprocessor
CPPFLAGS=-DF_CPU$(F_CPU) -DBAUD$(BAUD) -I.#Flags passed to preprocessor. This allows for macros and such to have the selected clock rate and baud universally without having to put it into each file

#Flashing options
FLASHMETHOD=arduino#linuxgpio #name of programmer in avrdude
FLASHPORT=/dev/ttyACM0#what port avrdude should use
FLASHBAUD=19200#baudrate that avrdude will flash at
CHIP=m328p#avrdude name of target chip, avrdude takes m328p as name, avr-gcc does not

#Rules
all: flash

flash: hex
	avrdude -c $(FLASHMETHOD) -P $(FLASHPORT) -b $(FLASHBAUD) -p $(CHIP) -U flash:w:$(TARGET).hex

hex: elf
	$(OBJCOPY) -j .text -j .data -O ihex $(TARGET).elf $(TARGET).hex

elf: object $(OBJECTS)
	$(CC) $(LDFLAGS) $(TARGET_ARCH) $^ -o $(TARGET).elf

object: $(SRC)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(INC) $(TARGET_ARCH) $^ -o $(TARGET).o

clean:
	rm -f $(TARGET).elf $(TARGET).hex $(TARGET).map $(TARGET).o
	
size: hex
	arm-none-eabi-size -c -d -t --mcu=$(MCU) $(TARGET).hex
