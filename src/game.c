#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "game.h"
#include "sons.h"
#include <stdio.h>
#include <math.h>

// --- CONSTANTES ---
#define TILE_SIZE 16        
#define PLAYER_SPEED 2.0f   
#define LOGICAL_WIDTH 320
#define LOGICAL_HEIGHT 240
#define MAP_WIDTH 20        
#define MAP_HEIGHT 15       

// Pour les sons
static Mix_Chunk *sonTransition = NULL;
static Mix_Chunk *sonPickUp = NULL;
static Mix_Chunk *sonOpenDoor = NULL;
static Mix_Chunk *sonCloseDoor = NULL;
  
// Pour les touches
static int toucheE_Relache = 1;
static int toucheEnter_Relache = 1;

// --- VARIABLES GLOBALES ---
typedef struct {
    float x, y;     
    int w, h;       
} Joueur;

static Joueur player;
static SDL_Texture *tilesetTexture = NULL; 
// static SDL_Texture *playerTexture = NULL; 


#define NB_LEVELS 5      
currentLevel = 0;   // 0 = Chambre, 1 = Couloir


/* 
0 = sol
1 = mur en brique 
2 = toit
3 = haut du lit
4 = bas du lit
5 = ancien perso
6 = doudou
7 = perso actuel
8 = haut gauche armoire fermée 
9 = haut droit armoire fermée
10 = bas gauche armoire fermée
11 = bas droit armoire fermée
12 = haut gauche armoire ouverte 
13 = haut droit armoire ouverte
14 = bas gauche armoire ouverte
15 = bas droit armoire ouverte
16 = haut gauche armoire ouverte doudou
17 = haut droit armoire ouverte doudou
18 = bas gauche armoire ouverte doudou
19 = bas droit armoire ouverte doudou
20 = livre 
21 = lampe 
22 = armoire vu de haut partie gauche
23 = armoire vu de haut partie droite
24,25,26 = bas tapis
27,28,29 = haut tapis
30,31 = tapis rond
*/

// --- LA CARTE DU NIVEAU ---
static int maps[NB_LEVELS][MAP_HEIGHT][MAP_WIDTH] = {
 {      //carte 1 (chambre)
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2}, // Trou en haut
        {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 8, 9, 1, 1}, 
        {1, 1, 0, 3, 21, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 11, 1, 1},
        {1, 1, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 30, 31, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 27, 28, 29, 0, 0, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 25, 26, 0, 0, 1, 1}, // Bas fermé
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 22, 23, 0, 0, 0, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    },
    //carte 2 (couloir)
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1}, // Haut (Suite)
        {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1}, // Toits cotés
        {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1}, // Murs cotés
        {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1}, // Vide (Sol)
        {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1}, // Bas (Entrée)
        {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1}
    }, 
    {      //carte 3 (hall)
        {2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2}, // Trou en haut
        {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 8, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, // Bas fermé
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1}
    },
         {      //carte 4 
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2}, // Trou en haut
        {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 8, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, // Bas fermé
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    },
    {   // carte
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2}, // Trou en haut
        {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 8, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}, // Bas fermé
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1}
    }
};

int dialogueStep = 0;
int dialogueStep_sortie1 = 0;
int dialogue_hasDoudou = 0;
int toucheRelache = 0;
int hasDoudou = 0;
SDL_Rect doudouRect = { 200, 150, 12, 12 };

int collision[] = {1, 2, 3, 4, 8, 9, 10, 11, 12, 13, 14, 15, 18, 19, 21, 22, 23}; // Rajouter dans ce tableau les int des collisions
int taille_collision = (int) (sizeof(collision) / sizeof(collision[0]));

// --- INITIALISATION ---
void InitGame(SDL_Renderer *renderer) {
    player.x = 100; 
    player.y = 100; 
    player.w = 12; 
    player.h = 12;
    dialogueStep = 1;
    toucheRelache = 0;
    hasDoudou = 0;

    // Chargement des sons
    sonTransition = chargement_son_transition();
    sonPickUp = chargement_son_item_pick_up();
    sonOpenDoor = chargement_son_door_open();
    sonCloseDoor = chargement_son_door_close();
    

    // Chargement du Tileset
    SDL_Surface *surface = SDL_LoadBMP("assets/tuille_into.bmp");
    if (surface) {
        SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0, 0));
        tilesetTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    } else {
        printf("ERREUR: Impossible de charger assets/tileset.bmp ! %s\n", SDL_GetError());
    }


}

