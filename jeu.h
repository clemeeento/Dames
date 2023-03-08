#ifndef JEU_H
#define JEU_H

#include "board.h"
#include "list.h"

#define RANDINIT()  srand(time(NULL))
#define RANDOM()  ((float)rand() / (float)RAND_MAX)
#define RANDMAX(x)  (int)((float)(x)*rand()/(RAND_MAX+1.0))

char *initGame();

char *moveJeu( char *plateau, int pos_init, int pos_final);

int isValidPositionJeu( char *plateau, int pos_init, int pos_final);

int partieFini(char *plateau);

char *coupIA(Item *node,char *plateau);

#endif