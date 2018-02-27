DEVICE     = m1284
DEVICEG    = atmega1284
CLOCK      = 8000000
PROGRAMMER = atmelice_isp
BAUD       = 19200
FILENAME   = cta003_jjian014_lab5_part2_
COMPILE    = avr-gcc -Wall -Os -mmcu=$(DEVICEG)

all:  clean build upload
			
build:
		$(COMPILE) -c $(FILENAME).c -o $(FILENAME).o
			$(COMPILE) -o $(FILENAME).elf $(FILENAME).o
				avr-objcopy -j .text -j .data -O ihex $(FILENAME).elf $(FILENAME).hex
					avr-size --format=avr --mcu=$(DEVICEG) $(FILENAME).elf

upload:
		avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -b $(BAUD) -U flash:w:$(FILENAME).hex:i 

clean:
		rm *.o
			rm *.elf
				rm *.hex

