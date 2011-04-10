CC=gcc
CFLAGS=-Wall -g

i2ctest: i2ctest.o
	$(CC) -o i2ctest i2ctest.o

i2ctest.o: i2ctest.c
	$(CC) $(CFLAGS) -c i2ctest.c