int IsCollision(int type){
    for (int i = 0; i < taille_collision; ++i)
    {
        if(collision[i] == type) return 1;
    }
    return 0;
}

// Fonction utilitaire collision
int isWall(float x, float y) {
    int caseX = x / TILE_SIZE;
    int caseY = y / TILE_SIZE;
    if (caseX < 0 || caseX >= MAP_WIDTH || caseY < 0 || caseY >= MAP_HEIGHT) return 1;
    int type = maps[currentLevel][caseY][caseX];

    if (IsCollision(type)) return 1;
    return 0;
}

// --- UPDATE ---
void UpdateGame(void) {
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    // --- MODE DIALOGUE (Joueur Bloqué) ---
    if (dialogueStep > 0) {

        if (state[SDL_SCANCODE_RETURN]) {
            if (toucheRelache) {
                dialogueStep++;
                if (dialogueStep > 2) { 
                    dialogueStep = 0;  
                }
                toucheRelache = 0;
            }
        } else {
            toucheRelache = 1;
        }
        return;
    }
    if (dialogueStep_sortie1 > 0) {
        if (state[SDL_SCANCODE_RETURN]) {
            if (toucheRelache) {
                dialogueStep_sortie1 = 0; // On ferme le dialogue
                toucheRelache = 0;
            }
        } else {
            toucheRelache = 1;
        }
        return;
    }
    if (dialogue_hasDoudou > 0) {
        if (state[SDL_SCANCODE_RETURN]) {
            if (toucheRelache) {
                dialogue_hasDoudou ++;
                if (dialogue_hasDoudou > 3)
                {
                    dialogue_hasDoudou = 0;
                }
                
                toucheRelache = 0;
            }
        } else {
            toucheRelache = 1;
        }
        return;
    }

    float nextX = player.x;
    float nextY = player.y;

    if (state[SDL_SCANCODE_UP])    nextY -= PLAYER_SPEED;
    if (state[SDL_SCANCODE_DOWN])  nextY += PLAYER_SPEED;
    if (state[SDL_SCANCODE_LEFT])  nextX -= PLAYER_SPEED;
    if (state[SDL_SCANCODE_RIGHT]) nextX += PLAYER_SPEED;

    // Collision X
    int touchWallX = 0;
    if (isWall(nextX, player.y)) touchWallX = 1;
    if (isWall(nextX + player.w, player.y)) touchWallX = 1;
    if (isWall(nextX, player.y + player.h)) touchWallX = 1;
    if (isWall(nextX + player.w, player.y + player.h)) touchWallX = 1;

    if (!touchWallX) player.x = nextX;

    
    // Collision Y
    int touchWallY = 0;
    if (isWall(player.x, nextY)) touchWallY = 1;
    if (isWall(player.x + player.w, nextY)) touchWallY = 1;
    if (isWall(player.x, nextY + player.h)) touchWallY = 1;
    if (isWall(player.x + player.w, nextY + player.h)) touchWallY = 1;

    if (!touchWallY) player.y = nextY;

    float armoireX = 256 + 16; 
    float armoireY = 32 + 16; 

    // Calcul de la distance entre le joueur et l'armoire
    float dx = (player.x + player.w / 2) - armoireX;
    float dy = (player.y + player.h / 2) - armoireY;
    float distance = sqrt(dx*dx + dy*dy);

    if (state[SDL_SCANCODE_E]) {
        if (toucheE_Relache) {
            // Si le joueur est à moins de 16 pixel (une tuile)
            if (distance < 16 && currentLevel == 0) {
                
                // On vérifie si l'armoire est FERMÉE (8) (tuile en haut à gauche de l'armoire)
                // On l'ouvre avec le doudou dedans
                if (maps[0][1][16] == 8 && hasDoudou == 0) {
                    if (sonOpenDoor) Mix_PlayChannel(-1, sonOpenDoor, 0);
                    SDL_Delay(250);
                    maps[0][1][16] = 16; 
                    maps[0][1][17] = 17; 
                    maps[0][2][16] = 18; 
                    maps[0][2][17] = 19; 
                }
                // Si elle est OUVERTE (16 = avec le doudou dedans), on peut la refermer
                else if (maps[0][1][16] == 16 || maps[0][1][16] == 12) {
                    if (sonCloseDoor) Mix_PlayChannel(-1, sonCloseDoor, 0);
                    SDL_Delay(250);
                    maps[0][1][16] = 8;
                    maps[0][1][17] = 9;
                    maps[0][2][16] = 10;
                    maps[0][2][17] = 11;
                }
                else{
                    if (sonOpenDoor) Mix_PlayChannel(-1, sonOpenDoor, 0);
                    SDL_Delay(250);
                    maps[0][1][16] = 12; 
                    maps[0][1][17] = 13; 
                    maps[0][2][16] = 14; 
                    maps[0][2][17] = 15; 
                }
            }
            toucheE_Relache = 0; // On verrouille tant qu'on n'a pas lâché E
        }
    } else {
        toucheE_Relache = 1; // On a lâché la touche E on peut re appuyer
    }

    if (state[SDL_SCANCODE_RETURN] || state[SDL_SCANCODE_KP_ENTER])
    {
        if(toucheEnter_Relache){
            if(distance < 16 && currentLevel==0){
                if (maps[0][1][16] == 16)
                {
                    if (sonPickUp) Mix_PlayChannel(-1, sonPickUp, 0);
                    SDL_Delay(250);
                    maps[0][1][16] = 12; 
                    maps[0][1][17] = 13; 
                    maps[0][2][16] = 14; 
                    maps[0][2][17] = 15; 
                    hasDoudou = 1; 
                    dialogue_hasDoudou = 1;
                }

            }
        }
        toucheEnter_Relache = 0;
    }
    else{
            toucheEnter_Relache = 1;
    }
    

    // 1. Quitter la CHAMBRE (Niveau 0) par le HAUT
    // On vérifie si on est au niveau 0 ET si on dépasse le haut de l'écran (y < 5)
    if (currentLevel == 0 && player.y < 5) {
        if (hasDoudou == 1) {
            
            if (sonTransition) Mix_PlayChannel(-1, sonTransition, 0);
            SDL_Delay(300);


            currentLevel = 1; 
            player.y = (MAP_HEIGHT * TILE_SIZE) - 20;
            

    }
    else{
        player.y = 10;
        dialogueStep_sortie1 = 1;
    }
    }
    // 2. Quitter le COULOIR (Niveau 1) par le BAS
    // On vérifie si on est au niveau 1 ET si on dépasse le haut de l'écran
    else if (currentLevel == 1 && player.y > (MAP_HEIGHT * TILE_SIZE) - 20) {
        if (sonTransition) Mix_PlayChannel(-1, sonTransition, 0);
        SDL_Delay(300);
        currentLevel = 0;  // On retourne à la CHAMBRE
        player.y = 10;     // On apparaît tout en HAUT de la chambre
        
    }
    
if (currentLevel == 1 && player.y < 5) {
        if (hasDoudou == 1) {
            if (sonTransition) Mix_PlayChannel(-1, sonTransition, 0);
            SDL_Delay(300);
            currentLevel = 2; 
            player.y = (MAP_HEIGHT * TILE_SIZE) - 20;
    }
    else{
        player.y = 10;
    }
    }
    else if (currentLevel == 2 && player.y > (MAP_HEIGHT * TILE_SIZE) - 20) {
        if (sonTransition) Mix_PlayChannel(-1, sonTransition, 0);
        SDL_Delay(300);
        currentLevel = 1;  // On retourne à la CHAMBRE
        player.y = 10;     // On apparaît tout en HAUT de la chambre
        
    }
    
    if (currentLevel == 2 && player.x < 5) {
        if (hasDoudou == 1) {
            if (sonTransition) Mix_PlayChannel(-1, sonTransition, 0);
            SDL_Delay(300);
            currentLevel = 3; 
            player.x = (MAP_WIDTH * TILE_SIZE) - 20;
    }
    else{
        player.x = 3;
    }
    }
    else if (currentLevel == 3 && player.x > (MAP_WIDTH * TILE_SIZE) - 20) {
        if (sonTransition) Mix_PlayChannel(-1, sonTransition, 0);
        SDL_Delay(300);
        currentLevel = 2;  // On retourne à la CHAMBRE
        player.x = 3;     // On apparaît tout en HAUT de la chambre
        
    }
        
if (currentLevel == 3 && player.y < 5) {
        if (hasDoudou == 1) {
            if (sonTransition) Mix_PlayChannel(-1, sonTransition, 0);
            SDL_Delay(300);
            currentLevel = 4; 
            player.y = (MAP_HEIGHT * TILE_SIZE) - 20;
    }
    else{
        player.y = 10;
    }
    }
    else if (currentLevel == 4 && player.y > (MAP_HEIGHT * TILE_SIZE) - 20) {
        if (sonTransition) Mix_PlayChannel(-1, sonTransition, 0);
        SDL_Delay(300);
        currentLevel = 3;  // On retourne à la CHAMBRE
        player.y = 10;     // On apparaît tout en HAUT de la chambre
        
    }




}

