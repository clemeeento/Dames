#ifndef INTERFACE_H
#define INTERFACE_H

#define MAX_BOARD 100 //Taille total du tableau
#define WH_BOARD 10 //Dimension du tableau


char *initGame();

int **convertTableau(char *uneDimension);
int **convertTableau(char *uneDimension) ;

int convertPosition(int x, int y);

void affichage(int **deuxDimension);

int *inputSouris();

#endif