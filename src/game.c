#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "game.h"
#include "sons.h"
#include "map.h"
#include "ia.h"
#include <stdio.h>
#include <math.h>

#define VOLUME_MUSIQUE 32

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

static SDL_Texture *tilesetTexture = NULL; 
// static SDL_Texture *playerTexture = NULL; 

int rayon = 0;

 
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
82 = sol labyrinthe
83 = mur labyrinthe
.........................
65, 66 = haut tapis 
67, 68 = tapis 
69, 70 = bas mur 
71 72 = table
73 = vase
74, 75 = lampe murale gauche
76, 77 = lampe murale droite
...........................
84(bas), 85(tete rouge) = statue mur gauche 
86 = tete statue yeux verts 


*/



// --- LA CARTE DU NIVEAU ---
int maps[NB_LEVELS][MAP_HEIGHT][MAP_WIDTH] = {

 {      //carte 1 (chambre) index 0
        {2,  2,  2,  2,  2,  2,  2,  2,  0,  0,  0,  0, 2,  2,  5,  2,  8,  9,  2, 2}, // Trou en haut   
        {2,  2,  2, 36, 37,  2,  2,  2,  0,  1,  0,  0, 2,  2, 41,  2, 10, 11,  2, 2}, 
        {2,  1,  0, 32, 33, 21,  0,  1,  0,  1,  0,  1, 0,  1,  0,  1,  0,  1,  0, 2},
        {2,  1,  0, 34, 35,  1,  0,  1,  0,  1,  0,  1, 0,  1,  0,  1,  0,  1,  0, 2},
        {2,  1, 30, 31,  0,  1,  0,  1,  0,  1,  0,  1, 0,  1,  0,  1,  0,  1,  0, 2},
        {2,  1,  0, 20,  0,  1,  0,  1,  0,  1, 44,  1, 0,  1,  0,  1,  0,  1,  0, 2},
        {2, 42,  0,  1,  0,  1,  0,  1,  0,  1,  0, 45, 0,  1,  0,  1, 55, 56, 57, 2},
        {2, 43,  0,  1,  0,  1,  0,  1,  0,  1,  0,  1, 0,  1,  0,  1, 52, 53, 54, 2},
        {2,  1,  0,  1,  0,  3, 44,  1,  0,  1,  0,  1, 0,  1,  0, 58,  0, 59, 60, 2},
        {2,  1,  0, 40,  0,  1,  4,  1,  0,  1,  0,  1, 0,  1,  0,  1,  0,  1,  0, 2},
        {2,  1,  0,  1, 38, 39,  0,  1,  0,  1,  0,  1, 0,  1,  0,  1, 40,  1,  0, 2},
        {2, 78,  0,  1,  0,  1,  0,  1,  0,  1,  0,  1, 0, 27, 28, 29,  0,  1, 81, 2},
        {2,  1, 79,  1,  0,  1, 51,  1,  0,  1,  0,  1, 0, 24, 25, 26,  0,  1, 80, 2}, // Bas fermé
        {2,  1,  0,  1,  0,  1, 46, 47, 48, 49,  0,  1, 0, 22, 23,  0,  0,  1,  0, 2},
        {2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 2,  2,  2,  2,  2,  2,  2, 2}  // Bas fermé
    },
    // CARTE 2 : COULOIR (Niveau 1) index 1
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
    // CARTE 3 : HALL (Niveau 2) index 2
    {       
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,  2, 2, 2}, // Trou en haut (tout fermé en 2 ici ?)
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2,  8, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 2, 2},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  86, 2, 2},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  84, 2, 2},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 85, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 84, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 2, 2}, // Bas fermé
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2,  2, 2, 2}
    },
    // CARTE 4 (Niveau 3) index 3
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
    // CARTE 5 (Niveau 4 - dernière du bloc précédent) index 4
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
    // --- LABYRINTHE 1 (Index 5) --- 
    {
        { 2, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83}, 
        { 2, 82, 82, 82, 83, 82, 82, 82, 82, 82, 83, 82, 82, 82, 82, 82, 82, 82, 82, 83}, 
        { 2, 82, 83, 82, 83, 82, 83, 83, 83, 82, 83, 82, 83, 83, 83, 83, 83, 83, 82, 83},
        { 2, 82, 83, 82, 82, 82, 83, 82, 82, 82, 83, 82, 82, 82, 82, 82, 82, 83, 82, 83},
        { 2, 82, 83, 83, 83, 83, 83, 82, 83, 83, 83, 83, 83, 83, 83, 83, 82, 83, 82, 83}, 
        { 2, 82, 82, 82, 82, 82, 83, 82, 82, 82, 82, 82, 82, 82, 82, 83, 82, 83, 82, 83},
        { 2, 83, 83, 83, 83, 82, 83, 83, 83, 83, 83, 83, 83, 83, 82, 83, 83, 83, 82, 83}, 
        { 82, 82, 82, 82, 83, 82, 82, 82, 82, 82, 82, 82, 83, 83, 82, 82, 82, 82, 82, 83}, 
        { 82, 82, 82, 82, 83, 83, 83, 83, 83, 83, 83, 82, 83, 83, 83, 83, 83, 83, 83, 83}, 
        { 2, 82, 82, 82, 82, 82, 82, 82, 83, 82, 82, 82, 83, 82, 82, 82, 82, 82, 82, 83},
        { 2, 82, 83, 83, 83, 83, 83, 82, 83, 82, 83, 83, 83, 82, 83, 83, 82, 83, 82, 83}, 
        { 2, 82, 83, 82, 82, 82, 83, 82, 82, 82, 83, 82, 82, 82, 83, 82, 82, 83, 82, 83},
        { 2, 82, 83, 82, 83, 82, 83, 83, 83, 83, 83, 82, 83, 83, 83, 82, 82, 82, 82, 82}, // SORTIE DROITE
        { 2, 82, 82, 82, 83, 82, 82, 82, 82, 82, 82, 82, 83, 82, 82, 82, 83, 82, 82, 83}, 
        { 2, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83}

    },

    // --- LABYRINTHE 2 (Index 6) ---
    {
        {83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83}, // Mur Haut
        {83, 82, 82, 82, 82, 82, 83, 82, 82, 82, 83, 82, 82, 82, 82, 82, 82, 82, 82, 83},
        {83, 82, 83, 83, 83, 82, 83, 82, 83, 83, 83, 82, 83, 83, 83, 83, 83, 83, 82, 83},
        {83, 82, 83, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83, 82, 82, 82, 82, 83, 82, 83},
        {83, 82, 83, 82, 83, 83, 83, 83, 83, 83, 83, 82, 83, 82, 83, 83, 83, 83, 82, 83}, // Gros bloc central
        {83, 82, 82, 82, 83, 82, 82, 82, 82, 82, 83, 82, 82, 82, 82, 82, 82, 82, 82, 83},
        {83, 83, 83, 82, 83, 82, 83, 83, 83, 82, 83, 83, 83, 83, 83, 83, 83, 83, 82, 83},
        {83, 82, 82, 82, 82, 82, 83, 82, 82, 82, 82, 82, 82, 82, 82, 83, 82, 82, 82, 83},
        {83, 82, 83, 83, 83, 83, 83, 82, 83, 83, 83, 83, 83, 83, 82, 83, 82, 83, 83, 83},
        {83, 82, 83, 82, 82, 82, 82, 82, 82, 82, 83, 82, 82, 82, 82, 83, 82, 82, 82, 83},
        {83, 82, 83, 82, 83, 83, 83, 83, 83, 82, 83, 82, 83, 83, 83, 83, 82, 83, 82, 83},
        {83, 82, 82, 82, 83, 82, 82, 82, 82, 82, 83, 82, 82, 82, 82, 82, 82, 83, 82, 83},
        {82, 82, 83, 83, 83, 82, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 82, 83}, // ENTRÉE GAUCHE (Ligne 12, connectée à l'autre)
        {83, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83},
        {83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 82, 82, 83, 83, 83, 83, 83, 83, 83, 83}  // SORTIE BAS (Ligne 14, Colonnes 10-11)
    },
    // --- LABYRINTHE 3 (Index 7) ---
    {
        {83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 82, 82, 83, 83, 83, 83, 83, 83, 83, 83}, // ENTRÉE HAUT (Alignée avec sortie précédente)
        {83, 82, 82, 82, 83, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83}, // Zone ouverte pour tromper
        {83, 82, 83, 83, 83, 82, 83, 83, 83, 83, 83, 82, 83, 83, 83, 83, 83, 83, 82, 83},
        {83, 82, 82, 82, 82, 82, 83, 82, 82, 82, 82, 82, 83, 82, 82, 82, 82, 83, 82, 83}, // Début des fausses pistes
        {83, 83, 83, 83, 83, 83, 83, 82, 83, 83, 83, 83, 83, 82, 83, 83, 82, 83, 82, 83},
        {83, 82, 82, 82, 82, 82, 82, 82, 83, 82, 82, 82, 82, 82, 83, 82, 82, 83, 82, 83},
        {83, 82, 83, 83, 83, 83, 83, 83, 83, 82, 83, 83, 83, 83, 83, 82, 83, 83, 82, 83},
        {83, 82, 82, 82, 82, 82, 83, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83}, // Grand couloir transversal
        {83, 82, 83, 83, 83, 82, 83, 82, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83}, // Cul de sac massif à droite
        {83, 82, 82, 82, 83, 82, 83, 82, 82, 82, 82, 82, 82, 82, 83, 82, 82, 82, 82, 83},
        {83, 83, 83, 82, 83, 82, 83, 83, 83, 83, 83, 83, 83, 82, 83, 83, 83, 83, 82, 83},
        {83, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83, 82, 82, 82, 82, 82, 82, 83},
        {82, 82, 82, 82, 83, 83, 83, 83, 83, 83, 83, 82, 83, 83, 83, 83, 83, 83, 83, 83}, // SORTIE GAUCHE (Ligne 12)
        {83, 83, 83, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83},
        {83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83}  // Mur Bas fermé
    },

    // --- LABYRINTHE 4 (Index 8) ---
    {
        {83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83}, // Mur Haut
        {83, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83}, // Chemin du haut
        {83, 82, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 82, 83},
        {83, 82, 83, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83, 82, 83},
        {83, 82, 83, 82, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 82, 83, 82, 83},
        {83, 82, 83, 82, 83, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83, 82, 83, 82, 83},
        {83, 82, 83, 82, 83, 82, 83, 83, 83, 83, 83, 83, 83, 83, 82, 83, 82, 83, 82, 83},
        {83, 82, 83, 82, 83, 82, 83, 82, 82, 82, 82, 82, 82, 83, 82, 83, 82, 83, 82, 83}, // <--- LE DOUDOU (6) EST ICI
        {83, 82, 83, 82, 83, 82, 83, 83, 83, 82, 83, 83, 82, 83, 82, 83, 82, 83, 82, 83},
        {83, 82, 83, 82, 83, 82, 82, 82, 82, 82, 82, 83, 82, 83, 82, 83, 82, 82, 82, 83},
        {83, 82, 83, 82, 83, 83, 83, 83, 83, 83, 82, 83, 82, 83, 83, 83, 83, 83, 82, 83},
        {83, 82, 82, 82, 82, 82, 82, 82, 82, 83, 82, 82, 82, 82, 82, 82, 82, 83, 82, 83},
        {83, 83, 83, 83, 83, 83, 83, 83, 82, 83, 83, 83, 83, 83, 83, 83, 82, 82, 82, 82}, // ENTRÉE DROITE (Ligne 12)
        {83, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83}, // Fausse piste en bas
        {83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83}
    },

    // TENTE (index 9)
    {
        {50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50}, // Marge Haut
        {50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50}, 
        {50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50},
        {50, 50, 50, 50, 50, 2,  2,  2,  2,  2,  2,  2, 2, 2, 50, 50, 50, 50, 50, 50}, // Début Rectangle
        {50, 50, 50, 50, 50, 2,  1,  0,  1,  0,  1,  0, 1, 2, 50, 50, 50, 50, 50, 50}, 
        {50, 50, 50, 50, 50, 2,  1,  0,  1,  0,  1,  0, 1, 2, 50, 50, 50, 50, 50, 50},
        {50, 50, 50, 50, 50, 50,  1,  0,  1,  0,  1,  0, 1, 2, 50, 50, 50, 50, 50, 50}, 
        {50, 50, 50, 50, 50, 50,  1,  0,  1,  0,  1,  0, 1, 2, 50, 50, 50, 50, 50, 50}, 
        {50, 50, 50, 50, 50, 50,  1,  0,  1,  0,  1,  0, 1, 2, 50, 50, 50, 50, 50, 50}, 
        {50, 50, 50, 50, 50, 2,  1,  0,  1,  0,  1,  0, 1, 2, 50, 50, 50, 50, 50, 50},
        {50, 50, 50, 50, 50, 2,  1,  0,  1,  0,  1,  0, 1, 2, 50, 50, 50, 50, 50, 50},
        {50, 50, 50, 50, 50, 2,  2,  2,  2,  2,  2,  2, 2, 2, 50, 50, 50, 50, 50, 50}, // Fin Rectangle
        {50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50}, // Marge Bas
        {50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50}, 
        {50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50}
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
int showInteractPromptTente = 0;
int showInteractPromptLit = 0;
int showInteractPromptChaise = 0;
int isSleeping = 0;
int isSitting = 0;
int sleepTimer = 0;
static int toucheX_Relache = 1;
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


    // Test pour le fantome
    fantome.x = 8 * TILE_SIZE; 
    fantome.y = 11 * TILE_SIZE;
    fantome.w = 15;
    fantome.h = 15;
    fantome.direction = 0; 
    fantome.timer = 0;     

    // Chargement des sons
    // sonTransition = chargement_son_transition();
    sonPickUp = chargement_son_item_pick_up();
    sonOpenDoor = chargement_son_door_open();
    sonCloseDoor = chargement_son_door_close();
    MusicInterior = chargement_son_ambiance();
    MusicExterior = chargement_son_exterieur();
    
    // currentLevel = 5;
    // player.x = 20; 
    // player.y = 12*TILE_SIZE ;
    // hasDoudou = 1;

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

// Vérifie si une position est dans la zone 3D d'un mur en face
int isInWallFace3DZone(float x, float y) {
    int caseX = x / TILE_SIZE;
    int caseY = y / TILE_SIZE;
    if (caseX < 0 || caseX >= MAP_WIDTH || caseY < 0 || caseY >= MAP_HEIGHT) return 0;
    int type = maps[currentLevel][caseY][caseX];
    int type_pattern = maps_patern[currentLevel][caseY][caseX];
    
    if (type == 2 || type_pattern == 2 || type == 83) {
        // Vérifier si c'est un mur EN FACE (pas de mur en dessous)
        int caseY_Below = caseY + 1;
        if (caseY_Below < MAP_HEIGHT) {
            int typeBelow = maps[currentLevel][caseY_Below][caseX];
            int typeBelow_pattern = maps_patern[currentLevel][caseY_Below][caseX];
            if (typeBelow != 2 && typeBelow_pattern != 2 && typeBelow != 83) {
                // C'est un mur en face, vérifier si on est dans la zone 3D
                int localY = (int)y % TILE_SIZE;
                if (localY >= 4) {
                    return 1;  // Dans la zone 3D
                }
            }
        }
    }
    return 0;
}

// Fonction utilitaire collision - MOUVEMENT HORIZONTAL
// playerX, playerY = position ACTUELLE du joueur (pour savoir s'il est déjà dans un mur)
int isWallX_withContext(float x, float y, float playerX, float playerY) {
    int caseX = x / TILE_SIZE;
    int caseY = y / TILE_SIZE;
    if (caseX < 0 || caseX >= MAP_WIDTH || caseY < 0 || caseY >= MAP_HEIGHT) return 1;
    int type = maps[currentLevel][caseY][caseX];
    int type_pattern = maps_patern[currentLevel][caseY][caseX];

    // Si le joueur est DÉJÀ dans la zone 3D d'un mur en face
    int playerInWall3D = isInWallFace3DZone(playerX, playerY) || isInWallFace3DZone(playerX + 8, playerY) ||
                         isInWallFace3DZone(playerX, playerY + 8) || isInWallFace3DZone(playerX + 8, playerY + 8);

    // Pour les murs
    if (type == 2 || type_pattern == 2 || type == 83){
        // Si le joueur est DÉJÀ dans un mur en face (zone 3D)
        if (playerInWall3D) {
            // Vérifier si la destination est aussi un mur en face (pas de mur en dessous)
            int caseY_Below = caseY + 1;
            if (caseY_Below < MAP_HEIGHT) {
                int typeBelow = maps[currentLevel][caseY_Below][caseX];
                int typeBelow_pattern = maps_patern[currentLevel][caseY_Below][caseX];
                if (typeBelow != 2 && typeBelow_pattern != 2 && typeBelow != 83) {
                    // Destination = mur en face, vérifier zone 3D
                    int localY = (int)y % TILE_SIZE;
                    if (localY >= 4) {
                        return 0;  // Glissement autorisé dans la zone 3D
                    }
                }
            }
        }
        // Sinon le mur est SOLIDE (on ne peut pas entrer par le côté)
        return 1;
    }

    // Si le joueur est dans la zone 3D et la destination est VIDE (pas un mur)
    // => Autoriser la sortie du mur
    if (playerInWall3D) {
        return 0;  // Sortie du mur autorisée
    }

    // Meubles solides pour mouvement horizontal
    if (type == 10 || type == 11 || type == 14 || type == 15 || type == 18 || type == 19) {
        int localY = (int)y % TILE_SIZE;
        if (localY < 4) return 1;
        return 0;
    }

    if (type == 22 || type == 23) {
        int localY = (int)y % TILE_SIZE;
        if (localY > 2) return 1;
        return 0;
    }
    
    if (type == 20 || type == 21) return 1;
    
    // Collision avec le lit (32, 33 = haut, 34, 35 = bas, 36, 37 = déco) avec effet de profondeur
    if (type == 32 || type == 33 || type == 34 || type == 35) {
        int localY = (int)y % TILE_SIZE;
        if (localY < 6) return 1; // Partie basse du lit solide
        return 0; // Partie haute traversable (effet 3D)
    }
    if (type == 36 || type == 37) return 1; // Déco du lit toujours solide
    
    // Collision avec le bureau (46, 47, 48, 49) - pas la chaise pour pouvoir s'asseoir
    if (type == 46 || type == 47 || type == 48 || type == 49) return 1;
    
    // Collision avec la tente (52, 53, 54 = bas, 55, 56, 57 = haut)
    if (type == 52 || type == 53 || type == 54 || type == 55 || type == 56 || type == 57) return 1;
    
    // Collision avec la commode mur gauche (42, 43) et mur droit (80, 81)
    if (type == 42 || type == 43 || type == 80 || type == 81) return 1;
    
    return 0;
}

// Fonction simple pour compatibilité
int isWallX(float x, float y) {
    int caseX = x / TILE_SIZE;
    int caseY = y / TILE_SIZE;
    if (caseX < 0 || caseX >= MAP_WIDTH || caseY < 0 || caseY >= MAP_HEIGHT) return 1;
    int type = maps[currentLevel][caseY][caseX];
    int type_pattern = maps_patern[currentLevel][caseY][caseX];

    if (type == 2 || type_pattern == 2 || type == 83) return 1;
    if (type == 10 || type == 11 || type == 14 || type == 15 || type == 18 || type == 19) {
        int localY = (int)y % TILE_SIZE;
        if (localY < 4) return 1;
        return 0;
    }
    if (type == 22 || type == 23) {
        int localY = (int)y % TILE_SIZE;
        if (localY > 2) return 1;
        return 0;
    }
    if (type == 20 || type == 21) return 1;
    
    // Collision avec le lit (32, 33 = haut, 34, 35 = bas, 36, 37 = déco) avec effet de profondeur
    if (type == 32 || type == 33 || type == 34 || type == 35) {
        int localY = (int)y % TILE_SIZE;
        if (localY < 6) return 1; // Partie basse du lit solide
        return 0; // Partie haute traversable (effet 3D)
    }
    if (type == 36 || type == 37) return 1; // Déco du lit toujours solide
    
    // Collision avec le bureau (46, 47, 48, 49) - pas la chaise pour pouvoir s'asseoir
    if (type == 46 || type == 47 || type == 48 || type == 49) return 1;
    
    // Collision avec la tente (52, 53, 54 = bas, 55, 56, 57 = haut)
    if (type == 52 || type == 53 || type == 54 || type == 55 || type == 56 || type == 57) return 1;
    
    // Collision avec la commode mur gauche (42, 43) et mur droit (80, 81)
    if (type == 42 || type == 43 || type == 80 || type == 81) return 1;
    
    return 0;
}

// Fonction utilitaire collision - MOUVEMENT VERTICAL (effet 3D pour murs en face)
int isWallY(float x, float y) {
    int caseX = x / TILE_SIZE;
    int caseY = y / TILE_SIZE;
    if (caseX < 0 || caseX >= MAP_WIDTH || caseY < 0 || caseY >= MAP_HEIGHT) return 1;
    int type = maps[currentLevel][caseY][caseX];
    int type_pattern = maps_patern[currentLevel][caseY][caseX];

    // --- MURS AVEC EFFET 3D ---
    if (type == 2 || type_pattern == 2 || type == 83){
        // Vérifier si c'est un mur latéral (mur en dessous)
        int caseY_Below = caseY + 1;
        if (caseY_Below < MAP_HEIGHT) {
            int typeBelow = maps[currentLevel][caseY_Below][caseX];
            int typeBelow_pattern = maps_patern[currentLevel][caseY_Below][caseX];
            if (typeBelow_pattern == 2 || typeBelow == 83) {
                return 1;  // Mur latéral - complètement solide
            }
        } else {
            return 1;
        }

        // Mur en face - EFFET 3D uniforme sur TOUTE la longueur
        int localY = (int)y % TILE_SIZE;
        if (localY < 4) {
            return 1; // Bas du mur solide
        }
        return 0; // Haut traversable (effet 3D)
    }

    // Meubles avec profondeur
    if (type == 10 || type == 11 || type == 14 || type == 15 || type == 18 || type == 19) {
        int localY = (int)y % TILE_SIZE;
        if (localY < 4) return 1;
        return 0;
    }

    if (type == 22 || type == 23) {
        int localY = (int)y % TILE_SIZE;
        if (localY > 2) return 1;
        return 0;
    }
    
    if (type == 20 || type == 21) return 1;
    
    // Collision avec le lit (32, 33 = haut, 34, 35 = bas, 36, 37 = déco) avec effet de profondeur
    if (type == 32 || type == 33 || type == 34 || type == 35) {
        int localY = (int)y % TILE_SIZE;
        if (localY < 6) return 1; // Partie basse du lit solide
        return 0; // Partie haute traversable (effet 3D)
    }
    if (type == 36 || type == 37) return 1; // Déco du lit toujours solide
    
    // Collision avec le bureau (46, 47, 48, 49) - pas la chaise pour pouvoir s'asseoir
    if (type == 46 || type == 47 || type == 48 || type == 49) return 1;
    
    // Collision avec la tente (52, 53, 54 = bas, 55, 56, 57 = haut)
    if (type == 52 || type == 53 || type == 54 || type == 55 || type == 56 || type == 57) return 1;
    
    // Collision avec la commode mur gauche (42, 43) et mur droit (80, 81)
    if (type == 42 || type == 43 || type == 80 || type == 81) return 1;
    
    return 0;
}

// Fonction de compatibilité (pour les autres usages)
int isWall(float x, float y) {
    return isWallX(x, y) || isWallY(x, y);
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
        Mix_VolumeMusic(VOLUME_MUSIQUE);
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

    // Gestion du sommeil - décrémenter le timer
    if (isSleeping) {
        if (sleepTimer > 0) {
            sleepTimer--;
            if (sleepTimer <= 0) {
                isSleeping = 0;
            }
        }
        // Permettre de se réveiller avec X même pendant le sommeil
        if (state[SDL_SCANCODE_X]) {
            if (toucheX_Relache) {
                isSleeping = 0;
                sleepTimer = 0;
                // Le joueur sort du lit - se téléporte à côté
                player.x = 2 * TILE_SIZE; // À gauche du lit
                player.y = 3 * TILE_SIZE; // À la hauteur du lit
                toucheX_Relache = 0;
            }
        } else {
            toucheX_Relache = 1;
        }
        // Si sleepTimer == -1, sommeil infini jusqu'à ce qu'on appuie sur X
        // Reset les prompts d'interaction pendant le sommeil
        showInteractPrompt = 0;
        showInteractPrompt2 = 0;
        showInteractPrompt3 = 0;
        showInteractPromptTente = 0;
        showInteractPromptLit = 0;
        showInteractPromptChaise = 0;
        return; // Le joueur ne peut pas bouger pendant qu'il dort
    }

    // Gestion quand le joueur est assis sur la chaise
    if (isSitting) {
        // Permettre de se lever avec X
        if (state[SDL_SCANCODE_X]) {
            if (toucheX_Relache) {
                isSitting = 0;
                // Le joueur se lève - se téléporte à côté de la chaise
                player.x = 5 * TILE_SIZE; // À gauche de la chaise
                player.y = 12 * TILE_SIZE; // À la hauteur de la chaise
                toucheX_Relache = 0;
            }
        } else {
            toucheX_Relache = 1;
        }
        // Reset les prompts d'interaction pendant qu'on est assis
        showInteractPrompt = 0;
        showInteractPrompt2 = 0;
        showInteractPrompt3 = 0;
        showInteractPromptTente = 0;
        showInteractPromptLit = 0;
        showInteractPromptChaise = 0;
        return; // Le joueur ne peut pas bouger pendant qu'il est assis
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
    // Collision X - utilise isWallX_withContext pour permettre le glissement sur les murs en face
    int touchWallX = 0;
    if (isWallX_withContext(nextX, player.y, player.x, player.y)) touchWallX = 1;
    if (isWallX_withContext(nextX + player.w, player.y, player.x, player.y)) touchWallX = 1;
    if (isWallX_withContext(nextX, player.y + player.h, player.x, player.y)) touchWallX = 1;
    if (isWallX_withContext(nextX + player.w, player.y + player.h, player.x, player.y)) touchWallX = 1;

    if (!touchWallX) player.x = nextX;

    
    // Collision Y - utilise isWallY (effet 3D pour murs en face)
    int touchWallY = 0;
    if (isWallY(player.x, nextY)) touchWallY = 1;
    if (isWallY(player.x + player.w, nextY)) touchWallY = 1;
    if (isWallY(player.x, nextY + player.h)) touchWallY = 1;
    if (isWallY(player.x + player.w, nextY + player.h)) touchWallY = 1;

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

    float tenteX = 16 * TILE_SIZE + 8; 
    float tenteY = 7 * TILE_SIZE + 8; 

    // Calcul de la distance entre le joueur et l'armoire
    float dx_tente = (player.x + player.w / 2) - tenteX;
    float dy_tente = (player.y + player.h / 2) - tenteY;
    float distance_tente = sqrt(dx_tente*dx_tente + dy_tente*dy_tente);

    if(distance_tente <= 24 && currentLevel == 0 && maps[0][6][16] == 55){
        showInteractPromptTente = 1;
    }
    else{
        showInteractPromptTente = 0;
    }

    // Calcul de la distance entre le joueur et le lit (lit en position 3,4 colonnes, lignes 2-3)
    float litX = 3 * TILE_SIZE + TILE_SIZE; // Centre du lit (entre colonnes 3 et 4)
    float litY = 3 * TILE_SIZE + 8; // Centre du lit (entre lignes 2 et 3)

    float dx_lit = (player.x + player.w / 2) - litX;
    float dy_lit = (player.y + player.h / 2) - litY;
    float distance_lit = sqrt(dx_lit*dx_lit + dy_lit*dy_lit);

    if(distance_lit <= 28 && currentLevel == 0 && !isSleeping){
        showInteractPromptLit = 1;
    }
    else{
        showInteractPromptLit = 0;
    }

    // Calcul de la distance entre le joueur et la chaise (chaise en position colonne 6, ligne 12)
    float chaiseX = 6 * TILE_SIZE + 8; // Centre de la chaise
    float chaiseY = 12 * TILE_SIZE + 8; // Centre de la chaise

    float dx_chaise = (player.x + player.w / 2) - chaiseX;
    float dy_chaise = (player.y + player.h / 2) - chaiseY;
    float distance_chaise = sqrt(dx_chaise*dx_chaise + dy_chaise*dy_chaise);

    if(distance_chaise <= 24 && currentLevel == 0 && !isSitting){
        showInteractPromptChaise = 1;
    }
    else{
        showInteractPromptChaise = 0;
    }

    if(IsLocationLeft(6, 10, 9, 6*TILE_SIZE-8)){
        player.x = 15 * TILE_SIZE;
        currentLevel = 0;
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

            else if(distance_tente <= 24 && currentLevel == 0 && maps[0][6][16] == 55){
                currentLevel = 9;
                player.x = 7 * TILE_SIZE;
                player.y = 7 * TILE_SIZE;
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

    // Gestion de la touche X pour dormir/se réveiller sur le lit OU s'asseoir/se lever de la chaise
    if (state[SDL_SCANCODE_X]) {
        if (toucheX_Relache) {
            // Si le joueur dort déjà, il se réveille et sort du lit
            if (isSleeping) {
                isSleeping = 0;
                sleepTimer = 0;
                // Le joueur sort du lit - se téléporte à côté
                player.x = 2 * TILE_SIZE; // À gauche du lit
                player.y = 3 * TILE_SIZE; // À la hauteur du lit
            }
            // Si le joueur est assis sur la chaise, il se lève
            else if (isSitting) {
                isSitting = 0;
                // Le joueur se lève - se téléporte à côté de la chaise
                player.x = 5 * TILE_SIZE; // À gauche de la chaise
                player.y = 12 * TILE_SIZE; // À la hauteur de la chaise
            }
            // Sinon, s'il est près du lit, il s'allonge pour dormir
            else if (distance_lit <= 28 && currentLevel == 0) {
                isSleeping = 1;
                sleepTimer = -1; // Durée infinie jusqu'à ce qu'on appuie à nouveau sur X
                // Le joueur s'allonge sur le lit
                player.x = 3 * TILE_SIZE + 4; // Position X sur le lit
                player.y = 2 * TILE_SIZE + 8; // Position Y sur le lit
            }
            // Sinon, s'il est près de la chaise, il s'assoit
            else if (distance_chaise <= 24 && currentLevel == 0) {
                isSitting = 1;
                // Le joueur s'assoit sur la chaise
                player.x = 6 * TILE_SIZE + 2; // Position X sur la chaise
                player.y = 12 * TILE_SIZE + 4; // Position Y sur la chaise
            }
            toucheX_Relache = 0;
        }
    } else {
        toucheX_Relache = 1;
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
        player.x = 5;     // On apparaît tout en HAUT de la chambre
        
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


    // 1. Entrée dans le labyrinthe (Niveau 2 -> 5)
    if(IsLocationRight(6, 10, 2, 20)){
        currentLevel = 5;
        player.x = 5;
        SpawnFantomeRandom(); // <--- NOUVEAU
    }

    // 2. Retour couloir (5 -> 2)
    else if (IsLocationLeft(6, 10, 5, 5)) {
        currentLevel = 2;
        player.x = (MAP_WIDTH * TILE_SIZE) - 20;
        // Pas de fantôme dans le couloir, on le laisse où il est (invisible)
    }

    // 3. Passage 5 -> 6
    if(IsLocationRight(11, 14, 5, 20)){
        currentLevel = 6;
        player.x = 5;
        SpawnFantomeRandom(); // <--- NOUVEAU
    }
    // Retour 6 -> 5
    else if (IsLocationLeft(11, 14, 6, 5)) {
        currentLevel = 5;
        player.x = (MAP_WIDTH * TILE_SIZE) - 20;
        SpawnFantomeRandom(); // <--- NOUVEAU (Le fantôme change de place quand on revient !)
    }

    // 4. Passage 6 -> 7
    if(IsLocationDown(10, 13, 6, 20)){
        currentLevel = 7;
        player.y = 10;
        SpawnFantomeRandom(); // <--- NOUVEAU
    }
    // Retour 7 -> 6
    else if(IsLocationUp(10, 13, 7, 5)){
        currentLevel = 6;
        player.y = (MAP_HEIGHT * TILE_SIZE) - 20;
        SpawnFantomeRandom(); // <--- NOUVEAU
    }

    // 5. Passage 7 -> 8 (Dernier niveau)
    if(IsLocationLeft(11, 14, 7, 5)){
        currentLevel = 8;
        player.x = (MAP_WIDTH * TILE_SIZE) - 20;
        SpawnFantomeRandom(); // <--- NOUVEAU
    }
    // Retour 8 -> 7
    else if(IsLocationRight(11, 14, 8, 20)){
        currentLevel = 7;
        player.x = 5;
        SpawnFantomeRandom(); // <--- NOUVEAU
    }


    // Changement de son d'ambiance
    ManageMusic();

    if (currentLevel >= 5) {
        ActionFantome(200); 
    }

    // --- GESTION COLLISION JOUEUR / FANTOME (GAME OVER / RESET) ---
    if (currentLevel >= 5 && currentLevel <=8) {
        
        // 1. On définit la hitbox d'attaque du fantôme
        // On veut qu'il attrape plus haut (pour la perspective) et un peu plus large
        float killZoneHaut = fantome.y - 12; // Il attrape 12 pixels au-dessus de sa tête !
        float killZoneBas  = fantome.y + fantome.h; 
        float killZoneGauche = fantome.x; 
        float killZoneDroite = fantome.x + fantome.w;

        // 2. On vérifie si le joueur est dans cette zone
        if (player.x < killZoneDroite &&
            player.x + player.w > killZoneGauche &&
            player.y < killZoneBas &&
            player.y + player.h > killZoneHaut) // C'est cette ligne qui change tout
        {
            printf("GAME OVER - ATTRAPE !\n");
            
            // RESET
            currentLevel = 0; 
            player.x = 80; 
            player.y = 50;
            
            fantome.x = 8 * TILE_SIZE; 
            fantome.y = 11 * TILE_SIZE;
            fantome.timer = 0; 
        }
    }


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
             if (maps[currentLevel][ly][lx] == 21 || (maps[currentLevel][ly][lx] >= 75 && maps[currentLevel][ly][lx] <= 76) || maps[currentLevel][ly][lx] >= 85 || maps[currentLevel][ly][lx] >= 86 ) { // Si c'est une lampe
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

        DrawTexte(texteAffiche, renderer, font, 20, 180 ,280, 50);
    }
    if (dialogueStep_sortie1 > 0) {
        char *texteAffiche = "";
        if (dialogueStep_sortie1 == 1) texteAffiche = "je peux pas sortir sans mon doudou...";
        DrawTexte(texteAffiche, renderer, font, 20, 180 ,280, 50);
    }
    if (dialogue_hasDoudou > 0) {
        char *texteAffiche = "";
        if (dialogue_hasDoudou == 1) texteAffiche = "je te tiens";
        if (dialogue_hasDoudou == 2) texteAffiche = "OH...";
        if (dialogue_hasDoudou == 3) texteAffiche = "De la lumiere !";
        DrawTexte(texteAffiche, renderer, font, 20, 180 ,280, 50);
    }

    

    SDL_Rect srcPlayer = { 112, 0, 16, 16 };
    SDL_Rect destPlayer = { (int)player.x - 2, (int)player.y - 2, 16, 16 };
    SDL_RenderCopy(renderer, tilesetTexture, &srcPlayer, &destPlayer);  

    // Dessiner les éléments qui doivent passer DEVANT le joueur (panneau de basket, etc.)
    if (tilesetTexture && currentLevel == 0) {
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                int type_maps = maps[currentLevel][y][x];
                // Panneau de basket (78, 79) - doit être dessiné par-dessus le joueur
                if (type_maps == 78 || type_maps == 79) {
                    float intensite = getLuminosite(x, y, rayon);
                    if (intensite > 0.0f) {
                        int lum = (int)(intensite * 255);
                        DrawTuiles(x, y, type_maps, renderer, lum);
                    }
                }
            }
        }
    }

    int caseX = (int)(fantome.x / TILE_SIZE);
    int caseY = (int)(fantome.y / TILE_SIZE);

    // On l'affiche s'il est éclairé ET qu'on est dans un niveau de labyrinthe
    if (estEclaire(caseX, caseY, rayon) && currentLevel >= 5 && currentLevel <= 8) {
        SDL_Rect src = { 63 * TILE_SIZE, 0, 16, 16 }; 
        SDL_Rect dest = { (int)fantome.x, (int)fantome.y, 16, 16 }; 
        SDL_RenderCopy(renderer, tilesetTexture, &src, &dest);
    }

    if (showInteractPrompt == 1) {
        SDL_Color cBlanc = {255, 255, 255, 255};
        SDL_Surface *sText = TTF_RenderText_Solid(fontMini, "[E] Ouvrir", cBlanc);
        if (sText) DrawInteractions(renderer, sText);
    }

     if (showInteractPrompt2 == 1) {
        SDL_Color cBlanc = {255, 255, 255, 255};
        SDL_Surface *sText = TTF_RenderText_Solid(fontMini, "[ENTER] Interagir", cBlanc);
        
        if (sText) DrawInteractions(renderer, sText);
    }
    if (showInteractPrompt3 == 1) {
        SDL_Color cBlanc = {255, 255, 255, 255};
        SDL_Surface *sText = TTF_RenderText_Solid(fontMini, "[E] Fermer", cBlanc);
        
        if (sText) DrawInteractions(renderer, sText);
    }
    if (showInteractPromptTente == 1)            
    {
        SDL_Color cBlanc = {255, 255, 255, 255};
        SDL_Surface *sText = TTF_RenderText_Solid(fontMini, "[E] Entrer", cBlanc);
        
        if (sText) DrawInteractions(renderer, sText);
    }
    if (showInteractPromptLit == 1)            
    {
        SDL_Color cBlanc = {255, 255, 255, 255};
        SDL_Surface *sText = TTF_RenderText_Solid(fontMini, "[X] Dormir", cBlanc);
        
        if (sText) DrawInteractions(renderer, sText);
    }
    if (showInteractPromptChaise == 1)            
    {
        SDL_Color cBlanc = {255, 255, 255, 255};
        SDL_Surface *sText = TTF_RenderText_Solid(fontMini, "[X] S'asseoir", cBlanc);
        
        if (sText) DrawInteractions(renderer, sText);
    }


    // Effet de sommeil (petit "Zzz" près du joueur)
    if (isSleeping) {
        // Fond semi-transparent noir léger
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
        SDL_Rect overlay = {0, 0, LOGICAL_WIDTH, LOGICAL_HEIGHT};
        SDL_RenderFillRect(renderer, &overlay);

        // Afficher "Zzz" en petit près du joueur
        SDL_Color cBlanc = {255, 255, 255, 255};
        SDL_Surface *sZzz = TTF_RenderText_Solid(fontMini, "Zzz...", cBlanc);
        if (sZzz) {
            SDL_Texture *tZzz = SDL_CreateTextureFromSurface(renderer, sZzz);
            SDL_Rect rZzz = { (int)player.x + 10, (int)player.y - 10, sZzz->w, sZzz->h };
            SDL_RenderCopy(renderer, tZzz, NULL, &rZzz);
            SDL_FreeSurface(sZzz);
            SDL_DestroyTexture(tZzz);
        }
    }
}

void DrawInteractions(SDL_Renderer *renderer, SDL_Surface *sText){
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

void DrawTexte(char *texteAffiche, SDL_Renderer *renderer,TTF_Font *font, int x, int y, int w, int h){
        SDL_Rect rectBulle = { x, y, w, h };
        
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