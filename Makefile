CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic -std=c11

main: main.c
	$(CC) $(CFLAGS) -o $@ $^
