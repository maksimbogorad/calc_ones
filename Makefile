CC = gcc
CFLAGS = -g -Wall -Werror -pedantic
TARGET = calc_ones

all: $(TARGET)

calc_ones.o: calc_ones.c
	$(CC) $(CFLAGS) -c calc_ones.c -o calc_ones.o

$(TARGET): calc_ones.o
	$(CC) $(CFLAGS) calc_ones.o -o $(TARGET)

run: $(TARGET)
	./$(TARGET) test1.bin

test: $(TARGET)
	./$(TARGET) test1.bin
	./$(TARGET) test2.bin
	./$(TARGET) test3.bin

clean:
	rm -f *.o $(TARGET)