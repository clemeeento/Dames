#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "minmax.h"


int evaluation(Item *node)
{
    int i=0 , cout=0 ;  //coût est la variable à retourner
    for(i=0;i<MAX_BOARD;i++)
    {
       if (node->board[i]==1)
           cout++ ;
       if (node->board[i]==3)
           cout=cout+3 ;
       if (node->board[i]==2)
           cout-- ;
       if (node->board[i]==4)
           cout=cout-3 ;
    }

    return cout ;

}


Item *generation_coups(Item* node, list_t listeCoups,int i,int j)
{
    if(i>=MAX_BOARD)
    {
        
        return listeCoups.last;
    }
    else
    {
        Item *newNode=nodeAlloc();
        newNode->depth=node->depth+1;
        newNode->parent=node;
        if(newNode->depth%2==0)//Coups des blancs
        {
            if(node->board[i]==2||node->board[i]==4)
            {
                if(isValidPosition(node,i,j)==1)
                {
                    initBoard(newNode,node->board);
                    newNode=move(newNode,i,j);
                    addFirst(&listeCoups,newNode);

                    if(j<MAX_BOARD) generation_coups(node,listeCoups,i,j+1);
                    else generation_coups(node,listeCoups,i+1,0);
                }
                else
                {
                    if(j<MAX_BOARD) generation_coups(node,listeCoups,i,j+1);
                    else generation_coups(node,listeCoups,i+1,0);
                }
                
            }
            else
            {
                generation_coups(node,listeCoups,i+1,0);
            }
        }
        else //Coup des noirs
        {
            if(node->board[i]==1 || node->board[i]==3)
            {
                if(isValidPosition(node,i,j)==1)
                {
                    initBoard(newNode,node->board);
                    newNode=move(newNode,i,j);
                    addFirst(&listeCoups,newNode);

                    if(j<MAX_BOARD) generation_coups(node,listeCoups,i,j+1);
                    else generation_coups(node,listeCoups,i+1,0);
                }
                else
                {
                    if(j<MAX_BOARD) generation_coups(node,listeCoups,i,j+1);
                    else generation_coups(node,listeCoups,i+1,0);
                }
                
            }
            else
            {
                generation_coups(node,listeCoups,i+1,0);
            }
        }
    }
}


Item *generationArbre(Item *node,Item *tmp)
{
    //printf("\nnode : %d\n",node==NULL);
    //printf("prev : %d\n",node->prev==NULL);
    //printf("parent : %d\n",node->parent==NULL);
    if(node->parent==NULL && tmp->h==1)
    {
        
        return tmp;
    }
    else
    {
        if(node->prev==NULL && node->depth!=0 && node->h==1)
        {
            node->parent->h=1;
            return generationArbre(node->parent,tmp);
        }
        else
        {
            if(node->prev!=NULL && node->depth!=DepthMax && node->h==1)
            {
                return generationArbre(node->prev,tmp);
            }
            else
            {
                if(node->depth==DepthMax)
                {
                    node->g=evaluation(node);
                    if(node->g>tmp->g)
                    {
                        tmp=node;
                        tmp->h=1;
                    }
                    if(node->prev==NULL)
                    {
                        node->parent->h=1;
                        return generationArbre(node->parent,tmp);
                    }
                    else
                    {
                        return generationArbre(node->prev,tmp);
                    }
                }
                else
                {
                    list_t listeCoups;
                    listeCoups.numElements=0;
                    int i=0;
                    int j=0;
                    return generationArbre(generation_coups(node,listeCoups,i,j),tmp);
                }
            }
        }
    }
}

//Fonction qui prend en entré un node avec le tableau contenant l'etat actuel du jeu et renvoyant un node avec le tableau avec le coup de l'IA en plus
Item * minmax(Item *node)
{
    Item *tmp=nodeAlloc();
    tmp->g=-60;
    tmp=generationArbre(node,tmp);
    for(int i=0;i<DepthMax-1;i=i+1)
    {
        tmp=tmp->parent;
        
    }
    return tmp;
}



