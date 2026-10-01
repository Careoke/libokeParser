CC = cc 
CFLAGS = -Wall -Wextra -Iinclude -O2

SRC = $(wildcard src/*.c)
OBJ := $(patsubst src/%.c,out/%.o,$(SRC))

TARGET = out/parser

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@

ifeq ($(OS),Windows_NT)
out/%.o: src/%.c
	-mkdir out
	$(CC) $(CFLAGS) -c $< -o $@
else
out/%.o: src/%.c
	mkdir -p out
	$(CC) $(CFLAGS) -c $< -o $@
endif

run: $(TARGET)
	./$(TARGET)

ifeq ($(OS),Windows_NT)
clean:
	-rmdir /s /q out
else
clean:
	rm -rf out
endif