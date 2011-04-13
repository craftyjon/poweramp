CC=gcc
CFLAGS=-Wall -g
LFLAGS=-ljack -lncurses

classh: i2c.o jack.o main.o
	$(CC) $(LFLAGS) -o classh jack.o i2c.o main.o

jack.o: jack.c
	$(CC) $(CFLAGS) -c jack.c

i2c.o: i2c.c
	$(CC) $(CFLAGS) -c i2c.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c
