ARDUINO_DIR=/usr/local/arduino
ARDUINO_MK_DIR=/usr/local/arduino-bsd-mk
ARDUINO_LIBS=SoftwareSerial
AVRDUDE_PORT=/dev/cuaU0
ARDUINO_BOARD=uno
SRCS=car.ino
TARGET=a.out

include /usr/local/arduino-bsd-mk/bsd.arduino.mk
