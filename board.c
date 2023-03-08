#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "board.h"


// Print le tableau d'un node
void printBoard( Item *node )
{
  assert(node);
  printf("\n");
	for (int j=0; j<WH_BOARD; j++) if (j==0) printf(" ___"); else printf("____"); printf("\n");
	for (int i = 0 ; i < MAX_BOARD ; i++) {
    if (i%WH_BOARD == 0) printf("|");
    if (node->board[i] == 0) printf("   |");
    else printf("%2d |", node->board[i]);
    if (((i+1)%WH_BOARD) == 0) {
			printf("\n");
			for (int j=0; j<WH_BOARD; j++) if (j==0) printf(" ___"); else printf("____"); printf("\n");
		}
  }
	printf("\n");
}

// Initialisation d'un node depuis un tableau donné
void initBoard(Item *node, char *board) {
  node->size = MAX_BOARD;
  node->board = calloc(MAX_BOARD, sizeof(char));
  
	int i;
  for(i=0;i<MAX_BOARD;i=i+1)
  {
    node->board[i]=board[i];
  }
  node->g=0;
}

//Vérifie la validité d'un coup à partir se la position initial et de la position finale
int isValidPosition( Item *node, int pos_init, int pos_final)
{
	if(node->board[pos_init]==1) //Pion Noir 
  {
    if(node->board[pos_final]==0)//Il faut que la case soit vide pour pouvoir deplacer le pion 
    {
      if((pos_final==pos_init+WH_BOARD+1) && ((pos_init+1)%WH_BOARD!=0)) //Deplacement en diagonale à droite sans prendre
      {
        return 1;
      }
      if((pos_final==pos_init+WH_BOARD-1) && ((pos_init)%WH_BOARD!=0)) //Deplacement en diagonale à gauche sans prendre
      {
        return 1;
      }

      if((pos_final==pos_init+2*(WH_BOARD+1)) && ((pos_init+2)%WH_BOARD!=0) && (node->board[pos_init+WH_BOARD+1]==2 ||node->board[pos_init+WH_BOARD+1]==4)) //Deplacement en diagonale à droite en prenant
      {
        return 1;
      }
      if((pos_final==pos_init+2*(WH_BOARD-1)) && ((pos_init-1)%WH_BOARD!=0) && (node->board[pos_init+WH_BOARD-1]==2 ||node->board[pos_init+WH_BOARD-1]==4)) //Deplacement en diagonale à gauche en prenant
      {
        return 1;
      }
    }

  }

  if(node->board[pos_init]==2) //Pion Blanc
  {
    if(node->board[pos_final]==0)//Il faut que la case soit vide pour pouvoir deplacer le pion 
    {
      if((pos_final==pos_init-WH_BOARD+1) && ((pos_init+1)%WH_BOARD!=0)) //Deplacement en diagonale à droite sans prendre
      {
        return 1;
      }
      if((pos_final==pos_init-WH_BOARD-1) && ((pos_init)%WH_BOARD!=0)) //Deplacement en diagonale à gauche sans prendre
      {
        return 1;
      }

      if((pos_final==pos_init-2*(WH_BOARD+1)) && ((pos_init+2)%WH_BOARD!=0) && (node->board[pos_init-WH_BOARD-1]==1 ||node->board[pos_init-WH_BOARD-1]==3)) //Deplacement en diagonale à droite en prenant
      {
        return 1;
      }
      if((pos_final==pos_init-2*(WH_BOARD-1)) && ((pos_init-1)%WH_BOARD!=0) && (node->board[pos_init-WH_BOARD+1]==1 ||node->board[pos_init-WH_BOARD+1]==3)) //Deplacement en diagonale à gauche en prenant
      {
        return 1;
      }
    }
  }
  

  if(node->board[pos_init]==3) //Dame Noir
  {
    if(node->board[pos_final]==0)//Il faut que la case soit vide pour pouvoir deplacer la dame
    {
      //Deplacement en diagonale à droite haut
      if(((pos_final-pos_init)%(WH_BOARD-1))==0 && (pos_final-pos_init)/(WH_BOARD-1)<0) //On verifie que le coup est bien dans la diagonale doite haut
      {
        /*Pour eviter que la dame sorte du plateau vérifiez que la différence absolue entre la ligne de la case initiale et 
        la ligne de la case finale est égale à la différence absolue entre la colonne de la case initiale et la colonne de la case finale.*/
        int i=pos_init/WH_BOARD;
        int ii=pos_final/WH_BOARD;
        int j=pos_init%WH_BOARD;
        int jj=pos_final%WH_BOARD;
        if(abs(i-ii)==abs(j-jj)) //On verifie que le deplacement ne soit pas en dehors du plateau
        {
          int k=abs(pos_final-pos_init)/(WH_BOARD-1); //K le nombre de cases entre le coup initial et le coup final
          if(k==1) //Si k=1 alors la dame se deplace que d'une case et donc le coup est valide
          {
            return 1;
          }
          //Sinon on verifie qu'il y ai un pion ou une dame sur son chemin (et qu'un seul)
          int pion=0;
          for(int i=1;i<=k;i=i+1)
          {
            if(node->board[pos_init-i*(WH_BOARD-1)]==2 || node->board[pos_init-i*(WH_BOARD-1)]==4)
            pion++;
          }
          if(pion==1)
          {
            return 1;
          }
        }
        
      }

      //Deplacement en diagonale à droite bas
      if(((pos_final-pos_init)%(WH_BOARD+1))==0 && (pos_final-pos_init)/(WH_BOARD+1)>0) //On verifie que le coup est bien dans la diagonale doite bas
      {
        /*Pour eviter que la dame sorte du plateau vérifiez que la différence absolue entre la ligne de la case initiale et 
        la ligne de la case finale est égale à la différence absolue entre la colonne de la case initiale et la colonne de la case finale.*/
        int i=pos_init/WH_BOARD;
        int ii=pos_final/WH_BOARD;
        int j=pos_init%WH_BOARD;
        int jj=pos_final%WH_BOARD;
        if(abs(i-ii)==abs(j-jj)) //On verifie que le deplacement ne soit pas en dehors du plateau
        {
          int k=abs(pos_final-pos_init)/(WH_BOARD+1); //K le nombre de cases entre le coup initial et le coup final
          if(k==1) //Si k=1 alors la dame se deplace que d'une case et donc le coup est valide
          {
            return 1;
          }
          //Sinon on verifie qu'il y ai un pion ou une dame sur son chemin (et qu'un seul)
          int pion=0;
          for(int i=1;i<=k;i=i+1)
          {
            if(node->board[i*(WH_BOARD+1)+pos_init]==2 || node->board[i*(WH_BOARD+1)+pos_init]==4)
            pion++;
          }
          if(pion==1)
          {
            return 1; 
          }
        }
        
      }
      
      //Deplacement en diagonale à gauche haut
      if(((pos_final-pos_init)%(WH_BOARD+1))==0 && (pos_final-pos_init)/(WH_BOARD+1)<0) //On verifie que le coup est bien dans la diagonale gauche haut
      {
        /*Pour eviter que la dame sorte du plateau vérifiez que la différence absolue entre la ligne de la case initiale et 
        la ligne de la case finale est égale à la différence absolue entre la colonne de la case initiale et la colonne de la case finale.*/
        int i=pos_init/WH_BOARD;
        int ii=pos_final/WH_BOARD;
        int j=pos_init%WH_BOARD;
        int jj=pos_final%WH_BOARD;
        if(abs(i-ii)==abs(j-jj)) //On verifie que le deplacement ne soit pas en dehors du plateau
        {
          int k=abs(pos_final-pos_init)/(WH_BOARD+1); //K le nombre de cases entre le coup initial et le coup final
          if(k==1) //Si k=1 alors la dame se deplace que d'une case et donc le coup est valide
          {
            return 1;
          }
          //Sinon on verifie qu'il y ai un pion ou une dame sur son chemin (et qu'un seul)
          int pion=0;
          for(int i=1;i<=k;i=i+1)
          {
            if(node->board[pos_init-i*(WH_BOARD+1)]==2 || node->board[pos_init-i*(WH_BOARD+1)]==4)
            pion++;
          }
          if(pion==1)
          {
            return 1;
          }
        }
      }

      //Deplacement en diagonale à gauche bas
      if(((pos_final-pos_init)%(WH_BOARD-1))==0 && (pos_final-pos_init)/(WH_BOARD-1)>0) //On verifie que le coup est bien dans la diagonale gauche bas
      {
        /*Pour eviter que la dame sorte du plateau vérifiez que la différence absolue entre la ligne de la case initiale et 
        la ligne de la case finale est égale à la différence absolue entre la colonne de la case initiale et la colonne de la case finale.*/
        int i=pos_init/WH_BOARD;
        int ii=pos_final/WH_BOARD;
        int j=pos_init%WH_BOARD;
        int jj=pos_final%WH_BOARD;
        if(abs(i-ii)==abs(j-jj)) //On verifie que le deplacement ne soit pas en dehors du plateau
        {
          int k=abs(pos_final-pos_init)/(WH_BOARD-1); //K le nombre de cases entre le coup initial et le coup final
          if(k==1) //Si k=1 alors la dame se deplace que d'une case et donc le coup est valide
          {
            return 1;
          }
          //Sinon on verifie qu'il y ai un pion ou une dame sur son chemin (et qu'un seul)
          int pion=0;
          for(int i=1;i<=k;i=i+1)
          {
            if(node->board[i*(WH_BOARD-1)+pos_init]==2 || node->board[i*(WH_BOARD-1)+pos_init]==4)
            pion++;
          }
          if(pion==1)
          {
            return 1;
          }
        }
      }
    }
  }

  if(node->board[pos_init]==4) //Dame Blanche
  {
    if(node->board[pos_final]==0)//Il faut que la case soit vide pour pouvoir deplacer la dame
    {
      //Deplacement en diagonale à droite haut
      if(((pos_final-pos_init)%(WH_BOARD-1))==0 && (pos_final-pos_init)/(WH_BOARD-1)<0) //On verifie que le coup est bien dans la diagonale doite haut
      {
        /*Pour eviter que la dame sorte du plateau vérifiez que la différence absolue entre la ligne de la case initiale et 
        la ligne de la case finale est égale à la différence absolue entre la colonne de la case initiale et la colonne de la case finale.*/
        int i=pos_init/WH_BOARD;
        int ii=pos_final/WH_BOARD;
        int j=pos_init%WH_BOARD;
        int jj=pos_final%WH_BOARD;
        if(abs(i-ii)==abs(j-jj)) //On verifie que le deplacement ne soit pas en dehors du plateau
        {
          int k=abs(pos_final-pos_init)/(WH_BOARD-1); //K le nombre de cases entre le coup initial et le coup final
          if(k==1) //Si k=1 alors la dame se deplace que d'une case et donc le coup est valide
          {
            return 1;
          }
          //Sinon on verifie qu'il y ai un pion ou une dame sur son chemin (et qu'un seul)
          int pion=0;
          for(int i=1;i<=k;i=i+1)
          {
            if(node->board[pos_init-i*(WH_BOARD-1)]==1 || node->board[pos_init-i*(WH_BOARD-1)]==3)
            pion++;
          }
          if(pion==1)
          {
            return 1;
          }
        }
        
      }

      //Deplacement en diagonale à droite bas
      if(((pos_final-pos_init)%(WH_BOARD+1))==0 && (pos_final-pos_init)/(WH_BOARD+1)>0) //On verifie que le coup est bien dans la diagonale doite bas
      {
        /*Pour eviter que la dame sorte du plateau vérifiez que la différence absolue entre la ligne de la case initiale et 
        la ligne de la case finale est égale à la différence absolue entre la colonne de la case initiale et la colonne de la case finale.*/
        int i=pos_init/WH_BOARD;
        int ii=pos_final/WH_BOARD;
        int j=pos_init%WH_BOARD;
        int jj=pos_final%WH_BOARD;
        if(abs(i-ii)==abs(j-jj)) //On verifie que le deplacement ne soit pas en dehors du plateau
        {
          int k=abs(pos_final-pos_init)/(WH_BOARD+1); //K le nombre de cases entre le coup initial et le coup final
          if(k==1) //Si k=1 alors la dame se deplace que d'une case et donc le coup est valide
          {
            return 1;
          }
          //Sinon on verifie qu'il y ai un pion ou une dame sur son chemin (et qu'un seul)
          int pion=0;
          for(int i=1;i<=k;i=i+1)
          {
            if(node->board[i*(WH_BOARD+1)+pos_init]==1 || node->board[i*(WH_BOARD+1)+pos_init]==3)
            pion++;
          }
          if(pion==1)
          {
            return 1; 
          }
        }
        
      }
      
      //Deplacement en diagonale à gauche haut
      if(((pos_final-pos_init)%(WH_BOARD+1))==0 && (pos_final-pos_init)/(WH_BOARD+1)<0) //On verifie que le coup est bien dans la diagonale gauche haut
      {
        /*Pour eviter que la dame sorte du plateau vérifiez que la différence absolue entre la ligne de la case initiale et 
        la ligne de la case finale est égale à la différence absolue entre la colonne de la case initiale et la colonne de la case finale.*/
        int i=pos_init/WH_BOARD;
        int ii=pos_final/WH_BOARD;
        int j=pos_init%WH_BOARD;
        int jj=pos_final%WH_BOARD;
        if(abs(i-ii)==abs(j-jj)) //On verifie que le deplacement ne soit pas en dehors du plateau
        {
          int k=abs(pos_final-pos_init)/(WH_BOARD+1); //K le nombre de cases entre le coup initial et le coup final
          if(k==1) //Si k=1 alors la dame se deplace que d'une case et donc le coup est valide
          {
            return 1;
          }
          //Sinon on verifie qu'il y ai un pion ou une dame sur son chemin (et qu'un seul)
          int pion=0;
          for(int i=1;i<=k;i=i+1)
          {
            if(node->board[pos_init-i*(WH_BOARD+1)]==1 || node->board[pos_init-i*(WH_BOARD+1)]==3)
            pion++;
          }
          if(pion==1)
          {
            return 1;
          }
        }
      }

      //Deplacement en diagonale à gauche bas
      if(((pos_final-pos_init)%(WH_BOARD-1))==0 && (pos_final-pos_init)/(WH_BOARD-1)>0) //On verifie que le coup est bien dans la diagonale gauche bas
      {
        /*Pour eviter que la dame sorte du plateau vérifiez que la différence absolue entre la ligne de la case initiale et 
        la ligne de la case finale est égale à la différence absolue entre la colonne de la case initiale et la colonne de la case finale.*/
        int i=pos_init/WH_BOARD;
        int ii=pos_final/WH_BOARD;
        int j=pos_init%WH_BOARD;
        int jj=pos_final%WH_BOARD;
        if(abs(i-ii)==abs(j-jj)) //On verifie que le deplacement ne soit pas en dehors du plateau
        {
          int k=abs(pos_final-pos_init)/(WH_BOARD-1); //K le nombre de cases entre le coup initial et le coup final
          if(k==1) //Si k=1 alors la dame se deplace que d'une case et donc le coup est valide
          {
            return 1;
          }
          //Sinon on verifie qu'il y ai un pion ou une dame sur son chemin (et qu'un seul)
          int pion=0;
          for(int i=1;i<=k;i=i+1)
          {
            if(node->board[i*(WH_BOARD-1)+pos_init]==1 || node->board[i*(WH_BOARD-1)+pos_init]==3)
            pion++;
          }
          if(pion==1)
          {
            return 1;
          }
        }
      }
    }
    
  }

  return 0;
}

