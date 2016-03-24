#General compiler and linker stuff
CC=avr-gcc#which compiler to use
CFLAGS=-O$(OPT) -g -std=gnu99 -Wall -Wstrict-prototypes -ffunctions-sections -fdata-sections#compiler flags to use
OBJCOPY=avr-objcopy#what version of objcopy to use to change formats
TARGET_ARCH=-mmcu=$(MCU)#compiler flag for target uC name come from MCU below
LDFLAGS=-Wl,-Map,$(TARGET).map -Wl,--gc-sections#linker flags
OPT=s#s,0,1,2,3

#Project files and output
SRC=main.c#source files
TARGET=Blink#name of output
OBJECTS=#object files to include
INC=#extra directories to search for headers
INC_PARAMS=$(foreach d, $(INC), -I$d)#prepends a -I to each item in INC

#Chip and project global definitions
MCU=atmega328p#compiler name of target chip
F_CPU=1000000UL#uC clock
BAUD=9600UL#uC baud
CPPFLAGS=-DF_CPU$(F_CPU) -DBAUD$(BAUD) -I.#Flags

#Flashing options
FLASHMETHOD=arduino#linuxgpio #name of programmer in avrdude
FLASHPORT=/dev/ttyACM0#what port avrdude should use
FLASHBAUD=19200#baudrate that avrdude will flash at
CHIP=m328p#avrdude name of target chip

#Rules
all: flash

flash: hex
	avrdude -c $(FLASHMETHOD) -P $(FLASHPORT) -b $(FLASHBAUD) -p $(CHIP) -U flash:w:$(TARGET).hex

hex: elf
	$(OBJCOPY) -j .text -j .data -O ihex $(TARGET).elf $(TARGET).hex

elf: object $(OBJECTS)
	$(CC) $(LDFLAGS) $(TARGET_ARCH) $^ -o $(TARGET).elf

object: $(SRC)
	$(CC) $(CFLAGS) $(INC) $(TARGET_ARCH) $^ -o $(TARGET)

clean:
	rm -f $(TARGET).elf $(TARGET).hex $(TARGET).map
	
size: hex
	avr-size -B -x -t $(TARGET).hex
