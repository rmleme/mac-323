CFLAGS=-Wall -g ${INC} ${LIB}
CC=gcc

ep2.o: ep2.c
	$(CC) $(CFLAGS) -c $*.c

ep2 : ep2.o
	${CC} ${CFLAGS} -o ep2 ep2.o

clean: 
	rm -f *.o ep2 *~