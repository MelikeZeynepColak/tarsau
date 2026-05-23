CC = gcc
CFLAGS = -Wall -Iinclude

SRC = $(wildcard src/*.c)

tarsau:
	$(CC) $(CFLAGS) $(SRC) -o tarsau
