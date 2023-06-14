CC = gcc
CFLAGS = -Wall -Wextra

all: Multi Mono

Multi: MainMulti.c HeaderMulti.h
	$(CC) $(CFLAGS) MainMulti.c HeaderMulti.h -o Multi

Mono: MainMono.c HeaderMono.h
	$(CC) $(CFLAGS) MainMono.c HeaderMono.h -o Mono

clean:
	rm -f Multi Mono

