all: program.o
	gcc -o main program.o

program.o: program.c
	gcc -c program.c

run: 
	./main
