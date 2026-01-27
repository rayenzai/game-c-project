#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "game.h"
#include "sons.h"
#include "map.h"
#include <stdio.h>
#include <math.h>

// --- CONSTANTES ---
#define TILE_SIZE 16        
#define PLAYER_SPEED 2.0f   
#define LOGICAL_WIDTH 320
#define LOGICAL_HEIGHT 240
#define MAP_WIDTH 20        
#define MAP_HEIGHT 15     
#define BED_TILE_X 3
#define BED_TILE_Y 1
#define BED_TILE_W 2
#define BED_TILE_H 3
#define PLAYER_HITBOX_INSET 2
#define CHAIR_TILE_X 6
#define CHAIR_TILE_Y 12
#define CHAIR_TILE_W 1
#define CHAIR_TILE_H 1

// -- Pour les sons -- 

// Sound Effects
// static Mix_Chunk *sonTransition = NULL;
static Mix_Chunk *sonPickUp = NULL;
static Mix_Chunk *sonOpenDoor = NULL;
static Mix_Chunk *sonCloseDoor = NULL;

// Musiques D'ambiance
static Mix_Music *MusicInterior = NULL;
static Mix_Music *MusicExterior = NULL;

// Pour les touches
static int toucheE_Relache = 1;
static int toucheEnter_Relache = 1;
static int toucheSpace_Relache = 1;
static int toucheX_Relache = 1;
static int toucheI_Relache = 1;
static int toucheT_Relache = 1;
static int isSleeping = 0;
static int isSitting = 0;
static int isHidingInTent = 0;
static int showSleepPrompt = 0;
static int showSitPrompt = 0;
static int showPhone = 0;
static int showTentPrompt = 0;

// --- VARIABLES GLOBALES ---
typedef struct {
    float x, y;     
    int w, h;       
} Joueur;

static Joueur player;
static SDL_Texture *tilesetTexture = NULL; 
// static SDL_Texture *playerTexture = NULL; 

int rayon = 0;

#define NB_LEVELS 9   
int currentLevel = 0;   // 0 = Chambre, 1 = Couloir


/* 
0,1 = sol en bois 
2 = mur en brique
3 = cube 1
4 = cube 2
5 = affiche perso
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
32,33 = haut grand lit bb 
34,35 = bas grand lit bb
36,37 = déco haut du lit
38,39 = petit train
40 = avion 
41 = dino
42,43 = commode mur gauche
44 = cubes
45 = cubes 
46,47,48,49 = bureau enfant
50 = cube noir
51 = haut chaise 
52,53,54 = bas tente
55,56,57 = haut tente
*/

