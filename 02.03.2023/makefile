CC = gcc
CFLAGS = -Wall -Werror

all: init destroy print set

init: init.c
	$(CC) $(CFLAGS) $^ -o $@

destroy: destroy.c
	$(CC) $(CFLAGS) $^ -o $@

print: print.c
	$(CC) $(CFLAGS) $^ -o $@

set: set.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f init destroy print set
