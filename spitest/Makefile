C = gcc
CFLAGS = -g
OBJECTS = spitest.o

spitest : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o spitest

spitest.o : spitest.c
	$(CC) $(CFLAGS) -c spitest.c 