// --- LA CARTE DU NIVEAU ---
static int maps[NB_LEVELS][MAP_HEIGHT][MAP_WIDTH] = {
 {      //carte 1 (chambre)
        {2, 2,  2,  2,  2,  2,  2,  2,  0,  0, 0, 0, 2,  2,  5,  2,  8,  9,  2, 2}, // Trou en haut   
        {2, 2,  2, 36, 37,  2,  2,  2,  0,  1, 0, 0, 2,  2, 41,  2, 10, 11,  2, 2}, 
        {2, 1,  0, 32, 33, 21,  0,  1,  0,  1, 0, 1, 0,  1,  0,  1,  0,  1,  0, 2},
        {2, 1,  0, 34, 35,  1,  0,  1,  0,  1, 0, 1, 0,  1,  0,  1,  0,  1,  0, 2},
        {2, 1, 30, 31,  0,  1,  0,  1,  0,  1, 0, 1, 0,  1,  0,  1,  0,  1,  0, 2},
        {2, 1,  0, 20,  0,  1,  0,  1,  0,  1, 0, 1, 0,  1,  0,  1,  0,  1,  0, 2},
        {2, 42, 0,  1,  0,  1,  0,  1,  0,  1, 0, 1, 0,  1,  0,  1, 55, 56, 57, 2},
        {2, 43, 0,  1,  0,  1,  0,  1,  0,  1, 0, 1, 0,  1,  0,  1, 52, 53, 54, 2},
        {2, 1,  0,  1,  0,  3, 44,  1,  0,  1, 0, 1, 0,  1,  0,  1,  0,  1,  0, 2},
        {2, 1,  0, 40,  0,  1,  4,  1,  0,  1, 0, 1, 0,  1,  0,  1,  0,  1,  0, 2},
        {2, 1,  0,  1, 38, 39,  0,  1,  0,  1, 0, 1, 0,  1,  0,  1,  0,  1,  0, 2},
        {2, 1,  0,  1,  0,  1,  0,  1,  0,  1, 0, 1, 0, 27, 28, 29,  0,  1,  0, 2},
        {2, 1,  0,  1,  0,  1, 51,  1,  0,  1, 0, 1, 0, 24, 25, 26,  0,  1,  0, 2}, // Bas fermé
        {2, 1,  0,  1,  0,  1, 46, 47, 48, 49, 0, 1, 0, 22, 23,  0,  0,  1,  0, 2},
        {2, 2,  2,  2,  2,  2,  2,  2,  2,  2, 2, 2, 2,  2,  2,  2,  2,  2,  2, 2}  // Bas fermé
    },
    // CARTE 2 : COULOIR (Niveau 1)
    {
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2}, // Haut (Suite)
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2}, // Toits cotés
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2}, // Murs cotés
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2}, // Vide (Sol)
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2}, // Bas (Entrée)
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2}
    }, 
    // CARTE 3 : HALL (Niveau 2)
    {       
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}, // Trou en haut (tout fermé en 2 ici ?)
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 8, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Bas fermé
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2}
    },
    // CARTE 4 (Niveau 3)
    {       
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2}, // Trou en haut
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 8, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2}, // Bas fermé
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}
    },
    // CARTE 5 (Niveau 4 - dernière du bloc précédent)
    {    
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2}, // Trou en haut
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 8, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2}, // Bas fermé
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2}
    },
    // --- LABYRINTHE 1 (Index 4) ---
    {
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}, 
        {2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 2}, 
        {2, 1, 2, 1, 2, 1, 2, 2, 2, 1, 2, 1, 2, 2, 2, 2, 2, 2, 1, 2},
        {2, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 2},
        {2, 1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1, 2}, 
        {2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2},
        {2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2}, 
        {2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2}, 
        {2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2}, 
        {2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2},
        {2, 1, 2, 2, 2, 2, 2, 1, 2, 1, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2}, 
        {1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 2, 1, 2},
        {1, 1, 2, 1, 2, 1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 1, 1}, // SORTIE DROITE (Ligne 12)
        {2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 2}, 
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}  
    },

    // --- LABYRINTHE 2 (Index 5) ---
    {
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}, // Mur Haut
        {2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2},
        {2, 1, 2, 2, 2, 1, 2, 1, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1, 2},
        {2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 2},
        {2, 1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1, 2, 2, 2, 2, 1, 2}, // Gros bloc central
        {2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2},
        {2, 2, 2, 1, 2, 1, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2},
        {2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 2},
        {2, 1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1, 2, 1, 2, 2, 2},
        {2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 2},
        {2, 1, 2, 1, 2, 2, 2, 2, 2, 1, 2, 1, 2, 2, 2, 2, 1, 2, 1, 2},
        {2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 2},
        {1, 1, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2}, // ENTRÉE GAUCHE (Ligne 12, connectée à l'autre)
        {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2}  // SORTIE BAS (Ligne 14, Colonnes 10-11)
    },

    // --- LABYRINTHE 3 (Index 6) ---
    {
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2}, // ENTRÉE HAUT (Alignée avec sortie précédente)
        {2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2}, // Zone ouverte pour tromper
        {2, 1, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1, 2},
        {2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 2}, // Début des fausses pistes
        {2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1, 2, 2, 1, 2, 1, 2},
        {2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 2, 1, 2},
        {2, 1, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1, 2, 2, 1, 2},
        {2, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2}, // Grand couloir transversal
        {2, 1, 2, 2, 2, 1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}, // Cul de sac massif à droite
        {2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2},
        {2, 2, 2, 1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2},
        {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2},
        {1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2}, // SORTIE GAUCHE (Ligne 12)
        {2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}  // Mur Bas fermé
    },

    // --- LABYRINTHE 4 (Index 7) ---
    {
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}, // Mur Haut
        {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2}, // Chemin du haut
        {2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2},
        {2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2},
        {2, 1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1, 2},
        {2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2},
        {2, 1, 2, 1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 1, 2, 1, 2},
        {2, 1, 2, 1, 2, 1, 2, 1, 1, 6, 1, 1, 1, 2, 1, 2, 1, 2, 1, 2}, // <--- LE DOUDOU (6) EST ICI
        {2, 1, 2, 1, 2, 1, 2, 2, 2, 1, 2, 2, 1, 2, 1, 2, 1, 2, 1, 2},
        {2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2, 1, 1, 1, 2},
        {2, 1, 2, 1, 2, 2, 2, 2, 2, 2, 1, 2, 1, 2, 2, 2, 2, 2, 1, 2},
        {2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1}, // ENTRÉE DROITE (Ligne 12)
        {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2}, // Fausse piste en bas
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}
    },
};

int dialogueStep = 0;
int dialogueStep_sortie1 = 0;
int dialogue_hasDoudou = 0;
int toucheRelache = 0;
int hasDoudou = 0;
int showInteractPrompt = 0;
int showInteractPrompt2 = 0;
int showInteractPrompt3 = 0;
SDL_Rect doudouRect = { 200, 150, 12, 12 };

static int isBedTileIndex(int type) {
    return (type >= 32 && type <= 37);
}

static int isDeskTileIndex(int type) {
    return (type >= 46 && type <= 49) || type == 51;
}

static int isCabinetTileIndex(int type) {
    return (type >= 42 && type <= 43);
}

int TuilesNotSpecial[] = {0, 1, 2};
int tailleTuilesNotSpecial = (int)sizeof(TuilesNotSpecial) / (int)sizeof(TuilesNotSpecial[0]);

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
    // sonTransition = chargement_son_transition();
    sonPickUp = chargement_son_item_pick_up();
    sonOpenDoor = chargement_son_door_open();
    sonCloseDoor = chargement_son_door_close();
    MusicInterior = chargement_son_ambiance();
    MusicExterior = chargement_son_exterieur();
    

    // Chargement du Tileset
    SDL_Surface *surface = SDL_LoadBMP("assets/tuille_into.bmp");
    if (surface) {
        SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 0, 255));
        tilesetTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    } else {
        printf("ERREUR: Impossible de charger assets/tileset.bmp ! %s\n", SDL_GetError());
    }


}

