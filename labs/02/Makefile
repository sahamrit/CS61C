CC=gcc
CFLAGS=-Wall -std=c99 -g

all: ll_cycle 
ll_cycle: ll_cycle.c test_ll_cycle.c ll_cycle.h
	$(CC) $(CFLAGS) -c ll_cycle.c; \
	$(CC) $(CFLAGS) -o ll_cycle test_ll_cycle.c ll_cycle.o 

clean:
	rm -f ll_cycle 