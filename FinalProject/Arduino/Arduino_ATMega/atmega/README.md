ATmega
========
The folder contains the ATmega88 template, atmega programming environment, examples showing how to use the libraries and Utility.
 - ATmega88Template: The template contains all necessary files for user to work with ATmega88;
 - atmega: 
    * common_library: contain commonly used libraries for users;
    * atmega88a_common: contain ADC, PWM, TWI_slave and USART for ATmega88a;
    * atmega168a_common: contain ADC, PWM, TWI_slave and USART for ATmega168a;
    * atmega32u4_common: contain m_bus and m_usb for ATmega32U4;
    * project: all projects' folder are here and put all source files including main.c in src folder and put all header files in inc folder;
    * Makefile: common Makefile;
    * smores_common_library: contain commonly used libraries for smores project, including drive_coils and wiper_encoder.
 - Utility: all necessary files and README for programming atmega in Windows.

In order to use the programming environment, install gcc-avr, avr-libc, avrdude and libusb first.  
 - For Mac, go here: https://www.obdev.at/products/crosspack/index.html and install the latest version of CrossPack;
 - For Ubuntu, sudo apt-get install gcc-avr avrdude avr-libc libusb-dev;
 - For Windows, refer to README in Utility.

How to use:

 - Make your own project_peripherals_customized-name.h and modify project_peripherals.h accordingly;
 - Modify the Makefile in project to specify the user's onw directories for header files and source files.
 - Figure out what programmer you are using and change the PROGRAMMER in project Makefile accordingly;
 - For ATmega88a and ATmega168a, all codes are made to work with 8MHz internal RC oscillator. With a new chip, first program the fuse using “make fuse”;
 - For ATmega32U4, all codes are made to work with 16MHz external oscillator;
 - Use command “make” to generate the object files and hex file and all object files are in obj folder;
 - Use command “make flash” to flash the code to the chip;
 - In Linux, you need "sudo" to run "make fuse" and "make flash" commands.

