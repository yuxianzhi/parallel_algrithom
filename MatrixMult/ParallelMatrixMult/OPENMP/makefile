CC=gcc -w 
FFLAGS=-fopenmp -O3 -msse4

all: main
main : matrixMult.c
	$(CC) $(FFLAGS)  matrixMult.c -o main -lm
clean:
	rm -f main


