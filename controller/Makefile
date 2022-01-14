CC=gcc
CFLAGS=-g -I. -Wall -Wextra -pedantic
LIBS=-lSDL2main -lSDL2 -lbluetooth
OUTFILE=./out

$(OUTFILE): main.c
	$(CC) -o $(OUTFILE) $< $(CFLAGS) $(LIBS)

.PHONY: run test debug clean

run:
	$(OUTFILE)

test:
	make && make run

debug:
	make && gdb $(OUTFILE)

clean:
	rm $(OUTFILE)
