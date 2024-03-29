.PHONY: all flash clean

CC := arm-none-eabi-gcc
LD := arm-none-eabi-ld
OC := arm-none-eabi-objcopy
OD := arm-none-eabi-objdump
SZ := arm-none-eabi-size

CFLAGS := -mthumb \
	-mcpu=cortex-m4 \
	-mfpu=fpv4-sp-d16 \
	-mfloat-abi=softfp \
	-ffunction-sections \
	-fdata-sections \
	-std=c99 -Wall -pedantic \
	-Os -O0 -g \
	-DPART_TM4C123GH6PM -DTARGET_IS_BLIZZARD_RA1 -Dgcc

LDFLAGS := -T ../../lib/tm4c123.ld --entry ResetISR --gc-sections
OBJCOPYFLAGS := -O binary

SOURCES := $(wildcard *.c) $(wildcard ../../lib/*.c)
OBJECTS := $(patsubst %.c, build/%.o, $(SOURCES))
DEPENDS := $(patsubst %.c, build/%.d, $(SOURCES))

all: main

test:
	echo "SOURCES: $(SOURCES)"
	echo "OBJECTS: $(OBJECTS)"
	echo "DEPENDS: $(DEPENDS)"

flash: all
	lm4flash build/main.bin

clean:
	rm -rf build

-include $(DEPENDS)

build/%.o: %.c Makefile
	mkdir -p build
	$(CC) $(CFLAGS) -I../../lib -MD -MF build/$(notdir $*.d) -c $< -o build/$(notdir $@)

main: $(OBJECTS)
	$(LD) $(addprefix build/, $(notdir $^)) $(LDFLAGS) -o build/main.axf
	$(OC) $(OBJCOPYFLAGS) build/main.axf build/main.bin
	$(OD) -Sd -W build/main.axf > build/main.lss
	$(SZ) build/main.axf
