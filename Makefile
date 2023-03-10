all: dames board.o list.o minmax.o jeu.o

list.o: list.c list.h
	gcc -c list.c

board.o: board.c board.h
	gcc -c board.c

minmax.o :minmax.c minmax.h
	gcc -c minmax.c

jeu.o: jeu.c jeu.h
	gcc -c jeu.c

dames: dames.c board.c list.c minmax.c jeu.c board.h list.h minmax.h jeu.h item.h
	gcc -o dames dames.c board.c list.c minmax.c jeu.c -lSDL2
