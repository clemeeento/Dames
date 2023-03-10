#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_ttf.h>

#include "list.h"
#include "board.h"
#include "minmax.h"
#include "jeu.h"

#define MAX_BOARD 100 //Taille total du tableau
#define WH_BOARD 10 //Dimension du tableau


#define COTETOT 10

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;
SDL_Surface *image = NULL;
SDL_Color blanc = {255,255,255,255};
int statut= EXIT_FAILURE;

 int start_x,  start_y,  end_x,  end_y;
 

void SDL_ExitwithError(const char *message)//message d'erreur quand il faut quitter le programme
{
     SDL_Log("Erreur : %s > %s\n",message, SDL_GetError());//permet d'indiquer les erreurs de la fonction SDL_INIT
     SDL_Quit();
     exit(statut); // c'est pour quitter l SDL dans tous les cas et fermer la denetre et desallouer la memoire
                
}

//Convertit le tableau d'une dimension en deux dimension
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

void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
   const int32_t diameter = (radius * 2);

   int32_t x = (radius - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);

   while (x >= y)
   {
      //  Each of the following renders an octant of the circle
      SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

      if (error <= 0)
      {
         ++y;
         error += ty;
         ty += 2;
      }

      if (error > 0)
      {
         --x;
         tx += 2;
         error += (tx - diameter);
      }
   }
}
void SDL_RenderFillCircle(SDL_Renderer *renderer, int x, int y, int radius)
{
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius - 1;
    status = 0;

    while (offsety >= offsetx)
    {

        status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
                                     x + offsety, y + offsetx);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
                                     x + offsetx, y + offsety);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
                                     x + offsetx, y - offsety);
        status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
                                     x + offsety, y - offsetx);

        if (status < 0)
        {
            status = -1;
            break;
        }

        if (d >= 2 * offsetx)
        {
            d -= 2 * offsetx + 1;
            offsetx += 1;
        }
        else if (d < 2 * (radius - offsety))
        {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else
        {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }
}

void DrawEllipse(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t xRadius, int32_t yRadius)
{
   int32_t x = 0;
   int32_t y = yRadius;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = tx - (xRadius * 2);

   while (x <= y)
   {
      SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);

      if (error <= 0)
      {
         ++x;
         error += ty;
         ty += 2;
      }

      if (error > 0)
      {
         --y;
         tx += 2;
         error += (tx - (xRadius * 2));
      }
   }
}
void SDL_RenderFillEllipse(SDL_Renderer *renderer, int x, int y, int xRadius, int yRadius)
{
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = yRadius;
    d = (yRadius * yRadius * (offsetx + 0.5) * (offsetx + 0.5)) +
        (xRadius * xRadius * (offsety - 1) * (offsety - 1)) -
        (xRadius * xRadius * yRadius * yRadius);

    status = 0;

    while (yRadius * yRadius * (offsetx + 0.5) <= xRadius * xRadius * (offsety))
    {

        status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
                                     x + offsetx, y + offsety);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
                                     x + offsetx, y - offsety);

        if (status < 0)
        {
            status = -1;
            break;
        }

        if (d >= 0)
        {
            d += (2 * yRadius * yRadius * offsetx) + (yRadius * yRadius);
            offsetx += 1;
        }
        else
        {
            d += (2 * yRadius * yRadius * offsetx) + (yRadius * yRadius) -
                 (2 * xRadius * xRadius * offsety);
            offsetx += 1;
            offsety -= 1;
        }
    }

    d = (yRadius * yRadius * (offsetx + 0.5) * (offsetx + 0.5)) +
        (xRadius * xRadius * (offsety - 1) * (offsety - 1)) -
        (xRadius * xRadius * yRadius * yRadius);

    while (offsety >= 0)
    {

        status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
                                     x + offsetx, y + offsety);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
                                     x + offsetx, y - offsety);

        if (status < 0)
        {
            status = -1;
            break;
        }

        if (d <= 0)
        {
            d += (2 * xRadius * xRadius * offsety) - (xRadius * xRadius);
            offsety -= 1;
        }
        else
        {
            d += (2 * (yRadius * yRadius - xRadius * xRadius) * offsety) -
                 (xRadius * xRadius);
            offsetx += 1;
            offsety -= 1;
        }
    }
}


int convertPosition(int x, int y) 
{
    int newX=(int)(x/100)-1;
    int newY=(int)y/100;

    return newY * WH_BOARD + newX;
}

