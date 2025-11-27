CC = gcc
CFLAGS = -I include -Wall -Wextra -Werror

all:
	$(CC) $(CFLAGS) src/*.c -o main

clean:
	rm -f main

.PHONY: all clean