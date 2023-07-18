CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all: life

life: universe.o life.o
	$(CC) -o life universe.o life.o -lncurses

universe.o: universe.c
	$(CC) $(CFLAGS) -c universe.c

life.o: life.c
	$(CC) $(CFLAGS) -c life.c

debug: CFLAGS += -g
debug: all

clean:
	rm -f universe life *.o

format:
	clang-format -i -style=file *.[c,h]
