name = output
device = /dev/tty.usbmodem1411
lcd_src = ../ST7565-LCD/c/*.o
local_src =  ../Sensor_Code/sensor.c ../Sensor_Code/display.c ../Sensor_Code/storage.c
src = ../Sensor_Code/main.c 
flags = -std=c11 -Os
baud = 9600

printf_min = -Wl,-u,vfprintf -lprintf_min #only int/string support
printf_float = -Wl,-u,vfprintf -lprintf_flt -lm #with float/double support

link_printf = #default
#link_printf = $(printf_min)
#link_printf = $(printf_float)

all: build program

build: $(lcd_src) $(local_src) $(src)
	avr-gcc $(flags) -DBAUD=$(baud) -DF_CPU=16000000UL -mmcu=atmega328p \
    $(link_printf) -o $(name).elf $(lcd_src) $(src) $(local_src)
	avr-objcopy -O ihex -R .eeprom $(name).elf $(name).ihex
	rm $(name).elf

list: $(lcd_src) $(src)
	avr-gcc -c -g -Wa,-a,-ad $(flags) -DBAUD=$(baud) -DF_CPU=16000000UL \
    -mmcu=atmega328p $(src) > $(name).lst
	rm $(name).elf

program: build
	avrdude -F -V -c arduino -p ATMEGA328P -P $(device) -b 115200 \
    -U flash:w:$(name).ihex

clean:
	rm -f $(name).ihex $(name).lst
	(cd ../ST7565-LCD/c && make clean)

library: 
	(cd ../ST7565-LCD/c && make all)