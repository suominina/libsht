CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic -std=c11

test: test.c
	$(CC) $(CFLAGS) -o $@ $^
