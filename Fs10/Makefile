CC = g++
CFLAGS = -c
SRC = copy.cpp
OBJ = copy.o
TARGET = copy

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) $(SRC)

clean:
	rm -f $(OBJ) $(TARGET)
