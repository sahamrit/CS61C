CC=gcc
CFLAGS=-Wall -std=c99 -g -O3

all: ex2 ex3
ex2: matrixMultiply.c
	$(CC) $(CFLAGS) -o matrixMultiply matrixMultiply.c
ex3: transpose.c
	$(CC) $(CFLAGS) -o transpose transpose.c
clean:
	rm -f matrixMultiply transpose