#ifndef MINMAX_H
#define MINMAX_H

#include "board.h"
#include "list.h"
#include "jeu.h"

#define DepthMax 3
#define RANDINIT()  srand(time(NULL))
#define RANDOM()  ((float)rand() / (float)RAND_MAX)
#define RANDMAX(x)  (int)((float)(x)*rand()/(RAND_MAX+1.0))

int evaluation(Item *node) ;

Item *generation_coups(Item* node, list_t listeCoups,int i,int j) ;

Item *generationArbre(Item *node,Item *tmp);

Item * minmax(Item *node);

#endif