// Fonction utilitaire collision
int isWall(float x, float y) {
    int caseX = x / TILE_SIZE;
    int caseY = y / TILE_SIZE;
    if (caseX < 0 || caseX >= MAP_WIDTH || caseY < 0 || caseY >= MAP_HEIGHT) return 1;
    int type = maps[currentLevel][caseY][caseX];
    // int type_pattern = maps_patern[currentLevel][caseY][caseX];

    if (isBedTileIndex(type) || isDeskTileIndex(type) || isCabinetTileIndex(type)) {
        return 1;
    }
    
    // Collision avec la tente (tiles 52-57) - sauf si le joueur est caché dedans
    if ((type >= 52 && type <= 57) && !isHidingInTent) {
        return 1;
    }

    // --- TYPE 1 : MURS AVEC EFFET 3D/PERSPECTIVE ---
    // Les murs ont une profondeur : on peut passer "derrière" mais pas à travers

    if (type == 2 || type == 5){
        // Vérifier les cases adjacentes dans toutes les directions
        int caseY_Below = caseY + 1;
        int caseY_Above = caseY - 1;
        int caseX_Left = caseX - 1;
        int caseX_Right = caseX + 1;
        
        int typeBelow = -1;
        int typeAbove = -1;
        int typeLeft = -1;
        int typeRight = -1;
        
        if (caseY_Below < MAP_HEIGHT) {
            typeBelow = maps[currentLevel][caseY_Below][caseX];
        }
        if (caseY_Above >= 0) {
            typeAbove = maps[currentLevel][caseY_Above][caseX];
        }
        if (caseX_Left >= 0) {
            typeLeft = maps[currentLevel][caseY][caseX_Left];
        }
        if (caseX_Right < MAP_WIDTH) {
            typeRight = maps[currentLevel][caseY][caseX_Right];
        }
        
        // --- PRIORITÉ 1 : MURS VERTICAUX (gauche, droite, bas) ---
        // Si le mur a un voisin au-dessus OU en dessous, c'est un mur vertical → solide
        if (typeBelow == 2 || typeBelow == 5 || typeAbove == 2 || typeAbove == 5) {
            return 1; // Complètement solide
        }
        
        // --- PRIORITÉ 2 : MURS LATÉRAUX (bordures gauche/droite) ---
        // Si le mur a du vide/sol à gauche OU à droite, c'est une bordure latérale → solide
        if (typeLeft != 2 && typeLeft != 5 && typeLeft != -1) {
            return 1; // Mur de gauche = complètement solide
        }
        if (typeRight != 2 && typeRight != 5 && typeRight != -1) {
            return 1; // Mur de droite = complètement solide
        }
        
        // Bords de la carte = complètement solides
        if (caseX_Left < 0 || caseX_Right >= MAP_WIDTH || caseY_Below >= MAP_HEIGHT || caseY_Above < 0) {
            return 1;
        }
        
        // --- PRIORITÉ 3 : MURS DE FACE (horizontaux du haut) - Effet 3D ---
        // Si le mur n'a que des voisins murs ou est isolé, c'est un "mur de face"
        // Effet de profondeur : on peut passer derrière la partie basse
        int localY = (int)y % TILE_SIZE; 

        if (localY < 6) {
            return 1; // Partie haute = solide
        } else {
            return 0; // Partie basse = traversable (effet 3D)
        }
    }


    // --- TYPE 2 : MEUBLES AVEC PROFONDEUR (Placard 8 et 9) ---
    // On veut que le haut du meuble soit traversable (effet de perspective)
    // et que seul le bas bloque les pieds du joueur.
    if (type == 10 || type == 11 || type == 14 || type == 15 || type == 18 || type == 19) {
        int localY = (int)y % TILE_SIZE; // Position de 0 à 15 dans la case

        // Hitbox : Seulement les 8 pixels du bas sont solides
        if (localY < 4) {
            return 1;
        } else {
            return 0; // Le haut est traversable (on passe "derrière")
        }
    }

    if (type == 22 || type == 23)
    {
         int localY = (int)y % TILE_SIZE; // Position de 0 à 15 dans la case

        // Hitbox : Seulement les 8 pixels du bas sont solides
        if (localY > 2) {
            return 1;
        } else {
            return 0; // Le haut est traversable (on passe "derrière")
        }
    }
    if (type == 20)
    {
        return 1;
    }
    if (type == 21){
        return 1;
    }
    return 0;
}

// -- Fonctions pour vérification de directions et d'emplacements

// Entrer le xDébut et xFin du trou, CurrLvl correspond au lvl courant (<==> currentLevel),
// yDiff correpond à la marge de détection que vous accordez au joueur pour passer à l'autre salle 
// à combien de blocks de la porte vous accordez le joueur pour pouvoir passer d'une salle à l'autre

int IsLocationUp(int xDebut, int xFin, int CurrLvl, int yDiff){
    return (currentLevel == CurrLvl && player.y < yDiff && player.x >= (xDebut * TILE_SIZE) && player.x <= (xFin * TILE_SIZE) );
}

int IsLocationDown(int xDebut, int xFin, int CurrLvl, int yDiff){
    return (currentLevel == CurrLvl && player.y > (MAP_HEIGHT * TILE_SIZE) - yDiff && player.x >= (xDebut * TILE_SIZE) && player.x <= (xFin * TILE_SIZE) );
}

int IsLocationRight(int yDebut, int yFin, int CurrLvl, int xDiff){
    return (currentLevel == CurrLvl && player.x > (MAP_WIDTH * TILE_SIZE) - xDiff && player.y >= (yDebut * TILE_SIZE) && player.y <= (yFin * TILE_SIZE) );
}

int IsLocationLeft(int yDebut, int yFin, int CurrLvl, int xDiff){
    return (currentLevel == CurrLvl && player.x < xDiff && player.y >= (yDebut * TILE_SIZE) && player.y <= (yFin * TILE_SIZE) );
}
        
void ManageMusic() {
    static int currentZoneState = -1; 
    int newZoneState = 0; 

    // Si on est dans les niveaux 5, 6, 7 ou 8, on est à l'EXTERIEUR
    if (currentLevel >= 5 && currentLevel <= 8) {
        newZoneState = 1;
    } else {
        newZoneState = 0;
    }

    // Si la zone a changé (ex: on passe de 4 à 5, ou au démarrage du jeu)
    if (newZoneState != currentZoneState) {
        Mix_VolumeMusic(32);
        if (newZoneState == 1) {
            if (MusicExterior) Mix_FadeInMusic(MusicExterior, -1, 1000); 
        } 
        else {
            if (MusicInterior) Mix_FadeInMusic(MusicInterior, -1, 1000);
        }
        currentZoneState = newZoneState;
    }

}

