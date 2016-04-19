name = output
#Phillip
device = /dev/tty.usbmodem1411 
#Sahithya
#device = /dev/tty.usbmodem1451 
src = ../Sensor_Code/main.c
flags = -std=c11 -Os -O1
baud = 9600

printf_min = -Wl,-u,vfprintf -lprintf_min #only int/string support
printf_float = -Wl,-u,vfprintf -lprintf_flt -lm #with float/double support

link_printf = #default
#link_printf = $(printf_min)
#link_printf = $(printf_float)

all : build program clean

build: $(src)
	avr-gcc $(flags) -DBAUD=$(baud) -DF_CPU=16000000UL -mmcu=atmega32u4 \
    $(link_printf) -o $(name).elf $(src)
	avr-objcopy -O ihex -R .eeprom $(name).elf $(name).ihex
	rm $(name).elf

list: $(src)
	avr-gcc -c -g -Wa,-a,-ad $(flags) -DBAUD=$(baud) -DF_CPU=16000000UL \
    -mmcu=atmega32u4 $(src) > $(name).lst
	rm $(name).elf

program: build
	avrdude -F -V -c avr109 -p m32u4 -P $(device) -b 115200 \
	-U flash:w:$(name).ihex

clean:
	rm -f $(name).hex $(name).lst