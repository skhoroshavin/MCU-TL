#/bin/sh
avr-objcopy -O ihex $1 $1.hex
avrdude -c arduino -p m328p -P /dev/ttyUSB0 -b 57600 -u -U $1.hex
