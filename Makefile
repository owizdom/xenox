CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = chip8
SRC = xenox.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
