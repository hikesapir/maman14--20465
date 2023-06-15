main: symbolsTable.o util.o
	gcc -Wall -ansi -pedantic -g symbolsTable.o util.o -o main.exe
symbolsTable.o: symbolsTable.c symbolsTable.h util.h
	gcc -c -Wall -ansi -pedantic -g symbolsTable.c -o symbolsTable.o
util.o: util.c
	gcc -c -Wall -ansi -pedantic -g util.c -o util.o
