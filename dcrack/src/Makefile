CC = gcc
CFLAGS = -Wall -Werror -Wmissing-prototypes
OBJS = ../obj/input.o ../obj/crack.o ../obj/main.o

all: clean input.o crack.o main.o dcrack

dcrack: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o ../dcrack -lcrypt -lm -fopenmp

main.o: main.c ../lib/input.h
	${CC} $(CFLAGS) -c main.c -o ../obj/main.o
	
crack.o: crack.c ../lib/crack.h ../lib/input.h
	$(CC) $(CFLAGS) -c crack.c -o ../obj/crack.o -lcrypt -lm -fopenmp

input.o: input.c ../lib/input.h
	$(CC) $(CFLAGS) -c input.c -o ../obj/input.o

clean:
	rm -f ../obj/*.o
