CC=gcc
CFLAGS=-Wall -std=c99 -g

all: ll_cycle vector bork
ll_cycle: ll_cycle.c test_ll_cycle.c ll_cycle.h
	$(CC) $(CFLAGS) -c ll_cycle.c; \
	$(CC) $(CFLAGS) -o ll_cycle test_ll_cycle.c ll_cycle.o 
vector: vector.c test_vector.c vector.h
	$(CC) $(CFLAGS) -c vector.c; \
	$(CC) $(CFLAGS) -o vector test_vector.c vector.o 
bork: bork.c 
	$(CC) $(CFLAGS) -o bork bork.c
clean:
	rm -f ll_cycle vector bork