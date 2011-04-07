C = gcc
CFLAGS = -g -Wall -ljack -lpthread -lrt -lncurses -ltinfo
OBJECTS = jacktest.o

spitest : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o jacktest

spitest.o : jacktest.c
	$(CC) $(CFLAGS) -c jacktest.c 

