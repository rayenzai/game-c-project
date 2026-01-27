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
58 = coussin bas 
59,60 = coussins 
61 = digicode 0
78 = hanse basket
79 = ballon de basket
80,81 = commode mur droit
........................
62 = monstre vu de gauche
63 = monstre de face
64 = monstre vu de droite
.........................
65, 66 = haut tapis 
67, 68 = tapis 
69, 70 = bas mur 
71 72 = table
73 = vase
74, 75 = lampe murale gauche
76, 77 = lampe murale droite


*/

// --- LA CARTE DU NIVEAU ---
static int maps[NB_LEVELS][MAP_HEIGHT][MAP_WIDTH] = {
 {      //carte 1 (chambre)
        {2,  2,  2,  2,  2,  2,  2,  2,  0,  0, 0, 0, 2,  2,  5,  2,  8,  9,  2, 2}, // Trou en haut   
        {2,  2,  2, 36, 37,  2,  2,  2,  0,  1, 0, 0, 2,  2, 41,  2, 10, 11,  2, 2}, 
        {2,  1,  0, 32, 33, 21,  0,  1,  0,  1, 0, 1, 0,  1,  0,  1,  0,  1,  0, 2},
        {2,  1,  0, 34, 35,  1,  0,  1,  0,  1, 0, 1, 0,  1,  0,  1,  0,  1,  0, 2},
        {2,  1, 30, 31,  0,  1,  0,  1,  0,  1, 0, 1, 0,  1,  0,  1,  0,  1,  0, 2},
        {2,  1,  0, 20,  0,  1,  0,  1,  0,  1, 0, 1, 0,  1,  0,  1,  0,  1,  0, 2},
        {2, 42, 0,  1,  0,  1,  0,  1,  0,  1, 0, 1, 0,  1,  0,  1, 55, 56, 57, 2},
        {2, 43, 0,  1,  0,  1,  0,  1,  0,  1, 0, 1, 0,  1,  0,  1, 52, 53, 54, 2},
        {2,  1,  0,  1,  0,  3, 44,  1,  0,  1, 0, 1, 0,  1,  0, 58,  0, 59, 60, 2},
        {2,  1,  0, 40,  0,  1,  4,  1,  0,  1, 0, 1, 0,  1,  0,  1,  0,  1,  0, 2},
        {2,  1,  0,  1, 38, 39,  0,  1,  0,  1, 0, 1, 0,  1,  0,  1,  0,  1,  0, 2},
        {2, 78,  0,  1,  0,  1,  0,  1,  0,  1, 0, 1, 0, 27, 28, 29,  0,  1, 81, 2},
        {2,  1, 79,  1,  0,  1, 51,  1,  0,  1, 0, 1, 0, 24, 25, 26,  0,  1, 80, 2}, // Bas fermé
        {2,  1,  0,  1,  0,  1, 46, 47, 48, 49, 0, 1, 0, 22, 23,  0,  0,  1,  0, 2},
        {2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 2, 2, 2,  2,  2,  2,  2,  2,  2, 2}  // Bas fermé
    },
    // CARTE 2 : COULOIR (Niveau 1)
    {
        {2, 2, 2, 2, 2, 2, 2,  2,  0, 69, 70,  0,  2,  2, 2, 2, 2, 2, 2, 2}, // Haut (Suite)
        {2, 2, 2, 2, 2, 2, 2,  2,  0, 67, 68,  0,  2,  2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2,  2,  0, 67, 68,  0,  2,  2, 2, 2, 2, 2, 2, 2}, // Toits cotés
        {2, 2, 2, 2, 2, 2, 2,  2,  0, 67, 68,  0,  2,  2, 2, 2, 2, 2, 2, 2}, // Murs cotés
        {2, 2, 2, 2, 2, 2, 2,  2,  0, 67, 68, 76, 77,  2, 2, 2, 2, 2, 2, 2}, // Vide (Sol)
        {2, 2, 2, 2, 2, 2, 2, 74, 75, 67, 68,  0,  2,  2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2,  2,  0, 67, 68,  0,  2,  2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2,  2,  0, 67, 68,  0,  2,  2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2,  2, 72, 67, 68,  0,  2,  2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2,  2, 71, 67, 68,  0,  2,  2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2,  2,  0, 67, 68, 76, 77,  2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 74, 75, 67, 68,  0,  2,  2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2,  2,  0, 67, 68, 73,  2,  2, 2, 2, 2, 2, 2, 2}, // Bas (Entrée)
        {2, 2, 2, 2, 2, 2, 2,  2,  0, 67, 68,  0,  2,  2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2,  2,  0, 65, 66,  0,  2,  2, 2, 2, 2, 2, 2, 2}
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

int TuilesNotSpecial[] = {0, 1, 2};
int tailleTuilesNotSpecial = (int)sizeof(TuilesNotSpecial) / (int)sizeof(TuilesNotSpecial[0]);

// --- INITIALISATION ---
void InitGame(SDL_Renderer *renderer) {
    player.x = 80; 
    player.y = 50; 
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

    // --- TYPE 1 : MURS CLASSIQUES (Tout le bloc est solide) ---
    // Les murs, les bords, le vide...

    if (type == 2 || type == 5){
        // VERIFICATION DU MUR "DU DESSOUS"
        // Si la case en dessous est un autre mur (type 2), alors c'est un "mur de coté" ou un "mur plein".
        
        int caseY_Below = caseY + 1;
        if (caseY_Below < MAP_HEIGHT) {
            int typeBelow = maps[currentLevel][caseY_Below][caseX];
            if (typeBelow == 2 || typeBelow == 5) {
                return 1;
            }
        } else {
             return 1;
        }

        int localY = (int)y % TILE_SIZE; 

        if (localY < 4) {
            return 1;
        } else {
            return 0;
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

    float dirX = 0;
    float dirY = 0;

    if (state[SDL_SCANCODE_UP])    dirY -= 1;
    if (state[SDL_SCANCODE_DOWN])  dirY += 1;
    if (state[SDL_SCANCODE_LEFT])  dirX -= 1;
    if (state[SDL_SCANCODE_RIGHT]) dirX += 1;

    // Si on bouge sur les deux axes en même temps (Diagonale)
    if (dirX != 0 && dirY != 0) {
        // On multiplie par 0.707 (environ 1/racine(2)) pour ralentir
        dirX *= 0.7071f;
        dirY *= 0.7071f;
    }

    // 3. On applique la VITESSE
    float nextX = player.x + (dirX * PLAYER_SPEED);
    float nextY = player.y + (dirY * PLAYER_SPEED);
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
    if(IsLocationRight(10, 14, 2, 20)){
        currentLevel = 5;
        player.x = 5;
        // Mix_FreeMusic(bgm);
    }

    // Transition du premier niveau du labyrinthe (niveau 5) au niveau 2
    else if (IsLocationLeft(10, 14, 5, 5))
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


float getLuminosite(int gridX, int gridY, int rayonPx) {
    float maxIntensite = 0.0f;

    // --- 1. Lumière du JOUEUR (Calcul en pixels) ---
    int tileCenterX = (gridX * TILE_SIZE) + (TILE_SIZE / 2);
    int tileCenterY = (gridY * TILE_SIZE) + (TILE_SIZE / 2);
    int playerCenterX = (int)player.x + (player.w / 2);
    int playerCenterY = (int)player.y + (player.h / 2);

    float dx = (float)(tileCenterX - playerCenterX);
    float dy = (float)(tileCenterY - playerCenterY);
    float distPx = sqrtf(dx*dx + dy*dy);

    // Formule : Plus on est proche, plus c'est fort (1.0). Plus on s'éloigne, plus ça baisse.
    
    if (distPx < rayonPx) {
        float i = 1.0f - (distPx / (float)rayonPx);
        if (i > maxIntensite) maxIntensite = i;
    }
    /*
    // formule expo:
    if (distPx < rayonPx) {
        float ratio = distPx / (float)rayonPx; // 0.0 (proche) à 1.0 (loin)
        
        // Formule "Carrée" : La lumière reste forte près du joueur et tombe vite à la fin
        float i = 1.0f - (ratio * ratio); 
        
        if (i > maxIntensite) maxIntensite = i;
    }
    */
    // --- 2. Lumière des LAMPES (Calcul en cases) ---
    for (int ly = 0; ly < MAP_HEIGHT; ly++) {
        for (int lx = 0; lx < MAP_WIDTH; lx++) {
             if (maps[currentLevel][ly][lx] == 21 || (maps[currentLevel][ly][lx] >= 74 && maps[currentLevel][ly][lx] <= 77)) { // Si c'est une lampe
                 float distGrid = sqrtf(powf(gridX - lx, 2) + powf(gridY - ly, 2));
                 float rayonLampe = 2.5f; // Rayon d'une lampe (2.5 cases)
                 
                 if (distGrid < rayonLampe) {
                     float i = 1.0f - (distGrid / rayonLampe);
                     if (i > maxIntensite) maxIntensite = i;
                 }
             }
        }
    }
    
    return maxIntensite; // Retourne la lumière la plus forte trouvée
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


int estVisible(int x, int y, int rayonJoueur) {
    if (getLuminosite(x, y, rayonJoueur) > 0.0f) {
        return 1;
    }
    return 0;
}

void DrawTuiles(float x, float y, int indexTuile, SDL_Renderer *renderer, int luminosite){
    SDL_SetTextureColorMod(tilesetTexture, luminosite, luminosite, luminosite);

    SDL_Rect srcTuile = { TILE_SIZE * indexTuile, 0, TILE_SIZE, TILE_SIZE };
    SDL_Rect destTuile = { (int)x * TILE_SIZE, (int)y * TILE_SIZE , TILE_SIZE, TILE_SIZE };
    SDL_RenderCopy(renderer, tilesetTexture, &srcTuile, &destTuile);
    
    SDL_SetTextureColorMod(tilesetTexture, 255, 255, 255);
    
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
                float intensite = getLuminosite(x, y, rayon);

                // Si c'est totalement noir, on ne dessine rien (opti)
                if (intensite <= 0.0f) {
                    continue; 
                }

                // 2. On convertit en valeur 0-255
                int lum = (int)(intensite * 255);

                // --- A. DESSINER LA TUILE ---
                int type = maps_patern[currentLevel][y][x]; 
                DrawTuiles(x, y, type, renderer, lum); // On passe 'lum'

                int type_maps = maps[currentLevel][y][x];
                if(IsTuileSpecial(type_maps)){
                    DrawTuiles(x, y, type_maps, renderer, lum); // On passe 'lum'
                }

                // --- B. DESSINER LE CONTOUR (Lignes) ---
                // On utilise aussi 'lum' pour que les lignes blanches s'assombrissent 
                /*SDL_SetRenderDrawColor(renderer, lum, lum, lum, 255); 

                int px = x * TILE_SIZE;
                int py = y * TILE_SIZE;
                int ts = TILE_SIZE;
                
                // On garde estVisible juste pour savoir si le voisin est NOIR ou pas
                // (Si le voisin a une intensité <= 0, on dessine le trait)
                
                if (getLuminosite(x, y - 1, rayon) <= 0.0f) SDL_RenderDrawLine(renderer, px, py, px + ts, py);       // Haut
                if (getLuminosite(x, y + 1, rayon) <= 0.0f) SDL_RenderDrawLine(renderer, px, py + ts, px + ts, py + ts); // Bas
                if (getLuminosite(x - 1, y, rayon) <= 0.0f) SDL_RenderDrawLine(renderer, px, py, px, py + ts);       // Gauche
                if (getLuminosite(x + 1, y, rayon) <= 0.0f) SDL_RenderDrawLine(renderer, px + ts, py, px + ts, py + ts); // Droite
                */
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


}

