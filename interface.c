#include <stdio.h>
#include <stdlib.h>

#include"interface.h"

/*
0 : Case vide
1 : Pion noir
2 : Pion Blanc
3 : Dame noir
4 : Dame blanche

si au final tu travail avec un tableau d'une dimention comme tu pourra voir j'ai utilisé un tableau de char, 
pour avoir la valeur à la position i en int il faut que tu ecrive : position=(int)tableau[i]
*/

int **convertTableau(char *uneDimension) 
{
    int i, j;
    int **deuxDimension = (int**)malloc(WH_BOARD*sizeof(int*));
    for (i=0; i<WH_BOARD; i++)
        deuxDimension[i] = (int*)malloc(WH_BOARD*sizeof(int));
    for (i = 0; i < WH_BOARD; i++) {
        for (j = 0; j < WH_BOARD; j++) {
            deuxDimension[i][j] = (int)uneDimension[i * WH_BOARD + j];
        }
    }
    return deuxDimension;
}

//convertit les coordonnées (x, y) d'une case d'un tableau de deux dimensions en sa position dans un tableau d'une dimension
int convertPosition(int x, int y) 
{
    return x * WH_BOARD + y;
}

void affichage(int **deuxDimension)
{

    //Ton code pour l'affichage

}

int *inputSouris()
{
    int *input = (int*)malloc(2*sizeof(int));
    int x_init,y_init,x_final,y_final;

    //Ton Code pour les entres de la souris

    int pos_init=convertPosition(x_init,y_init);
    int pos_final=convertPosition(x_final,y_final);
    input[0]=pos_init;
    input[1]=pos_final;
    return input;
}


