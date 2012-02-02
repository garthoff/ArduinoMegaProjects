#-----------------------------------------------------------------------------------------------------------------------
# Project Double US: Panoramic 2D Ultrasound scanning
# Created by Carmelo J. Fdez-Agüera (a.k.a. Technik)
# on January 26th, 2012
#-----------------------------------------------------------------------------------------------------------------------
# makefile

# --- Variables ---

# -- Project configuration variables
PRJ_NAME:=cyclops
# -- End of project configuration

# -- Internal variables --
# Project
PRJ_ROOT = ../..
CODE_DIR = $(PRJ_ROOT)/src/code
PRJ_SRC:=	$(CODE_DIR)/hardware/clock.cpp \
			$(CODE_DIR)/hardware/hardware.cpp \
			$(CODE_DIR)/main.cpp
PRJ_OBJ:=$(PRJ_SRC:.cpp=.o)
OUTPUT_NAME = $(PRJ_ROOT)/bin/$(PRJ_NAME)
OUTPUT_FILE = $(OUTPUT_NAME).hex

# Build tools
AVR_CC = $(ARDUINO_HOME)/hardware/tools/avr/bin/avr-gcc
AVR_CXX = $(ARDUINO_HOME)/hardware/tools/avr/bin/avr-g++
AVR_AR = $(ARDUINO_HOME)/hardware/tools/avr/bin/avr-ar
AVR_OBJCPY = $(ARDUINO_HOME)/hardware/tools/avr/bin/avr-objcopy
AVRDUDE = $(ARDUINO_HOME)/hardware/tools/avr/bin/avrdude

PREPROCESSOR_DEFINITIONS= -DF_CPU=16000000L -DDEBUG -D_DEBUG
INCLUDE_DIRS=-I$(CODE_DIR)
COMMON_FLAGS= -c -g -Os -ffunction-sections -fdata-sections -mmcu=atmega2560 \
	$(PREPROCESSOR_DEFINITIONS) $(INCLUDE_DIRS)
ALL_C_FLAGS=$(COMMON_FLAGS)
ALL_CXX_FLAGS= -fno-exceptions $(COMMON_FLAGS)

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings

# build rules
all: $(OUTPUT_NAME).elf $(OUTPUT_NAME).hex $(OUTPUT_NAME).eep

run: $(OUTPUT_FILE)
	$(AVRDUDE) -C$(ARDUINO_HOME)/hardware/tools/avr/etc/avrdude.conf -v -v -v -v -patmega2560 -cstk500v2 -P\\.\COM12 -b115200 -D -Uflash:w:$(OUTPUT_FILE):i 

$(OUTPUT_NAME).elf: $(PRJ_OBJ)
	$(AVR_CXX) -mmcu=atmega2560 -Wl,-Map=blinkGcc.map \
	$(PRJ_OBJ) -o $(OUTPUT_NAME).elf
	
$(OUTPUT_NAME).hex: $(OUTPUT_NAME).elf
	$(AVR_OBJCPY) -O ihex -R .eeprom -R .fuse -R .lock -R .signature \
	$(OUTPUT_NAME).elf $(OUTPUT_NAME).hex

$(OUTPUT_NAME).eep: $(OUTPUT_NAME).elf
	$(AVR_OBJCPY) $(HEX_EEPROM_FLAGS) -O ihex $(OUTPUT_NAME).elf $@
	
%.o: %.cpp
	$(AVR_CXX) $(ALL_CXX_FLAGS) -o $@ $^

%.co: %.c
	$(AVR_CC) $(ALL_C_FLAGS) -o $@ $^
	
clean:
	rm -f $(ARDUINO_OBJ) $(PRJ_OBJ) $(OUTPUT_FILE)

rebuild: clean all
# Phony rules
.PHONY: all run rebuild clean