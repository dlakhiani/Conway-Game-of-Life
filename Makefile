#the compiler
CC = gcc
#compiler flags:
CFLAGS = -ansi -Wall

cgol:
	$(CC) $(CFLAGS) -o cgol cgol.c

dynCGOL:
	$(CC) $(CFLAGS) -o dynCGOL dynCGOL.c

all: cgol dynCGOL

clean:
	rm cgol dynCGOL