int main()
{
  Item *node=nodeAlloc();
  Item *newNode=nodeAlloc();

  char *plateau=initGame();
  int tour=0;

  int **deuxDimension=convertTableau(plateau);


  //Afficher le plateau
  
          //lancement de SDL
            if(SDL_Init(SDL_INIT_VIDEO)!=0)//on utilise SDL_INIT_VIDEO permet dde creer des fenetre donc d'avoir un rendu
            {
                SDL_ExitwithError("Initialisation SDL");
            }
            

          //creation de la fenetre + rendu 

            if(SDL_CreateWindowAndRenderer(1200, 1000, SDL_WINDOW_SHOWN, &window, &renderer)!=0)
            {
                
                SDL_ExitwithError("impossibl de creer la  fenetre et rendu");
            }

        //ON change la couleur de l'arriere plan en blanc
   
            if(0 != SDL_SetRenderDrawColor(renderer, blanc.r, blanc.g, blanc.b, blanc.a))
            {
                 SDL_ExitwithError("impossibl changer la couleur pour le  rendu");
                
            }

            SDL_RenderClear(renderer);
            if(SDL_RenderClear(renderer) !=0)//on nettoie le renderer et on modifie la couleur de la fentre en blanc
            {
                 SDL_ExitwithError("pb dans l'effacement du rendu");
                
            }
   
        //on creer la grille 
       SDL_Rect rect[COTETOT][COTETOT];

            int i=0, j=0 ; 

            for ( i=0; i<COTETOT ; i ++)
            {
                //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                for ( j=0; j< COTETOT; j ++)     
                {
                
                    int32_t radius= 30; 
                    int crad =30;
                       
                        rect[i][j].x=100 + i*100;
                        rect[i][j].y=-10+ j*100;
                        rect[i][j].w=100;
                        rect[i][j].h=100;
                  
                //on creer l'effet damier
                if ((i + j) % 2 == 0) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);//blanc
                } else {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);//noir
                }
                
                SDL_RenderFillRect(renderer,&rect[i][j]);
                SDL_RenderDrawRect(renderer,&rect[i][j] );

                 // Dessin des pièces de dames
                 //pieces noirs
                     if (deuxDimension[j][i] == 1) {
            
                          SDL_SetRenderDrawColor(renderer, 31, 97, 141, 0);//bleu
                          SDL_RenderFillCircle(renderer,rect[i][j].x+50,rect[i][j].y+50, crad);
                    }
                    //pieces blanches

                     if (deuxDimension[j][i] == 2){
                        
                       SDL_SetRenderDrawColor(renderer, 19, 141, 117, 0);//vert

                       SDL_RenderFillCircle(renderer, rect[i][j].x+50,rect[i][j].y +50, crad);

                    }

                     if (deuxDimension[j][i] == 3)//dame noir
                     {
                      
                       SDL_SetRenderDrawColor(renderer,31, 97, 141, 0);//Dames Noir

                       //SDL_RenderFillCircle(renderer, rect[i][j].x+50,rect[i][j].y +50, radius);
                      
                      SDL_RenderFillEllipse(renderer,rect[i][j].x+50,rect[i][j].y +50, radius, radius);
                      

                    }
                     if (deuxDimension[j][i] == 4)//dame blanche
                     {
                        
                       SDL_SetRenderDrawColor(renderer, 19, 141, 117, 0);//Dames Blanche 

                       //SDL_RenderFillCircle(renderer, rect[i][j].x+50,rect[i][j].y +50, crad);

                       SDL_RenderFillEllipse(renderer,rect[i][j].x+50,rect[i][j].y +50, radius, radius);

                    }

                }
            }
        SDL_RenderPresent(renderer);
        SDL_Delay(1000);
    
  while(partieFini(plateau)==0)
  {
    if(tour%2==0 || tour==0)//Tour du joueur
    {
      int aJoue = 0;
      while(aJoue==0)
      {

        SDL_bool running = SDL_TRUE;
        int mouse_down = 0; // Pour savoir si le bouton de la souris est enfoncé ou non
        end_x=0;
         while(end_x==0)
         {
                SDL_Event event;
                //SDL_PumpEvents();

              while(SDL_PollEvent(&event))
              {
                    switch(event.type)
                    {
                        case SDL_QUIT:

                        running=SDL_FALSE;
                        
                        break;

                        case SDL_MOUSEBUTTONDOWN://Detecte quand le bouton de la souris est clique

                           printf("Clic en %dx / %dy\n",event.button.x,event.button.y);
                           if(event.button.button==SDL_BUTTON_LEFT)
                           {
                                printf("clic gauche!\n"); 
                                start_x = event.button.x;
                                start_y =  event.button.y;
                                mouse_down = 1; 
                                //int * init =inputSouris(&start_x, &start_y, &end_x, &end_y);
                                
                                
                           } 
                        break;    
                        
                        case SDL_MOUSEBUTTONUP:

                           printf("relache en %dx / %dy\n",event.button.x,event.button.y);
                            if(event.button.button== SDL_BUTTON_LEFT && mouse_down == 1)
                            {
                              printf("souris_relache\n");
                                        end_x=0;
                                        end_x = event.button.x;
                                        end_y = event.button.y;
                                        mouse_down = 0;
                                        //int* final= inputSouris(&start_x,&start_y, &end_x, &end_y);
                    
                            }

                        break;

                        case SDL_WINDOWEVENT:
                        //event.window.event
                        if(event.window.event==SDL_WINDOWEVENT_SHOWN)
                           printf("on ferme la fenetre\n");
                          
                        break;
                            
                        default: 
                         break;
                  }
                
              }
            
             SDL_RenderPresent(renderer);  
             SDL_Delay(1000);
         }
   
        int pos_init=convertPosition(start_x,start_y);//de l'input souris
        int pos_final=convertPosition(end_x,end_y);//de l'input souris
        printf("voila: %d\n",pos_init);

        if((plateau[pos_init]==2 || plateau[pos_init]==4))
        {
          if(isValidPositionJeu(plateau,pos_init,pos_final)==1)
          {
            plateau=moveJeu(plateau,pos_init,pos_final);
            aJoue=1;

            int **deuxDimension=convertTableau(plateau);
          
            //fermer la fenetre
            // on detruit le rendu de la fenetre
            SDL_DestroyRenderer(renderer);
             //on va detruire la fentre crée , comme on va quiter sdl il faut detruire la fentre
            SDL_DestroyWindow(window);
            //Afficher/actualiser avec le nouveau plateau
            SDL_RenderClear(renderer);


          //lancement de SDL
            if(SDL_Init(SDL_INIT_VIDEO)!=0)//on utilise SDL_INIT_VIDEO permet dde creer des fenetre donc d'avoir un rendu
            {
                SDL_ExitwithError("Initialisation SDL");
            }
            

          //creation de la fenetre + rendu 

            if(SDL_CreateWindowAndRenderer(1200, 1000, SDL_WINDOW_SHOWN, &window, &renderer)!=0)
            {
                
                SDL_ExitwithError("impossibl de creer la  fenetre et rendu");
            }

        //ON change la couleur de l'arriere plan en blanc
   
            if(0 != SDL_SetRenderDrawColor(renderer, blanc.r, blanc.g, blanc.b, blanc.a))
            {
                 SDL_ExitwithError("impossibl changer la couleur pour le  rendu");
                
            }

            SDL_RenderClear(renderer);
            if(SDL_RenderClear(renderer) !=0)//on nettoie le renderer et on modifie la couleur de la fentre en blanc
            {
                 SDL_ExitwithError("pb dans l'effacement du rendu");
                
            }
   

              //on creer la grille 
            SDL_Rect rect[COTETOT][COTETOT];

            int i=0, j=0 ; 

            for ( i=0; i<COTETOT ; i ++)
            {
                //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                for ( j=0; j< COTETOT; j ++)     
                {
                
                    int32_t radius= 30; 
                    int crad =30;
                       
                        rect[i][j].x=100 + i*100;
                        rect[i][j].y=-10+ j*100;
                        rect[i][j].w=100;
                        rect[i][j].h=100;
                  
                //on creer l'effet damier
                if ((i + j) % 2 == 0) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);//blanc
                } else {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);//noir
                }
                
                SDL_RenderFillRect(renderer,&rect[i][j]);
                SDL_RenderDrawRect(renderer,&rect[i][j] );

                 // Dessin des pièces de dames
                 //pieces noirs
                     if (deuxDimension[j][i] == 1) {
            
                          SDL_SetRenderDrawColor(renderer, 31, 97, 141, 0);//bleu
                          SDL_RenderFillCircle(renderer,rect[i][j].x+50,rect[i][j].y+50, crad);
                    }
                    //pieces blanches

                     if (deuxDimension[j][i] == 2){
                        
                       SDL_SetRenderDrawColor(renderer, 19, 141, 117, 0);//vert

                       SDL_RenderFillCircle(renderer, rect[i][j].x+50,rect[i][j].y +50, crad);

                    }

                     if (deuxDimension[j][i] == 3)//dame noir
                     {
                      
                       SDL_SetRenderDrawColor(renderer,31, 97, 141, 0);//Dames Noir

                       //SDL_RenderFillCircle(renderer, rect[i][j].x+50,rect[i][j].y +50, radius);
                      
                      SDL_RenderFillEllipse(renderer,rect[i][j].x+50,rect[i][j].y +50, radius, radius);
                      

                    }
                     if (deuxDimension[j][i] == 4)//dame blanche
                     {
                        
                       SDL_SetRenderDrawColor(renderer, 19, 141, 117, 0);//Dames Blanche 

                       //SDL_RenderFillCircle(renderer, rect[i][j].x+50,rect[i][j].y +50, crad);

                       SDL_RenderFillEllipse(renderer,rect[i][j].x+50,rect[i][j].y +50, radius, radius);

                    }
                    
                }
            }
          }
         
        }SDL_RenderPresent(renderer);
          SDL_Delay(1000);
      }
       
    }
    else
    {
      initBoard(node,plateau);
      newNode=minmax(node);
      plateau=coupIA(newNode,plateau);
      
      int **deuxDimension=convertTableau(plateau);
      //fermer fenetre
       // on detruit le rendu de la fenetre
        SDL_DestroyRenderer(renderer);
      //on va detruire la fentre crée , comme on va quiter sdl il faut detruire la fentre
        SDL_DestroyWindow(window);

      //Afficher/actualiser avec le nouveau plateau
      SDL_RenderClear(renderer);
       //lancement de SDL
            if(SDL_Init(SDL_INIT_VIDEO)!=0)//on utilise SDL_INIT_VIDEO permet dde creer des fenetre donc d'avoir un rendu
            {
                SDL_ExitwithError("Initialisation SDL");
            }
            

          //creation de la fenetre + rendu 

            if(SDL_CreateWindowAndRenderer(1200, 1000, SDL_WINDOW_SHOWN, &window, &renderer)!=0)
            {
                
                SDL_ExitwithError("impossibl de creer la  fenetre et rendu");
            }

        //ON change la couleur de l'arriere plan en blanc
   
            if(0 != SDL_SetRenderDrawColor(renderer, blanc.r, blanc.g, blanc.b, blanc.a))
            {
                 SDL_ExitwithError("impossibl changer la couleur pour le  rendu");
                
            }

            SDL_RenderClear(renderer);
            if(SDL_RenderClear(renderer) !=0)//on nettoie le renderer et on modifie la couleur de la fentre en blanc
            {
                 SDL_ExitwithError("pb dans l'effacement du rendu");
                
            }
   

              //on creer la grille 
            SDL_Rect rect[COTETOT][COTETOT];

            int i=0, j=0 ; 

            for ( i=0; i<COTETOT ; i ++)
            {
                //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                for ( j=0; j< COTETOT; j ++)     
                {
                
                    int32_t radius= 30; 
                    int crad =30;
                       
                        rect[i][j].x=100 + i*100;
                        rect[i][j].y=-10+ j*100;
                        rect[i][j].w=100;
                        rect[i][j].h=100;
                  
                //on creer l'effet damier
                if ((i + j) % 2 == 0) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);//blanc
                } else {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);//noir
                }
                
                SDL_RenderFillRect(renderer,&rect[i][j]);
                SDL_RenderDrawRect(renderer,&rect[i][j] );

                 // Dessin des pièces de dames
                 //pieces noirs
                     if (deuxDimension[j][i] == 1) {
            
                          SDL_SetRenderDrawColor(renderer, 31, 97, 141, 0);//bleu
                          SDL_RenderFillCircle(renderer,rect[i][j].x+50,rect[i][j].y+50, crad);
                    }
                    //pieces blanches

                     if (deuxDimension[j][i] == 2){
                        
                       SDL_SetRenderDrawColor(renderer, 19, 141, 117, 0);//vert

                       SDL_RenderFillCircle(renderer, rect[i][j].x+50,rect[i][j].y +50, crad);

                    }

                     if (deuxDimension[j][i] == 3)//dame noir
                     {
                      
                       SDL_SetRenderDrawColor(renderer,31, 97, 141, 0);//Dames Noir

                       //SDL_RenderFillCircle(renderer, rect[i][j].x+50,rect[i][j].y +50, radius);
                      
                      SDL_RenderFillEllipse(renderer,rect[i][j].x+50,rect[i][j].y +50, radius, radius);
                      

                    }
                     if (deuxDimension[j][i] == 4)//dame blanche
                     {
                        
                       SDL_SetRenderDrawColor(renderer, 19, 141, 117, 0);//Dames Blanche 

                       //SDL_RenderFillCircle(renderer, rect[i][j].x+50,rect[i][j].y +50, crad);

                       SDL_RenderFillEllipse(renderer,rect[i][j].x+50,rect[i][j].y +50, radius, radius);

                    }

                }
           }
    }
    tour=tour+1;


  

    }
return 0;
}

