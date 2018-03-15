DEVICE     = m1284
DEVICEG    = atmega1284
CLOCK      = 8000000
PROGRAMMER = atmelice_isp
BAUD       = 19200
FILENAME   = main
COMPILE    = avr-gcc -Wall -Os -mmcu=$(DEVICEG)
# OBJS       = $(wildcard *.o)
OBJS       = main.o snes.o lcd_display.o vc_vector.o
all:  clean build upload
			
build:
		$(COMPILE) -c $(FILENAME).c snes.c lcd_display.c vc_vector.c
			$(COMPILE) -o $(FILENAME).elf $(OBJS) 
				avr-objcopy -j .text -j .data -O ihex $(FILENAME).elf $(FILENAME).hex
					avr-size --format=avr --mcu=$(DEVICEG) $(FILENAME).elf

upload:
		avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -b $(BAUD) -U flash:w:$(FILENAME).hex:i 

clean:
		rm -f *.o
			rm -f *.elf
				rm -f *.hex

