# Simple makefile to compile test programs for rose simulator
# Created by Carmelo J Fernandez-Agüera Tortosa (a.k.a. Technik)
# On March 22nd, 2012

#---- Global variables ----
ATMEGA_TOOLCHAIN := C:/Atmel/AVR Tools/AVR Toolchain/bin
ROSE_WORKING_DIR := C:/Users/technik/programming/rose/simulator/bin/windows/Debug

AS := $(ATMEGA_TOOLCHAIN)/avr-as
CC := $(ATMEGA_TOOLCHAIN)/avr-gcc
CXX := $(ATMEGA_TOOLCHAIN)/avr-g++
OBJCOPY := $(ATMEGA_TOOLCHAIN)/avr-objcopy
OBJDUMP := $(ATMEGA_TOOLCHAIN)/avr-objdump
READELF := $(ATMEGA_TOOLCHAIN)/avr-readelf

#----Project specific variables ----
PRJ_NAME := pinBlinker

CPP_SRC := main.cpp

#---- Internal variables ----
CPP_FLAGS := -fno-access-control -fno-enforce-eh-specs -fno-rtti -g0 -O1 -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega2560 -DF_CPU=16000000L
OUT_NAME := $(PRJ_NAME)
OUTPUT := $(OUT_NAME).hex

#---- Rules ----
all: $(OUTPUT)

%.hex : %.elf
	$(OBJCOPY) -O ihex -R .eeprom -R .fuse -R .lock -R .signature \
	$^ $@

$(OUT_NAME).elf : $(CPP_SRC)
	$(CXX) -o $@ $^ $(CPP_FLAGS)
	
readelf: $(OUT_NAME).elf
	$(READELF) -a $^
	
objdump: $(OUTPUT)
	$(OBJDUMP) -C -d -f -F -h $^
	
assembly: $(OUT_NAME).S

$(OUT_NAME).S : $(CPP_SRC)
	$(CXX) -S -o $@ $^ $(CPP_FLAGS)
	
test.elf : test.S
	$(AS) -mmcu=atmega2560 -o $@ $^
	
assembleTest: test.hex

.PHONY: all readelf objdump assembleTest