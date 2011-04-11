CC=gcc
CFLAGS=-Wall -g

classh: main.o i2c.o
	$(CC) -o classh main.o i2c.o

i2c.o: i2c.c
	$(CC) $(CFLAGS) -c i2c.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c
