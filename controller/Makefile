CC=gcc
CFLAGS=-g -I. -Wall -Wextra -pedantic
LIBS=-lSDL2 -lSDL2_ttf -lSDL2main -lbluetooth
TARGET=./out

$(TARGET): main.c
	$(CC) -o $(TARGET) $< $(CFLAGS) $(LIBS) -g

.PHONY: run test debug clean

run:
	$(TARGET)

test:
	make && make run

debug:
	make && gdb $(TARGET)

clean:
	rm $(TARGET)
