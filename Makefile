CC=gcc
CFLAGS=-Wall

all:
	$(CC) main.c archive.c extract.c utils.c -o tarsau $(CFLAGS)

clean:
	rm -f tarsau
