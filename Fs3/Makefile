CC = g++
CFLAGS = -c

all: Fs3

Fs3: FS3.o
	$(CC) FS3.o -o FS3

FS3.o: FS3.cpp
	$(CC) $(CFLAGS) FS3.cpp

clean:
	rm -rf *.o FS3
