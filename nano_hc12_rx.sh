#/bin/sh
arduino-cli board list
# arduino-cli compile --fqbn arduino:avr:nano:cpu=atmega328old HC12_RX/HC12_RX.ino
# arduino-cli upload -p /dev/cu.usbserial-1140 --fqbn arduino:avr:nano:cpu=atmega328old HC12_RX/HC12_RX.ino


arduino-cli compile --fqbn arduino:avr:nano:cpu=atmega328old HC12_RX/HC12_RX.ino
arduino-cli upload -p /dev/cu.usbserial-1140 --fqbn arduino:avr:nano:cpu=atmega328old HC12_RX/HC12_RX.ino