// Retourne 1 si la case est dans la lumière sinon 0.
int estEclaire(int gridX, int gridY, int rayon) {
    
    if (gridX < 0 || gridX >= MAP_WIDTH || gridY < 0 || gridY >= MAP_HEIGHT) {
        return 0; 
    }

    // 2. Calcul de distance 
    int tileCenterX = (gridX * TILE_SIZE) + (TILE_SIZE / 2);
    int tileCenterY = (gridY * TILE_SIZE) + (TILE_SIZE / 2);
    int playerCenterX = (int)player.x + (player.w / 2);
    int playerCenterY = (int)player.y + (player.h / 2);

    int dx = tileCenterX - playerCenterX;
    int dy = tileCenterY - playerCenterY;
    double distance = sqrt(dx*dx + dy*dy);

    if (distance <= rayon) {
        return 1; 
    }
    return 0; // C'est éteint
}

// --- DESSIN ---
void DrawGame(SDL_Renderer *renderer, TTF_Font *font) {
    // 1. Fond Noir
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // 2. Dessiner la CARTE avec le tileset
    if (tilesetTexture) {
        // int rayonLumiere = 0;
        int rayon = 0; 
        if (hasDoudou == 1) {
            // rayonLumiere = 60; 
            rayon = 60;
        } else {
            // rayonLumiere = 30;
            rayon = 30;
        }
        
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                
                if (estEclaire(x, y, rayon) == 0) {
                    continue; 
                }

                // --- A. DESSINER LA TUILE ---
                int type = maps[currentLevel][y][x]; 
                SDL_Rect srcRect = { type * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE };
                SDL_Rect destRect = { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
                SDL_RenderCopy(renderer, tilesetTexture, &srcRect, &destRect);

                // --- B. DESSINER LE CONTOUR (Bords pixelisés) ---
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Blanc

                int px = x * TILE_SIZE;
                int py = y * TILE_SIZE;
                int ts = TILE_SIZE; // 16

                // 1. Vérifier le voisin du HAUT (y-1)
                // Si le voisin du haut est éteint, on dessine une ligne en haut de notre case
                if (estEclaire(x, y - 1, rayon) == 0) {
                    SDL_RenderDrawLine(renderer, px, py, px + ts, py);
                }

                // 2. Vérifier le voisin du BAS (y+1)
                if (estEclaire(x, y + 1, rayon) == 0) {
                    SDL_RenderDrawLine(renderer, px, py + ts, px + ts, py + ts);
                }

                // 3. Vérifier le voisin de GAUCHE (x-1)
                if (estEclaire(x - 1, y, rayon) == 0) {
                    SDL_RenderDrawLine(renderer, px, py, px, py + ts);
                }

                // 4. Vérifier le voisin de DROITE (x+1)
                if (estEclaire(x + 1, y, rayon) == 0) {
                    SDL_RenderDrawLine(renderer, px + ts, py, px + ts, py + ts);
                }
            }
        }
    }
    //dialogues
    if (dialogueStep > 0) {
        char *texteAffiche = "";
        if (dialogueStep == 1) texteAffiche = "Maman ? Papa ? Il fait tout noir...";
        if (dialogueStep == 2) texteAffiche = "J'ai peur... Ou est mon Doudou ?";

        SDL_Rect rectBulle = { 20, 180, 280, 50 };
        
        // Fond blanc
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rectBulle);
        
        // Bordure noire
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &rectBulle);

        // 3. Dessiner le Texte
        SDL_Color couleurNoire = {0, 0, 0, 255};
        SDL_Surface *surfaceTexte = TTF_RenderText_Solid(font, texteAffiche, couleurNoire);
        if (surfaceTexte) {
            SDL_Texture *textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);
            SDL_Rect rectTexte = { 
                rectBulle.x + 10, 
                rectBulle.y + 15, 
                surfaceTexte->w, 
                surfaceTexte->h 
            };
            SDL_RenderCopy(renderer, textureTexte, NULL, &rectTexte);
            
            SDL_FreeSurface(surfaceTexte);
            SDL_DestroyTexture(textureTexte);
        }
    }
    if (dialogueStep_sortie1 > 0) {
        char *texteAffiche = "";
        if (dialogueStep_sortie1 == 1) texteAffiche = "je peux pas sortir sans mon doudou...";

        SDL_Rect rectBulle = { 20, 180, 280, 50 };
        
        // Fond blanc
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rectBulle);
        
        // Bordure noire
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &rectBulle);

        // 3. Dessiner le Texte
        SDL_Color couleurNoire = {0, 0, 0, 255};
        SDL_Surface *surfaceTexte = TTF_RenderText_Solid(font, texteAffiche, couleurNoire);
        if (surfaceTexte) {
            SDL_Texture *textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);
            SDL_Rect rectTexte = { 
                rectBulle.x + 10, 
                rectBulle.y + 15, 
                surfaceTexte->w, 
                surfaceTexte->h 
            };
            SDL_RenderCopy(renderer, textureTexte, NULL, &rectTexte);
            
            SDL_FreeSurface(surfaceTexte);
            SDL_DestroyTexture(textureTexte);
        }
    }
    if (dialogue_hasDoudou > 0) {
        char *texteAffiche = "";
        if (dialogue_hasDoudou == 1) texteAffiche = "je te tiens";
        if (dialogue_hasDoudou == 2) texteAffiche = "OH...";
        if (dialogue_hasDoudou == 3) texteAffiche = "De la lumiere !";

        SDL_Rect rectBulle = { 20, 180, 280, 50 };
        
        // Fond blanc
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rectBulle);
        
        // Bordure noire
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &rectBulle);

        // 3. Dessiner le Texte
        SDL_Color couleurNoire = {0, 0, 0, 255};
        SDL_Surface *surfaceTexte = TTF_RenderText_Solid(font, texteAffiche, couleurNoire);
        if (surfaceTexte) {
            SDL_Texture *textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);
            SDL_Rect rectTexte = { 
                rectBulle.x + 10, 
                rectBulle.y + 15, 
                surfaceTexte->w, 
                surfaceTexte->h 
            };
            SDL_RenderCopy(renderer, textureTexte, NULL, &rectTexte);
            
            SDL_FreeSurface(surfaceTexte);
            SDL_DestroyTexture(textureTexte);
        }
    }


    SDL_Rect srcPlayer = { 112, 0, 16, 16 };
    SDL_Rect destPlayer = { (int)player.x - 2, (int)player.y - 2, 16, 16 };
    SDL_RenderCopy(renderer, tilesetTexture, &srcPlayer, &destPlayer);
}
