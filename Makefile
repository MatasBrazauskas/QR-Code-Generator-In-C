CC = gcc
CFLAGS = -I include -Wall -Wextra -Werror -O3

all:
	$(CC) $(CFLAGS) src/*.c -o main

clean:
	rm -f main

.PHONY: all clean