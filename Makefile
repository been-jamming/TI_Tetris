CC = tigcc

CFLAGS = -O3

default: tetris.c tetris.h
	$(CC) $(CFLAGS) tetris.c -o tetris

