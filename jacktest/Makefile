C = gcc
CFLAGS = -g
OBJECTS = jacktest.o

spitest : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o jacktest

spitest.o : jacktest.c
	$(CC) $(CFLAGS) -c jacktest.c 

