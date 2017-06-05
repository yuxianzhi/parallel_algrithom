CC=gcc -w 
FFLAGS=-pthread -O3 -msse4 -fopenmp

all: main

main : matrixMult.c
	$(CC) $(FFLAGS)  matrixMult.c -o main -lm
clean:
	rm -f main