static int touchesWall(float x, float y, int w, int h) {
    float left   = x + PLAYER_HITBOX_INSET;
    float right  = x + w - PLAYER_HITBOX_INSET;
    float top    = y + PLAYER_HITBOX_INSET;
    float bottom = y + h - PLAYER_HITBOX_INSET;

    if (isWall(left, top)) return 1;
    if (isWall(right, top)) return 1;
    if (isWall(left, bottom)) return 1;
    if (isWall(right, bottom)) return 1;
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

    SDL_Rect bedRectPx = { BED_TILE_X * TILE_SIZE, BED_TILE_Y * TILE_SIZE, BED_TILE_W * TILE_SIZE, BED_TILE_H * TILE_SIZE };
    int nearBed = 0;
    if (currentLevel == 0) {
        SDL_Rect playerRect = { (int)player.x, (int)player.y, player.w, player.h };
        SDL_Rect bedReachRect = { bedRectPx.x - 8, bedRectPx.y - 8, bedRectPx.w + 16, bedRectPx.h + 16 };
        nearBed = SDL_HasIntersection(&playerRect, &bedReachRect);
    }
    showSleepPrompt = (nearBed && !isSleeping);

    if (isSleeping) {
        if (state[SDL_SCANCODE_SPACE]) {
            if (toucheSpace_Relache) {
                isSleeping = 0;
                toucheSpace_Relache = 0;
                player.x = bedRectPx.x + (bedRectPx.w / 2) - (player.w / 2);
                player.y = bedRectPx.y + bedRectPx.h + 2; // Wake up at foot of bed
            }
        } else {
            toucheSpace_Relache = 1;
        }
        return;
    }

    if (nearBed && state[SDL_SCANCODE_SPACE]) {
        if (toucheSpace_Relache) {
            isSleeping = 1;
            toucheSpace_Relache = 0;
            player.x = bedRectPx.x + (bedRectPx.w / 2) - (player.w / 2);
            player.y = bedRectPx.y + TILE_SIZE; // Place player on bed
        }
        return;
    } else if (!state[SDL_SCANCODE_SPACE]) {
        toucheSpace_Relache = 1;
    }

    SDL_Rect chairRectPx = { CHAIR_TILE_X * TILE_SIZE, CHAIR_TILE_Y * TILE_SIZE, CHAIR_TILE_W * TILE_SIZE, CHAIR_TILE_H * TILE_SIZE };
    int nearChair = 0;
    if (currentLevel == 0) {
        SDL_Rect playerRect = { (int)player.x, (int)player.y, player.w, player.h };
        SDL_Rect chairReachRect = { chairRectPx.x - 6, chairRectPx.y - 6, chairRectPx.w + 12, chairRectPx.h + 12 };
        nearChair = SDL_HasIntersection(&playerRect, &chairReachRect);
    }
    showSitPrompt = (nearChair && !isSitting && !isSleeping);

    if (isSitting) {
        if (state[SDL_SCANCODE_X]) {
            if (toucheX_Relache) {
                isSitting = 0;
                toucheX_Relache = 0;
                player.x = chairRectPx.x + (chairRectPx.w / 2) - (player.w / 2);
                player.y = chairRectPx.y - player.h - 2; // Se relève au-dessus pour éviter le bureau
            }
        } else {
            toucheX_Relache = 1;
        }
        return;
    }

    if (nearChair && state[SDL_SCANCODE_X]) {
        if (toucheX_Relache) {
            isSitting = 1;
            toucheX_Relache = 0;
            player.x = chairRectPx.x + (chairRectPx.w / 2) - (player.w / 2);
            player.y = chairRectPx.y + (chairRectPx.h / 2) - (player.h / 2) + 4; // Position assise
        }
        return;
    } else if (!state[SDL_SCANCODE_X]) {
        toucheX_Relache = 1;
    }

    if (state[SDL_SCANCODE_I]) {
        if (toucheI_Relache) {
            showPhone = !showPhone;
            toucheI_Relache = 0;
        }
    } else {
        toucheI_Relache = 1;
    }

    if (showPhone) {
        return;
    }
    
    // --- BLOQUER LE MOUVEMENT SI CACHÉ DANS LA TENTE ---
    if (isHidingInTent) {
        // Le joueur est immobile quand il est caché, on skip le mouvement
        // Seule la touche T peut le faire sortir (gérée plus bas)
    } else {
        // Mouvement normal du joueur
        float nextX = player.x;
        float nextY = player.y;

        if (state[SDL_SCANCODE_UP])    nextY -= PLAYER_SPEED;
        if (state[SDL_SCANCODE_DOWN])  nextY += PLAYER_SPEED;
        if (state[SDL_SCANCODE_LEFT])  nextX -= PLAYER_SPEED;
        if (state[SDL_SCANCODE_RIGHT]) nextX += PLAYER_SPEED;

        // Collision X
        int touchWallX = touchesWall(nextX, player.y, player.w, player.h);
        if (!touchWallX) player.x = nextX;

        // Collision Y
        int touchWallY = touchesWall(player.x, nextY, player.w, player.h);
        if (!touchWallY) player.y = nextY;
    }

    float armoireX = 256 + 16; 
    float armoireY = 16 + 16; 

    // Calcul de la distance entre le joueur et l'armoire
    float dx = (player.x + player.w / 2) - armoireX;
    float dy = (player.y + player.h / 2) - armoireY;
    float distance = sqrt(dx*dx + dy*dy);

    if (distance < 16 && currentLevel == 0 && maps[0][0][16] == 8) {
        showInteractPrompt = 1;
    }
    else{
        showInteractPrompt = 0;
    }

    if (distance < 16 && currentLevel == 0 && maps[0][0][16] == 16) {
        showInteractPrompt2 = 1;
    }
    else{
        showInteractPrompt2 = 0;
    }
    if (distance < 16 && currentLevel == 0 && maps[0][0][16] == 12) {
        showInteractPrompt3 = 1;
    }
    else{
        showInteractPrompt3 = 0;
    }


    if (state[SDL_SCANCODE_E]) {
        if (toucheE_Relache) {
            // Si le joueur est à moins de 16 pixel (une tuile)
            if (distance < 16 && currentLevel == 0) {
                // On vérifie si l'armoire est FERMÉE (8) (tuile en haut à gauche de l'armoire)
                // On l'ouvre avec le doudou dedans
                if (maps[0][0][16] == 8 && hasDoudou == 0) {
                    if (sonOpenDoor) Mix_PlayChannel(-1, sonOpenDoor, 0);
                    SDL_Delay(250);
                    maps[0][0][16] = 16; 
                    maps[0][0][17] = 17; 
                    maps[0][1][16] = 18; 
                    maps[0][1][17] = 19; 
                }
                // Si elle est OUVERTE (16 = avec le doudou dedans), on peut la refermer
                else if (maps[0][0][16] == 16 || maps[0][0][16] == 12) {
                    if (sonCloseDoor) Mix_PlayChannel(-1, sonCloseDoor, 0);
                    SDL_Delay(250);
                    maps[0][0][16] = 8;
                    maps[0][0][17] = 9;
                    maps[0][1][16] = 10;
                    maps[0][1][17] = 11;
                }
                else{
                    if (sonOpenDoor) Mix_PlayChannel(-1, sonOpenDoor, 0);
                    SDL_Delay(250);
                    maps[0][0][16] = 12; 
                    maps[0][0][17] = 13; 
                    maps[0][1][16] = 14; 
                    maps[0][1][17] = 15; 
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
                if (maps[0][0][16] == 16)
                {
                    if (sonPickUp) Mix_PlayChannel(-1, sonPickUp, 0);
                    SDL_Delay(250);
                    maps[0][0][16] = 12; 
                    maps[0][0][17] = 13; 
                    maps[0][1][16] = 14; 
                    maps[0][1][17] = 15; 
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
    
    // --- GESTION DE LA TENTE (touche T pour se cacher) ---
    // Position de la tente : colonnes 16,17,18 / lignes 6,7 (selon la map)
    float tentX = 17 * TILE_SIZE;  // Centre de la tente
    float tentY = 7 * TILE_SIZE;   // Bas de la tente
    
    float dxTent = (player.x + player.w / 2) - tentX;
    float dyTent = (player.y + player.h / 2) - tentY;
    float distanceTent = sqrt(dxTent*dxTent + dyTent*dyTent);
    
    // Afficher le prompt si proche de la tente (ou si déjà caché)
    if ((distanceTent < 24 && currentLevel == 0) || isHidingInTent) {
        showTentPrompt = 1;
    } else {
        showTentPrompt = 0;
    }
    
    // Gestion de la touche T pour se cacher/sortir
    if (state[SDL_SCANCODE_T]) {
        if (toucheT_Relache && currentLevel == 0) {
            if (!isHidingInTent && distanceTent < 24) {
                // Se cacher : positionner le joueur À L'INTÉRIEUR de la tente
                isHidingInTent = 1;
                player.x = tentX - (player.w / 2);
                player.y = tentY - 4;  // Position à l'intérieur, pieds visibles
                toucheT_Relache = 0;
            } else if (isHidingInTent) {
                // Sortir : repositionner DEVANT la tente, en dehors de la zone des tuiles
                isHidingInTent = 0;
                player.x = tentX - (player.w / 2);
                player.y = tentY + 20;  // Plus loin devant pour éviter les collisions
                toucheT_Relache = 0;
            }
        }
    } else {
        toucheT_Relache = 1;
    }

    // 1. Quitter la CHAMBRE (Niveau 0) par le HAUT
    // On vérifie si on est au niveau 0 ET si on dépasse le haut de l'écran (y < 5)
    if (IsLocationUp(8, 13, 0, 5)) {
        if (hasDoudou == 1) {
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
    else if (IsLocationDown(8, 13, 1, 20)) {
        currentLevel = 0;  // On retourne à la CHAMBRE
        player.y = 10;     // On apparaît tout en HAUT de la chambre
        
    }
    
    if (IsLocationUp(8, 13, 1, 5)) {
            currentLevel = 2; 
            player.y = (MAP_HEIGHT * TILE_SIZE) - 20;
    }

    else if (IsLocationDown(8, 13, 2, 20)) {
        currentLevel = 1;  
        player.y = 10;     
        
    }
    
    if (IsLocationLeft(5, 10, 2, 5)) {
            currentLevel = 3; 
            player.x = (MAP_WIDTH * TILE_SIZE) - 20;
    }

    else if (IsLocationRight(5, 10, 3, 20)) {
        currentLevel = 2;  // On retourne à la CHAMBRE
        player.x = 3;     // On apparaît tout en HAUT de la chambre
        
    }
        
    if (IsLocationUp(8, 13, 3, 5)) {
            currentLevel = 4; 
            player.y = (MAP_HEIGHT * TILE_SIZE) - 20;
    }

    else if (IsLocationDown(8, 13, 4, 20)) {
        currentLevel = 3;  // On retourne à la CHAMBRE
        player.y = 10;     // On apparaît tout en HAUT de la chambre
        
    }

    // --- TRANSITIONS DU LABYRINTHE ---

    // Transition du niveau 2 au premier niveau du labyrinthe (niveau 5)
    if(IsLocationRight(11, 14, 2, 20)){
        currentLevel = 5;
        player.x = 3;
        // Mix_FreeMusic(bgm);
    }

    // Transition du premier niveau du labyrinthe (niveau 5) au niveau 2
    else if (IsLocationLeft(11, 14, 5, 5))
    {
        currentLevel = 2;
        player.x = (MAP_WIDTH * TILE_SIZE) - 20;
    }

    if(IsLocationRight(12, 14, 5, 20)){
        currentLevel = 6;
        player.x = 3;
    }

    else if (IsLocationLeft(12, 14, 6, 5))
    {
        currentLevel = 5;
        player.x = (MAP_WIDTH * TILE_SIZE) - 20;
    }

    if(IsLocationDown(10, 13, 6, 20)){
        currentLevel = 7;
        player.y = 10;
    }

    else if(IsLocationUp(10, 13, 7, 5)){
        currentLevel = 6;
        player.y = (MAP_HEIGHT * TILE_SIZE) - 20;
    }

    if(IsLocationLeft(12, 14, 7, 5)){
        currentLevel = 8;
        player.x = (MAP_WIDTH * TILE_SIZE) - 20;
    }

    else if(IsLocationRight(12, 14, 8, 20)){
        currentLevel = 7;
        player.x = 3;
    }


    // Changement de son d'ambiance
    ManageMusic();


    // printf("lvl: %d \n", currentLevel);
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

void DrawTuiles(float x, float y, int indexTuile, SDL_Renderer *renderer, int rayon){
    if(estEclaire(x, y, rayon))
    {   
        SDL_Rect srcTuile = { TILE_SIZE * indexTuile, 0, TILE_SIZE, TILE_SIZE };
        SDL_Rect destTuile = { x * TILE_SIZE, y * TILE_SIZE , TILE_SIZE, TILE_SIZE };
        SDL_RenderCopy(renderer, tilesetTexture, &srcTuile, &destTuile);
    }
}

int IsTuileSpecial(int index){
    for (int i = 0; i < tailleTuilesNotSpecial; ++i)
    {
        if(TuilesNotSpecial[i] == index){
            return 0;
        }
    }
    return 1;
}

// --- DESSIN ---
void DrawGame(SDL_Renderer *renderer,TTF_Font *font, TTF_Font *fontMini) {
    // 1. Fond Noir
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // 2. Dessiner la CARTE avec le tileset
    if (tilesetTexture) {
        // int rayonLumiere = 0;
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

                // --- A. DESSINER LA TUILE --- (avec le tableau)
                int type = maps_patern[currentLevel][y][x]; 
                DrawTuiles(x, y, type, renderer, rayon);

                int type_maps = maps[currentLevel][y][x];
                if(IsTuileSpecial(type_maps)){
                    DrawTuiles(x, y, type_maps, renderer, rayon);
                }

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

    

    // Affichage du joueur (avec gestion de l'état caché dans la tente)
    if (isHidingInTent) {
        // Joueur caché : afficher seulement les pieds (partie basse 6 pixels)
        SDL_Rect srcPlayerFeet = { 112, 10, 16, 6 };  // 6 pixels du bas du sprite
        SDL_Rect destPlayerFeet = { (int)player.x - 2, (int)player.y + 8, 16, 6 };
        SDL_RenderCopy(renderer, tilesetTexture, &srcPlayerFeet, &destPlayerFeet);
    } else {
        // Joueur normal : afficher en entier
        SDL_Rect srcPlayer = { 112, 0, 16, 16 };
        SDL_Rect destPlayer = { (int)player.x - 2, (int)player.y - 2, 16, 16 };
        SDL_RenderCopy(renderer, tilesetTexture, &srcPlayer, &destPlayer);
    }

   if (showInteractPrompt == 1) {
        SDL_Color cBlanc = {255, 255, 255, 255};
        SDL_Surface *sText = TTF_RenderText_Solid(fontMini, "[E] Ouvrir", cBlanc);
        
        if (sText) {
            SDL_Texture *tText = SDL_CreateTextureFromSurface(renderer, sText);
            
            // 1. Calcul de la position théorique
            int posX = (int)player.x - (sText->w / 2) + 8;
            int posY = (int)player.y + 20;

            // 2. CORRECTION (Clamping) pour ne pas sortir de l'écran
            if (posX < 2) { 
                posX = 2; // Bloque à gauche
            } 
            else if (posX + sText->w > LOGICAL_WIDTH - 2) {
                posX = LOGICAL_WIDTH - sText->w - 2; // Bloque à droite
            }

            // 3. On applique la position corrigée
            SDL_Rect rText = { posX, posY, sText->w, sText->h };

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150); 
            SDL_Rect rFond = { rText.x - 2, rText.y - 1, rText.w + 4, rText.h + 2 };
            SDL_RenderFillRect(renderer, &rFond);

            SDL_RenderCopy(renderer, tText, NULL, &rText);
            
            SDL_FreeSurface(sText);
            SDL_DestroyTexture(tText);
        }
    }

     if (showInteractPrompt2 == 1) {
        SDL_Color cBlanc = {255, 255, 255, 255};
        SDL_Surface *sText = TTF_RenderText_Solid(fontMini, "[ENTER] Interagir", cBlanc);
        
        if (sText) {
            SDL_Texture *tText = SDL_CreateTextureFromSurface(renderer, sText);
            
            // 1. Calcul position
            int posX = (int)player.x - (sText->w / 2) + 8;
            int posY = (int)player.y + 20;

            // 2. Correction (Clamping)
            if (posX < 2) posX = 2;
            else if (posX + sText->w > LOGICAL_WIDTH - 2) posX = LOGICAL_WIDTH - sText->w - 2;
            
            // 3. Application
            SDL_Rect rText = { posX, posY, sText->w, sText->h };
            

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150); 
            SDL_Rect rFond = { rText.x - 2, rText.y - 1, rText.w + 4, rText.h + 2 };
            SDL_RenderFillRect(renderer, &rFond);

            SDL_RenderCopy(renderer, tText, NULL, &rText);
            
            SDL_FreeSurface(sText);
            SDL_DestroyTexture(tText);
        }
    }
    
    // --- PROMPT POUR LA TENTE ---
    if (showTentPrompt == 1) {
        SDL_Color cBlanc = {255, 255, 255, 255};
        const char* textePrompt = isHidingInTent ? "[T] Sortir" : "[T] Se cacher";
        SDL_Surface *sText = TTF_RenderText_Solid(fontMini, textePrompt, cBlanc);
        
        if (sText) {
            SDL_Texture *tText = SDL_CreateTextureFromSurface(renderer, sText);
            
            int posX = (int)player.x - (sText->w / 2) + 8;
            int posY = (int)player.y + 20;

            if (posX < 2) posX = 2;
            else if (posX + sText->w > LOGICAL_WIDTH - 2) posX = LOGICAL_WIDTH - sText->w - 2;
            
            SDL_Rect rText = { posX, posY, sText->w, sText->h };

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150); 
            SDL_Rect rFond = { rText.x - 2, rText.y - 1, rText.w + 4, rText.h + 2 };
            SDL_RenderFillRect(renderer, &rFond);

            SDL_RenderCopy(renderer, tText, NULL, &rText);
            
            SDL_FreeSurface(sText);
            SDL_DestroyTexture(tText);
        }
    }
    
    if (showInteractPrompt3 == 1) {
        SDL_Color cBlanc = {255, 255, 255, 255};
        SDL_Surface *sText = TTF_RenderText_Solid(fontMini, "[E] Fermer", cBlanc);
        
        if (sText) {
            SDL_Texture *tText = SDL_CreateTextureFromSurface(renderer, sText);
            
            // 1. Calcul position
            int posX = (int)player.x - (sText->w / 2) + 8;
            int posY = (int)player.y + 20;

            // 2. Correction (Clamping)
            if (posX < 2) posX = 2;
            else if (posX + sText->w > LOGICAL_WIDTH - 2) posX = LOGICAL_WIDTH - sText->w - 2;
            
            // 3. Application
            SDL_Rect rText = { posX, posY, sText->w, sText->h };
            

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150); 
            SDL_Rect rFond = { rText.x - 2, rText.y - 1, rText.w + 4, rText.h + 2 };
            SDL_RenderFillRect(renderer, &rFond);

            SDL_RenderCopy(renderer, tText, NULL, &rText);
            
            SDL_FreeSurface(sText);
            SDL_DestroyTexture(tText);
        }
    }

    if (showSleepPrompt && !isSleeping) {
        SDL_Color cBlanc = {255, 255, 255, 255};
        SDL_Surface *sText = TTF_RenderText_Solid(fontMini, "[ESPACE] Dormir", cBlanc);
        if (sText) {
            SDL_Texture *tText = SDL_CreateTextureFromSurface(renderer, sText);
            int posX = (int)player.x - (sText->w / 2) + 8;
            int posY = (int)player.y + 34;
            if (posX < 2) posX = 2;
            else if (posX + sText->w > LOGICAL_WIDTH - 2) posX = LOGICAL_WIDTH - sText->w - 2;
            SDL_Rect rText = { posX, posY, sText->w, sText->h };

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
            SDL_Rect rFond = { rText.x - 2, rText.y - 1, rText.w + 4, rText.h + 2 };
            SDL_RenderFillRect(renderer, &rFond);
            SDL_RenderCopy(renderer, tText, NULL, &rText);
            SDL_FreeSurface(sText);
            SDL_DestroyTexture(tText);
        }
    }

    if (showSitPrompt && !isSitting) {
        SDL_Color cBlanc = {255, 255, 255, 255};
        SDL_Surface *sText = TTF_RenderText_Solid(fontMini, "[X] S'asseoir", cBlanc);
        if (sText) {
            SDL_Texture *tText = SDL_CreateTextureFromSurface(renderer, sText);
            int posX = (int)player.x - (sText->w / 2) + 8;
            int posY = (int)player.y + 34;
            if (posX < 2) posX = 2;
            else if (posX + sText->w > LOGICAL_WIDTH - 2) posX = LOGICAL_WIDTH - sText->w - 2;
            SDL_Rect rText = { posX, posY, sText->w, sText->h };

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
            SDL_Rect rFond = { rText.x - 2, rText.y - 1, rText.w + 4, rText.h + 2 };
            SDL_RenderFillRect(renderer, &rFond);
            SDL_RenderCopy(renderer, tText, NULL, &rText);
            SDL_FreeSurface(sText);
            SDL_DestroyTexture(tText);
        }
    }

    if (isSleeping) {
        SDL_Color cBlanc = {255, 255, 255, 255};
        SDL_Surface *sText = TTF_RenderText_Solid(fontMini, "Zzz...", cBlanc);
        if (sText) {
            SDL_Texture *tText = SDL_CreateTextureFromSurface(renderer, sText);
            SDL_Rect rText = { (int)player.x - 4, (int)player.y - 12, sText->w, sText->h };
            SDL_RenderCopy(renderer, tText, NULL, &rText);
            SDL_FreeSurface(sText);
            SDL_DestroyTexture(tText);
        }
    }

    if (isSitting) {
        SDL_Color cBlanc = {255, 255, 255, 255};
        SDL_Surface *sText = TTF_RenderText_Solid(fontMini, "Assis", cBlanc);
        if (sText) {
            SDL_Texture *tText = SDL_CreateTextureFromSurface(renderer, sText);
            SDL_Rect rText = { (int)player.x - 4, (int)player.y - 12, sText->w, sText->h };
            SDL_RenderCopy(renderer, tText, NULL, &rText);
            SDL_FreeSurface(sText);
            SDL_DestroyTexture(tText);
        }
    }

    if (showPhone) {
        SDL_Rect phoneRect = { 50, 30, 220, 180 };
        
        // Fond du téléphone (noir)
        SDL_SetRenderDrawColor(renderer, 15, 15, 20, 255);
        SDL_RenderFillRect(renderer, &phoneRect);
        
        // Barre de titre (header coloré)
        SDL_Rect headerRect = { phoneRect.x, phoneRect.y, phoneRect.w, 20 };
        SDL_SetRenderDrawColor(renderer, 50, 100, 150, 255);
        SDL_RenderFillRect(renderer, &headerRect);
        
        // Bordure du téléphone
        SDL_SetRenderDrawColor(renderer, 80, 120, 160, 255);
        SDL_RenderDrawRect(renderer, &phoneRect);
        
        // Écran principal
        SDL_Rect screenRect = { phoneRect.x + 5, phoneRect.y + 25, phoneRect.w - 10, phoneRect.h - 30 };
        SDL_SetRenderDrawColor(renderer, 25, 30, 40, 255);
        SDL_RenderFillRect(renderer, &screenRect);
        
        SDL_Color cBlanc = {255, 255, 255, 255};
        SDL_Color cGris = {180, 180, 180, 255};
        
        // Titre dans le header
        SDL_Surface *sTitle = TTF_RenderText_Solid(fontMini, "TELEPHONE", cBlanc);
        if (sTitle) {
            SDL_Texture *tTitle = SDL_CreateTextureFromSurface(renderer, sTitle);
            SDL_Rect rTitle = { phoneRect.x + 8, phoneRect.y + 5, sTitle->w, sTitle->h };
            SDL_RenderCopy(renderer, tTitle, NULL, &rTitle);
            SDL_FreeSurface(sTitle);
            SDL_DestroyTexture(tTitle);
        }
        
        // Section Contacts
        SDL_Surface *sContacts = TTF_RenderText_Solid(fontMini, "CONTACTS:", cBlanc);
        if (sContacts) {
            SDL_Texture *tContacts = SDL_CreateTextureFromSurface(renderer, sContacts);
            SDL_Rect rContacts = { screenRect.x + 5, screenRect.y + 5, sContacts->w, sContacts->h };
            SDL_RenderCopy(renderer, tContacts, NULL, &rContacts);
            SDL_FreeSurface(sContacts);
            SDL_DestroyTexture(tContacts);
        }
        
        // Contact Père
        SDL_Surface *sPere = TTF_RenderText_Solid(fontMini, "- Pere", cGris);
        if (sPere) {
            SDL_Texture *tPere = SDL_CreateTextureFromSurface(renderer, sPere);
            SDL_Rect rPere = { screenRect.x + 10, screenRect.y + 22, sPere->w, sPere->h };
            SDL_RenderCopy(renderer, tPere, NULL, &rPere);
            SDL_FreeSurface(sPere);
            SDL_DestroyTexture(tPere);
        }
        
        // Contact Mère
        SDL_Surface *sMere = TTF_RenderText_Solid(fontMini, "- Mere", cGris);
        if (sMere) {
            SDL_Texture *tMere = SDL_CreateTextureFromSurface(renderer, sMere);
            SDL_Rect rMere = { screenRect.x + 10, screenRect.y + 37, sMere->w, sMere->h };
            SDL_RenderCopy(renderer, tMere, NULL, &rMere);
            SDL_FreeSurface(sMere);
            SDL_DestroyTexture(tMere);
        }
        
        // Ligne de séparation
        SDL_SetRenderDrawColor(renderer, 60, 70, 80, 255);
        SDL_RenderDrawLine(renderer, screenRect.x + 5, screenRect.y + 55, screenRect.x + screenRect.w - 5, screenRect.y + 55);
        
        // Section Inventaire
        SDL_Surface *sInv = TTF_RenderText_Solid(fontMini, "INVENTAIRE:", cBlanc);
        if (sInv) {
            SDL_Texture *tInv = SDL_CreateTextureFromSurface(renderer, sInv);
            SDL_Rect rInv = { screenRect.x + 5, screenRect.y + 62, sInv->w, sInv->h };
            SDL_RenderCopy(renderer, tInv, NULL, &rInv);
            SDL_FreeSurface(sInv);
            SDL_DestroyTexture(tInv);
        }
        
        if (hasDoudou) {
            SDL_Surface *sDoudou = TTF_RenderText_Solid(fontMini, "- Doudou", cGris);
            if (sDoudou) {
                SDL_Texture *tDoudou = SDL_CreateTextureFromSurface(renderer, sDoudou);
                SDL_Rect rDoudou = { screenRect.x + 10, screenRect.y + 79, sDoudou->w, sDoudou->h };
                SDL_RenderCopy(renderer, tDoudou, NULL, &rDoudou);
                SDL_FreeSurface(sDoudou);
                SDL_DestroyTexture(tDoudou);
            }
        } else {
            SDL_Surface *sEmpty = TTF_RenderText_Solid(fontMini, "(vide)", cGris);
            if (sEmpty) {
                SDL_Texture *tEmpty = SDL_CreateTextureFromSurface(renderer, sEmpty);
                SDL_Rect rEmpty = { screenRect.x + 10, screenRect.y + 79, sEmpty->w, sEmpty->h };
                SDL_RenderCopy(renderer, tEmpty, NULL, &rEmpty);
                SDL_FreeSurface(sEmpty);
                SDL_DestroyTexture(tEmpty);
            }
        }
        
        // Bouton de fermeture en bas
        SDL_Rect btnRect = { screenRect.x + 5, screenRect.y + screenRect.h - 18, screenRect.w - 10, 15 };
        SDL_SetRenderDrawColor(renderer, 50, 100, 150, 255);
        SDL_RenderFillRect(renderer, &btnRect);
        SDL_SetRenderDrawColor(renderer, 80, 120, 160, 255);
        SDL_RenderDrawRect(renderer, &btnRect);
        
        SDL_Surface *sClose = TTF_RenderText_Solid(fontMini, "[I] FERMER", cBlanc);
        if (sClose) {
            SDL_Texture *tClose = SDL_CreateTextureFromSurface(renderer, sClose);
            SDL_Rect rClose = { btnRect.x + (btnRect.w - sClose->w) / 2, btnRect.y + 2, sClose->w, sClose->h };
            SDL_RenderCopy(renderer, tClose, NULL, &rClose);
            SDL_FreeSurface(sClose);
            SDL_DestroyTexture(tClose);
        }
    }


}

