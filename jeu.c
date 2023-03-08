#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "jeu.h"

//Initialise tableau du jeu
char *initGame()
{
  //initialise un tab de case vide
  char *initial = (char*)malloc(MAX_BOARD*sizeof(char));  
  for (int i=0; i<MAX_BOARD; i++) initial[i] = 0;   

  // Placer les pièces noires
  for(int j=0;j<WH_BOARD/2;j=j+1) 
  {
    if((j%2)==0)
    {
      for(int i=(j-1)*WH_BOARD;i<j*WH_BOARD;i=i+2) initial[i]=1;
    }
    else
    {
      for(int i=(j-1)*WH_BOARD+1;i<j*WH_BOARD;i=i+2) initial[i]=1;
    }
  }

  // Placer les pièces blanches
  for(int j=(WH_BOARD/2)+2;j<=WH_BOARD;j=j+1) 
  {
    if((j%2)==0)
    {
      for(int i=(j-1)*WH_BOARD;i<j*WH_BOARD;i=i+2) initial[i]=2;
    }
    else
    {
      for(int i=(j-1)*WH_BOARD+1;i<j*WH_BOARD;i=i+2) initial[i]=2;
    }
  }

  return initial;
}

//Vérifie la validité d'un coup à partir se la position initial et de la position finale
int isValidPositionJeu( char *plateau, int pos_init, int pos_final)
{
	if(plateau[pos_init]==1) //Pion Noir
  {
    if(plateau[pos_final]==0)//Il faut que la case soit vide pour pouvoir deplacer le pion 
    {
      if((pos_final==pos_init+WH_BOARD+1) && ((pos_init+1)%WH_BOARD!=0)) //Deplacement en diagonale à droite sans prendre
      {
        return 1;
      }
      if((pos_final==pos_init+WH_BOARD-1) && ((pos_init)%WH_BOARD!=0)) //Deplacement en diagonale à gauche sans prendre
      {
        return 1;
      }

      if((pos_final==pos_init+2*(WH_BOARD+1)) && ((pos_init+2)%WH_BOARD!=0) && (plateau[pos_init+WH_BOARD+1]==2 || plateau[pos_init+WH_BOARD+1]==4)) //Deplacement en diagonale à droite en prenant
      {
        return 1;
      }
      if((pos_final==pos_init+2*(WH_BOARD-1)) && ((pos_init-1)%WH_BOARD!=0) && (plateau[pos_init+WH_BOARD-1]==2 || plateau[pos_init+WH_BOARD-1]==4)) //Deplacement en diagonale à gauche en prenant
      {
        return 1;
      }
    }

  }

  if(plateau[pos_init]==2) //Pion Blanc
  {
    if(plateau[pos_final]==0)//Il faut que la case soit vide pour pouvoir deplacer le pion 
    {
      if((pos_final==pos_init-WH_BOARD+1) && ((pos_init+1)%WH_BOARD!=0)) //Deplacement en diagonale à droite sans prendre
      {
        return 1;
      }
      if((pos_final==pos_init-WH_BOARD-1) && ((pos_init)%WH_BOARD!=0)) //Deplacement en diagonale à gauche sans prendre
      {
        return 1;
      }

      if((pos_final==pos_init-2*(WH_BOARD+1)) && ((pos_init+2)%WH_BOARD!=0) && (plateau[pos_init-WH_BOARD-1]==1 || plateau[pos_init-WH_BOARD-1]==3)) //Deplacement en diagonale à droite en prenant
      {
        return 1;
      }
      if((pos_final==pos_init-2*(WH_BOARD-1)) && ((pos_init-1)%WH_BOARD!=0) && (plateau[pos_init-WH_BOARD+1]==1 || plateau[pos_init-WH_BOARD+1]==3)) //Deplacement en diagonale à gauche en prenant
      {
        return 1;
      }
    }
  }
  

  if(plateau[pos_init]==3) //Dame Noir
  {
    if(plateau[pos_final]==0)//Il faut que la case soit vide pour pouvoir deplacer la dame
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
            if(plateau[pos_init-i*(WH_BOARD-1)]==2 || plateau[pos_init-i*(WH_BOARD-1)]==4)
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
            if(plateau[i*(WH_BOARD+1)+pos_init]==2 || plateau[i*(WH_BOARD+1)+pos_init]==4)
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
            if(plateau[pos_init-i*(WH_BOARD+1)]==2 || plateau[pos_init-i*(WH_BOARD+1)]==4)
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
            if(plateau[i*(WH_BOARD-1)+pos_init]==2 || plateau[i*(WH_BOARD-1)+pos_init]==4)
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

  if(plateau[pos_init]==4) //Dame Blanche
  {
    if(plateau[pos_final]==0)//Il faut que la case soit vide pour pouvoir deplacer la dame
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
            if(plateau[pos_init-i*(WH_BOARD-1)]==1 || plateau[pos_init-i*(WH_BOARD-1)]==3)
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
            if(plateau[i*(WH_BOARD+1)+pos_init]==1 || plateau[i*(WH_BOARD+1)+pos_init]==3)
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
            if(plateau[pos_init-i*(WH_BOARD+1)]==1 || plateau[pos_init-i*(WH_BOARD+1)]==3)
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
            if(plateau[i*(WH_BOARD-1)+pos_init]==1 || plateau[i*(WH_BOARD-1)+pos_init]==3)
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
char *moveJeu( char *plateau, int pos_init, int pos_final)
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
        if(plateau[pos_init-i*(WH_BOARD-1)]!=0)
        {
          pos_pris=pos_init-i*(WH_BOARD-1);
        }
      }
      plateau[pos_pris]=0;
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
        if(plateau[pos_init+i*(WH_BOARD+1)]!=0)
        {
          pos_pris=pos_init+i*(WH_BOARD+1);
        }
      }
      plateau[pos_pris]=0;
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
        if(plateau[pos_init-i*(WH_BOARD+1)]!=0)
        {
          pos_pris=pos_init-i*(WH_BOARD+1);
        }
      }
      plateau[pos_pris]=0;
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
        if(plateau[pos_init+i*(WH_BOARD-1)]!=0)
        {
          pos_pris=pos_init+i*(WH_BOARD-1);
        }
      }
      plateau[pos_pris]=0;
    }
  }

  //Mouvements qui transforment en dame
  if(plateau[pos_init]==1 && pos_final>=MAX_BOARD-WH_BOARD)
  {
    plateau[pos_final]=3;
  }
  else if(plateau[pos_init]==2 && pos_final<WH_BOARD)
  {
    plateau[pos_final]=4;
  }
  else
  {
    plateau[pos_final]=plateau[pos_init];
  }

  plateau[pos_init]=0;
  
  return plateau;
}

//Fonction qui renvoie 1 si la partie est fini sinon 0
int partieFini(char *plateau)
{
    int blanc=0;
    int noir=0;
    for(int i=0;i<MAX_BOARD;i=i+1)
    {
        if(plateau[i]==1 || plateau[i]==3)
        {
            noir=noir+1;
        }
        if(plateau[i]==2 || plateau[i]==4)
        {
            blanc=blanc+1;
        }
    }
    if(blanc==0 || noir==0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//Remplis le plateau avec le coup de l'IA
char *coupIA(Item *node,char *plateau)
{
    for(int i=0;i<MAX_BOARD;i=i+1)
    {
        plateau[i]=node->board[i];
    }
    
    return plateau;
}