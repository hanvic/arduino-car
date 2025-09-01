#/bin/sh
arduino-cli board list
arduino-cli compile --fqbn arduino:avr:nano:cpu=atmega328old HC12_TX/HC12_TX.ino
arduino-cli upload -p /dev/cu.usbserial-A5069RR4 --fqbn arduino:avr:nano:cpu=atmega328old HC12_TX/HC12_TX.ino