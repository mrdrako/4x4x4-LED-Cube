!#/bin/bash

make clean

make all

avrdude -c usbtiny -p m16 -U flash:w:main.hex