//Deplace une pièce
Item *move( Item *node, int pos_init, int pos_final)
{
  //Deplacement en diagonale à droite haut
  if(((pos_final-pos_init)%(WH_BOARD-1))==0 && (pos_final-pos_init)/(WH_BOARD-1)<0)
  {
    int k=abs(pos_final-pos_init)/(WH_BOARD-1);//K le nombre de cases entre le coup initial et le coup final
    if(k!=1)
    {
      int i=1;
      int pos_pris=0;
      for(int i=1;i<k;i=i+1)
      {
        if(node->board[pos_init-i*(WH_BOARD-1)]!=0)
        {
          pos_pris=pos_init-i*(WH_BOARD-1);
        }
      }
      node->board[pos_pris]=0;
    }
  }
  //Deplacement en diagonale à droite bas
  if(((pos_final-pos_init)%(WH_BOARD+1))==0 && (pos_final-pos_init)/(WH_BOARD+1)>0)
  {
    int k=abs(pos_final-pos_init)/(WH_BOARD+1);//K le nombre de cases entre le coup initial et le coup final
    if(k!=1)
    {
      int i=1;
      int pos_pris=0;
      for(int i=1;i<k;i=i+1)
      {
        if(node->board[pos_init+i*(WH_BOARD+1)]!=0)
        {
          pos_pris=pos_init+i*(WH_BOARD+1);
        }
      }
      node->board[pos_pris]=0;
    }
  }

  //Deplacement en diagonale à gauche haut
  if(((pos_final-pos_init)%(WH_BOARD+1))==0 && (pos_final-pos_init)/(WH_BOARD+1)<0)
  {
    int k=abs(pos_final-pos_init)/(WH_BOARD+1);//K le nombre de cases entre le coup initial et le coup final
    if(k!=1)
    {
      int i=1;
      int pos_pris=0;
      for(int i=1;i<k;i=i+1)
      {
        if(node->board[pos_init-i*(WH_BOARD+1)]!=0)
        {
          pos_pris=pos_init-i*(WH_BOARD+1);
        }
      }
      node->board[pos_pris]=0;
    }
  }
  //Deplacement en diagonale à gauche bas
  if(((pos_final-pos_init)%(WH_BOARD-1))==0 && (pos_final-pos_init)/(WH_BOARD-1)>0)
  {
    int k=abs(pos_final-pos_init)/(WH_BOARD-1);//K le nombre de cases entre le coup initial et le coup final
    if(k!=1)
    {
      int i=1;
      int pos_pris=0;
      for(int i=1;i<k;i=i+1)
      {
        if(node->board[pos_init+i*(WH_BOARD-1)]!=0)
        {
          pos_pris=pos_init+i*(WH_BOARD-1);
        }
      }
      node->board[pos_pris]=0;
    }
  }

  //Mouvements qui transforment en dame
  if(node->board[pos_init]==1 && pos_final>=MAX_BOARD-WH_BOARD)
  {
    node->board[pos_final]=3;
  }
  else if(node->board[pos_init]==2 && pos_final<WH_BOARD)
  {
    node->board[pos_final]=4;
  }
  else
  {
    node->board[pos_final]=node->board[pos_init];
  }

  node->board[pos_init]=0;
  
  return node;
}
