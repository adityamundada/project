project: my2048.o
	cc my2048.o -o project
my2048.o: my2048.c
	cc -Wall -c my2048.c
