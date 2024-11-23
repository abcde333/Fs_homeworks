CC = g++
CFLAGS = -c
SRC = simple-copy.cpp

all: Fs2

Fs2: FS2.o
	$(CC) FS2.o -o Fs2
	
FS2.o: FS2.cpp
	$(CC) $(CFLAGS) FS2.cpp

clean:
	rm -rf *.o Fs2
