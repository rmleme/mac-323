CFLAGS=-Wall -g ${INC} ${LIB}
CC=gcc

ep3.o: ep3.c
	$(CC) $(CFLAGS) -c $*.c

ep3 : ep3.o
	${CC} ${CFLAGS} -o ep3 ep3.o -lm

clean: 
	rm -f *.o ep3 *~