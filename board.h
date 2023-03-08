#ifndef BOARD_H
#define BOARD_H

#include "list.h"

#define RANDINIT()  srand(time(NULL))
#define RANDOM()  ((float)rand() / (float)RAND_MAX)
#define RANDMAX(x)  (int)((float)(x)*rand()/(RAND_MAX+1.0))

#define MAX_BOARD 100
#define WH_BOARD 10



void initBoard(Item *node, char *board);
	
void printBoard( Item *board );

int isValidPosition( Item *node, int pos_init, int pos_final ) ;

Item *move( Item *node, int pos_init, int pos_final);

#endif