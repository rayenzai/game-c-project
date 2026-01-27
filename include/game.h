#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>

 
// --- DEFINE ---
#define TILE_SIZE 16          
#define LOGICAL_WIDTH 320
#define LOGICAL_HEIGHT 240
#define MAP_WIDTH 20        
#define MAP_HEIGHT 15  
#define NB_LEVELS 9 
#define PLAYER_SPEED 2.0f 


// #define FANTOME_SPEED 0.5f 



typedef struct {
    float x, y;     
    int w, h;       
} Joueur;

typedef struct{
    float x, y;
    int w, h;
    int direction; // 0: Haut, 1: Bas, 2: Gauche, 3: Droite
    int timer;     // Nombre de frames restantes avant de changer de direction
} Fantome;

extern Joueur player;
extern Fantome fantome;

extern float FANTOME_SPEED;

extern int maps[NB_LEVELS][MAP_HEIGHT][MAP_WIDTH];

void InitGame(SDL_Renderer *renderer);
void UpdateGame(void);
void DrawGame(SDL_Renderer *renderer, TTF_Font *font, TTF_Font *fontMini);
void ActionFantome();
void SpawnFantomeRandom();
void DrawTexte(char *texteAffiche, SDL_Renderer *renderer,TTF_Font *font, int x, int y, int w, int h);
void DrawInteractions(SDL_Renderer *renderer, SDL_Surface *sText);

extern int currentLevel; 

#endif