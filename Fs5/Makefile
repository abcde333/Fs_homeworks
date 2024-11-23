CXX = g++
CXXFLAGS = -c
SRC = manual-append.cpp
OBJ = manual-append.o
TARGET = manual-append

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $(TARGET) $(OBJ)

$(OBJ): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC)

clean:
	rm -rf *.o $(TARGET)
