CC = gcc

CFLAGS = -Wall -g -Iinclude
LDFLAGS = -mwindows -lSDL3

SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:src/%.c=build/%.o)

EXECUTABLE = SNAKE.exe

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf build SNAKE.exe

rebuild: clean all

.PHONY: all clean rebuild