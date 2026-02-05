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
#define VOLUME_BRUITAGES 32

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
-tente-
87,88,89,90,91 = bas tapis
92,93,94,95,96 = mid tapis v1
97,98,99,100,101 = mid tapis v2
102,103,104,105,106 = fin tapis
147 = doudou duolingo
148 = lampe 
149 = cookie 
150 = mur tente 
151,152 = leds rouge
174 = dessin theo
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
71 72 = table avec broc d'eau 
71, 172 = table avec broc sans eau
71, 173 = table sans broc
73 = vase
74, 75 = lampe murale gauche
76, 77 = lampe murale droite
...........................
-haul d'entrée-
84(bas), 85(tete rouge) = statue mur gauche 
86 = tete statue yeux verts 
107, 108 = bas tableau 
109, 110 = haut tableau
111,112 = bas tableau vide 
113,114 = haut tableau vide
115(bas gauche),116(bas droit),117,118 = morceaux tableau
119-224 = bas tapis
125-130 = haut tapis
131 = tableau pomme
13(haut),133 = commode mur gauche
134,135 = table mur haut
136,137 = pieds de la table + chat
138 = carreau foncé 
139,140 = bas mirroir
141,142 = haut mirroir
143,144 = pot de fleurs
145,146 = fleur du pot de fleurs
153(bas),154 = fenetre 
...........................
-cuisine-
155 = sol
156(g),157(d) = bas bouche
158(g),159(d) = haut bouche 
160-166 = grande table 
167,168 = lavabo 
169,170 = plaque cuisson 
171 = bloc seul
175 = pain + oeil
176 = os
177 = crane + chenille
178 = poils + seringue
179, 180 = grimoire
181 = tirroir mur gauche 
182,183 = bouteille renversée 
184 = toile d'araignée
.........................
-chambre parents-
185 = commode avec télécommande (bas)
186 = suite commode (haut)
187,188,189, 190 = lit haut avec monstre qui dort 
191,192,193,194 = lit bas
195,196,197,198 = lit haut haut
199,200,201,202 = lit bas bas


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
        {2, 2, 2, 2, 2, 2, 2,  2,172, 67, 68,  0,  2,  2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2,  2, 71, 67, 68,  0,  2,  2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2,  2,  0, 67, 68, 76, 77,  2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 74, 75, 67, 68,  0,  2,  2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2,  2,  0, 67, 68, 73,  2,  2, 2, 2, 2, 2, 2, 2}, // Bas (Entrée)
        {2, 2, 2, 2, 2, 2, 2,  2,  0, 67, 68,  0,  2,  2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2,  2,  0, 65, 66,  0,  2,  2, 2, 2, 2, 2, 2, 2}
    }, 
    // CARTE 3 : HALL (Niveau 2)
    {         
        {2,  2,  2,    2,  2,    2,    2,  2,    2,  2,  2,   2,    2,    2,    2,   2,     2,  2,  2, 2}, // Trou en haut (tout fermé en 2 ici ?)
        {2,  2,  2,    2,  2,  113,  114,  2,    0,  0,  0,   0,    2,    2,  131,   2,   141,142,  2, 2},
        {2,  2,  2,    2,  2,  111,  112,  2,    0,  0,  0,   0,    2,  134,  135,   2,   139,140,  2, 2},
        {2,  2,145,  146,  0,    0,    0,  0,    0,  0,  0,   0,    0,  136,  137,   0,     0,  0,  2, 2},
        {2,  2,143,  144,  0,    0,    0,  0,    0,  0,  0,   0,    0,    0,    0,   0,     0,  0,  2, 2},
        {2,  0,  0,    0,  0,    0,    0,  0,    0,  0,  0,   0,    0,    0,    0,   0,     0, 85,  2, 2},
        {2,  0,  0,    0,  0,    0,    0,  0,    0,  0,  0,   0,    0,    0,    0,   0,     0, 84,  2, 2},
        {2,125,126,  126,126,  126,  126,126,  126,126,126, 126,  126,  126,  126,  126,  126,130,  0, 0},
        {2,119,120,  120,120,  120,  120,120,  120,120,120, 120,  120,  120,  120,  120,  120,124,  0, 0},
        {2,  2,  0,    0,  0,    0,    0,  0,    0,  0,  0,   0,    0,    0,    0,   0,     0, 85,  2, 2},
        {2,  2,  0,    0,  0,    0,    0,  0,    0,  0,  0,   0,    0,    0,    0,   0,     0, 84,  2, 2},
        {2,  2,  0,    0,  0,    0,    0,  0,    0,  0,  0,   0,    0,    0,    0,   0,     0,  0,  2, 2},
        {2,  2,  0,    0,  0,    0,    0,  0,    0,  0,  0,   0,    0,    0,    0,   0,     0,  0,  2, 2}, // Bas fermé
        {2,  2,  0,    0,  0,    0,    0,  0,    0,  0,  0,   0,    0,    0,    0,   0,     0,  0,  2, 2},
        {2,  2,  2,    2,  2,    2,    2,  2,    0,  0,  0,   0,    2,    2,    2,   2,     2,  2,  2, 2}
    },  
    // CARTE 4 (Niveau 3) index 3
    {       
        {2, 2, 2,  2,  2,   2,  2,  2,  0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2}, // Trou en haut
        {2, 2, 2,  2,  2,   2,  2,  2,  0, 0, 0, 0, 2, 2, 2, 2, 2, 8, 2, 2},
        {2, 2, 0,  0,  0,   0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0,  0,  0,   0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0,  0,  0,   0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0,182,183,   0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  
        {2,184, 0,  0,  0,   0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {2, 2, 0,  0,  0, 176,  0,175,  0, 0, 0, 0,158,159, 0, 0, 0, 0, 0, 0},
        {2, 2, 0,  0,  0,   0,  0,  0,  0, 0, 0, 0,156,157, 0, 0, 0, 0, 0, 0},
        {2, 2,181,  0,  0, 177,179,180,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2,171,  0,  0,   0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2,181,  0,  0,   0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0,  0,  0,   0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2}, // Bas fermé
        {2, 2, 0,178,169, 170,167,168,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 2,  2,  2,   2,  2,  2,  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}
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
        { 2, 82, 82, 82, 83, 82, 82, 82, 82, 82, 83, 82, 82, 82, 83, 82, 82, 83, 117, 83}, 
        { 2, 83, 83, 82, 83, 82, 83, 83, 83, 82, 83, 82, 83, 82, 83, 82, 83, 83, 82, 83},
        { 2, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83, 82, 82, 82, 82, 82, 82, 83},
        { 2, 82, 83, 83, 83, 82, 83, 83, 83, 83, 83, 82, 83, 83, 83, 83, 83, 83, 82, 83}, 
        { 2, 82, 83, 82, 82, 82, 82, 82, 83, 82, 82, 82, 82, 82, 82, 82, 82, 83, 82, 83},
        { 2, 82, 83, 82, 83, 83, 83, 82, 83, 82, 83, 83, 83, 83, 83, 83, 82, 83, 82, 83}, 
        { 82, 82, 82, 82, 83, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83}, 
        { 82, 82, 83, 83, 83, 82, 83, 83, 83, 83, 83, 83, 83, 83, 83, 82, 83, 83, 83, 83}, 
        { 2, 82, 82, 82, 82, 82, 83, 82, 82, 82, 83, 82, 82, 82, 83, 82, 82, 82, 82, 83},
        { 2, 83, 83, 83, 83, 82, 83, 82, 83, 82, 83, 82, 83, 82, 83, 83, 83, 83, 82, 83}, 
        { 2, 82, 82, 82, 83, 82, 82, 82, 83, 82, 82, 82, 83, 82, 82, 82, 82, 82, 82, 83},
        { 2, 82, 83, 82, 83, 83, 83, 83, 83, 83, 83, 82, 83, 83, 83, 83, 83, 83, 82, 82}, 
        { 2, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83}, 
        { 2, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83}
    },

    // --- LABYRINTHE 2 (Index 6) ---
    {
        {83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83}, // Mur Haut
        {83, 82, 82, 82, 82, 82, 83, 82, 82, 82, 83, 82, 82, 82, 82, 82, 82, 82, 82, 83},
        {83, 82, 83, 83, 83, 82, 83, 82, 83, 83, 83, 82, 83, 83, 83, 83, 83, 83, 82, 83},
        {83, 82, 83, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83, 118, 82, 82, 82, 83, 82, 83},
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
        {83, 83, 83, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 115, 82, 83},
        {83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83}  // Mur Bas fermé
    },

    // --- LABYRINTHE 4 (Index 8) ---
    {
        {83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83},
        {83, 82, 82, 82, 82, 82, 82, 82, 83, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83},
        {83, 82, 83, 83, 83, 83, 83, 82, 83, 82, 83, 83, 83, 83, 83, 83, 83, 83, 82, 83},
        {83, 116, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83, 82, 83},
        {83, 82, 83, 82, 83, 83, 83, 83, 83, 83, 83, 82, 83, 83, 83, 82, 82, 83, 82, 83},
        {83, 82, 83, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83, 82, 82, 82, 82, 83, 82, 83},
        {83, 82, 83, 83, 83, 82, 83, 83, 83, 83, 83, 82, 83, 82, 83, 83, 83, 83, 82, 83},
        {83, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83},
        {83, 82, 83, 83, 83, 83, 83, 82, 83, 83, 83, 83, 83, 82, 83, 83, 83, 83, 82, 83},
        {83, 82, 83, 82, 82, 82, 82, 82, 83, 82, 82, 82, 83, 82, 82, 82, 82, 83, 82, 83},
        {83, 82, 83, 82, 83, 83, 83, 83, 83, 82, 83, 82, 83, 83, 83, 83, 82, 83, 82, 83},
        {83, 82, 82, 82, 83, 82, 82, 82, 82, 82, 83, 82, 82, 82, 82, 82, 82, 83, 82, 83},
        {83, 83, 83, 82, 83, 82, 83, 83, 83, 83, 83, 82, 83, 83, 83, 83, 82, 82, 82, 82},
        {83, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83},
        {83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83}
    },

    // TENTE (index 9)
    {
        {50, 50, 50, 50, 50, 50, 50,  50,  50,  50, 50,  50,   50, 50, 50, 50, 50, 50, 50, 50}, // Marge Haut
        {50, 50, 50, 50, 50, 50, 50,  50,  50,  50, 50,  50,   50, 50, 50, 50, 50, 50, 50, 50}, 
        {50, 50, 50, 50, 50, 50, 50,  50,  50,  50, 50,  50,   50, 50, 50, 50, 50, 50, 50, 50},
        {50, 50, 50, 50, 50,151,152, 151, 152, 151,152, 151, 152, 151, 50, 50, 50, 50, 50, 50}, // Début Rectangle
        {50, 50, 50, 50, 50,150,  1,   0,   1,   0, 58,  41,   1, 150, 50, 50, 50, 50, 50, 50}, 
        {50, 50, 50, 50, 50,150, 44,   0,   1,   0,  1,   0, 147, 150, 50, 50, 50, 50, 50, 50},
        {50, 50, 50, 50, 50,  0,  1,   0,   1,   0,  1,   0,   1, 150, 50, 50, 50, 50, 50, 50}, 
        {50, 50, 50, 50, 50,  0,  1,   0, 148,  59, 60,   0,   1, 150, 50, 50, 50, 50, 50, 50}, 
        {50, 50, 50, 50, 50,  0,  1,   0,   1,   0,149,  45, 174, 150, 50, 50, 50, 50, 50, 50}, 
        {50, 50, 50, 50, 50,150, 58,   0,   1,   0,  1,  40,   1, 150, 50, 50, 50, 50, 50, 50},
        {50, 50, 50, 50, 50,150,  1,   0,   1,   0,  1,   0,   1, 150, 50, 50, 50, 50, 50, 50},
        {50, 50, 50, 50, 50,150,150, 150, 150, 150,150, 150, 150, 150, 50, 50, 50, 50, 50, 50}, // Fin Rectangle
        {50, 50, 50, 50, 50, 50, 50,  50,  50,  50, 50,  50,  50, 50, 50, 50, 50, 50, 50, 50}, // Marge Bas
        {50, 50, 50, 50, 50, 50, 50,  50,  50,  50, 50,  50,  50, 50, 50, 50, 50, 50, 50, 50}, 
        {50, 50, 50, 50, 50, 50, 50,  50,  50,  50, 50,  50,  50, 50, 50, 50, 50, 50, 50, 50}
    },
    // Chambre des parents (index 10)
    {    
        {2, 2,  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}, // Trou en haut
        {2, 2,  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 8, 2, 2},
        {2, 2,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2,186, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2,185, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2,195,196,197,198, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2,187,188,189,190, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2,191,192,193,194, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2,199,200,201,202, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2}, // Bas fermé
        {2, 2,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2,  2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2}
    },
};
 
int dialogueStep = 0; 
int dialogueStep_sortie1 = 0;
int dialogue_hasDoudou = 0;
int dialogue_statue_haut = 0;
int dialogue_statue_bas = 0;
int dialogue_entree_labyrinthe = 0;
int dialogue_max_objet = 0;
int toucheRelache = 0;
int hasDoudou = 0;
int showInteractPrompt = 0;
int showInteractPrompt2 = 0;
int showInteractPrompt3 = 0;
int showInteractPromptTente = 0;
int show_interact_prompt_statue_haut = 0;
int show_interact_prompt_statue_bas = 0;
int show_interact_prompt_eau = 0;
int show_interact_prompt_dessin = 0;
int interact_statue_haut = 0;
int interact_statue_bas = 0;
int has_water = 0;
int has_drawing = 0;
int statue_has_water = 0;
int statue_has_drawing = 0;
SDL_Rect doudouRect = { 200, 150, 12, 12 };
    
int carafeX = -1;
int carafeY = -1;
int dessinX = -1;
int dessinY = -1;

int showInteractPromptObjetTableau = 0;
int showInteractTableau = 0;
int showInteractImpossibleObjet = 0;
int whichTableauPiece = 0;

int cpt_piece_tableau = 0;

int TuilesNotSpecial[] = {0, 1, 2};
int tailleTuilesNotSpecial = (int)sizeof(TuilesNotSpecial) / (int)sizeof(TuilesNotSpecial[0]);

int papaReveil = 0;

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

// Fonction utilitaire collision
int isWall(float x, float y) {
    int caseX = (int)x / TILE_SIZE;
    int caseY = (int)y / TILE_SIZE;

    // Sécurité bornes map
    if (caseX < 0 || caseX >= MAP_WIDTH || caseY < 0 || caseY >= MAP_HEIGHT) {
        return 1;
    }

    int type = maps[currentLevel][caseY][caseX];
    int type_pattern = maps_patern[currentLevel][caseY][caseX];

    // --- LOGIQUE SPÉCIALE LABYRINTHE (83) ---
    if (type == 83) {

        // 1. TOIT (Perspective : Le haut est toujours solide)
        int localY = (int)y % TILE_SIZE;
        if (localY < 6) {
            return 1;
        }

        // 2. ANALYSE ENVIRONNEMENT (haut dessus 82 et en dessous 82)
        int upIs82 = 0;
        int downIs82 = 0;

        if (caseY - 1 >= 0) {
            if (maps[currentLevel][caseY - 1][caseX] == 82) {
                upIs82 = 1;
            }
        }
        if (caseY + 1 < MAP_HEIGHT) {
            if (maps[currentLevel][caseY + 1][caseX] == 82) {
                downIs82 = 1;
            }
        }
        if (upIs82 && downIs82) {
            
            // On regarde si les pieds sont physiquement plus bas que le mur
            float wallPixelBottom = (caseY + 1) * TILE_SIZE;

            if ((player.y + player.h) >= wallPixelBottom) {
                return 0; // Je suis devant le mur, je passe
            }
            
            int localX = (int)x % TILE_SIZE;
            float wallPixelLeft = caseX * TILE_SIZE;
            float wallPixelRight = (caseX + 1) * TILE_SIZE;

            // Test Bord GAUCHE
            if (localX < 4) {
                int leftIs82 = 0;
                if (caseX > 0) {
                    if (maps[currentLevel][caseY][caseX - 1] == 82) {
                        leftIs82 = 1;
                    }
                }

                if (leftIs82) {
                    // Si épaule DROITE est encore DANS le mur (+ marge 2px)
                    if ((player.x + player.w) > (wallPixelLeft + 2)) {
                        return 0; 
                    }
                    return 1; // Sinon je tape
                }
            }

            // Test Bord DROIT
            if (localX > 12) {
                int rightIs82 = 0;
                if (caseX < MAP_WIDTH - 1) {
                    if (maps[currentLevel][caseY][caseX + 1] == 82) {
                        rightIs82 = 1;
                    }
                }

                if (rightIs82) {
                    // Si épaule GAUCHE est encore DANS le mur (- marge 2px)
                    if (player.x < (wallPixelRight - 2)) {
                        return 0; 
                    }
                    return 1; // Sinon je tape
                }
            }

            // Si on n'est ni devant (pieds), ni en train de glisser (côtés), 
            // alors on est DANS le mur -> COLLISION
            return 1;
        }
        
        // --- CAS STANDARD (Gros murs sans passage haut/bas) ---
        int isSolidBelow = 0;
        if (caseY + 1 < MAP_HEIGHT) {
            int typeBelow = maps[currentLevel][caseY + 1][caseX];
            if (typeBelow == 83 || maps_patern[currentLevel][caseY + 1][caseX] == 2) {
                isSolidBelow = 1;
            }
        } else {
            isSolidBelow = 1;
        }

        if (isSolidBelow) {
            return 1;
        }

        return 0;
    }

    // --- RESTE DU CODE (Chambre...) ---
    if (type_pattern == 2 || type_pattern == 150) {
        int caseY_Below = caseY + 1;
        if (caseY_Below < MAP_HEIGHT) {
            int typeBelow = maps[currentLevel][caseY_Below][caseX];
            int typeBelow_pattern = maps_patern[currentLevel][caseY_Below][caseX];
            if (typeBelow_pattern == 2 || typeBelow == 83 || typeBelow_pattern == 150) {
                return 1;
            }
        } else {
            return 1;
        }
        
        if ((int)y % TILE_SIZE < 4) {
            return 1;
        }
        return 0;
    }

    if (type == 10 || type == 11 || type == 14 || type == 15 || type == 18 || type == 19) {
        if ((int)y % TILE_SIZE < 4) {
            return 1;
        } 
        return 0;
    }

    if (type == 22 || type == 23) {
        if ((int)y % TILE_SIZE > 2) {
            return 1;
        } 
        return 0;
    }

    if (type == 20 || type == 21) {
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
    Mix_Volume(-1, VOLUME_BRUITAGES);
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
            Mix_VolumeMusic(16);
            if (MusicInterior) Mix_FadeInMusic(MusicInterior, -1, 1000);
        }
        currentZoneState = newZoneState;
    }

}

// --- UPDATE ---
void UpdateGame(void) {
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    // --- MODE DIALOGUE (Joueur Bloqué) ---
    cpt_piece_tableau=4;
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
    dialogueStep_sortie1 = 0;
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
    if (dialogue_statue_haut> 0) {
        if (state[SDL_SCANCODE_RETURN]) {
            if (toucheRelache) {
                dialogue_statue_haut ++; // On ferme le dialogue
                interact_statue_haut = 1;
                if (dialogue_statue_haut > 2)
                {
                    dialogue_statue_haut = 0;
                }
                
                toucheRelache = 0;
            }
        } else {
            toucheRelache = 1;
        }
        return;
    }
    if (dialogue_statue_bas > 0) {
        if (state[SDL_SCANCODE_RETURN]) {
            if (toucheRelache) {
                dialogue_statue_bas ++; // On ferme le dialogue
                interact_statue_bas = 1;
                if (dialogue_statue_bas > 2)
                {
                    dialogue_statue_bas = 0;
                }
                
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
    dialogue_entree_labyrinthe = 0;
    if (dialogue_entree_labyrinthe > 0) {
        if (state[SDL_SCANCODE_RETURN]) {
            if (toucheRelache) {
                dialogue_entree_labyrinthe = 0; // On ferme le dialogue
                toucheRelache = 0;
            }
        } else {
            toucheRelache = 1;
        }
        return;
    }
    if(showInteractImpossibleObjet >0){
        if(state[SDL_SCANCODE_RETURN]){
            if(toucheRelache){
                showInteractImpossibleObjet = 0;
                toucheRelache = 0;
            }
        }else{toucheRelache=1;}
        return;
    }   
    if (dialogue_max_objet > 0) {
        if (state[SDL_SCANCODE_RETURN]) {
            if (toucheRelache) {
                dialogue_max_objet = 0;
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

    // if (dirX != 0 || dirY != 0) {
    //     Mix_Volume(2, 32); 
    // } else {
    //     Mix_Volume(2, 0);
    // }

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

    // if (dirX != 0 || dirY != 0) {
    //     Mix_Volume(2, 64); // On met le son si on bouge
    // } else {
    //     Mix_Volume(2, 0);  // On coupe le son si on arrête
    // }

    if( (dirX != 0 || dirY != 0) && papaReveil==1 && currentLevel == 10){
        player.y = (MAP_HEIGHT * TILE_SIZE)-60;
        player.x = 10 * TILE_SIZE;
    }
    
    float distance;

    showInteractPrompt = 0;
    showInteractPrompt2 = 0;
    showInteractPrompt3 = 0;

    if (IsLocationObjet(24, 0, 8, &distance, -1, -1)) {
        showInteractPrompt = 1;
    }

    else if (IsLocationObjet(24, 0, 16, &distance, -1, -1)) {
        showInteractPrompt2 = 1;
    }

    else if (IsLocationObjet(24, 0, 12, &distance, -1, -1)) {
        showInteractPrompt3 = 1;
    }

    float distance_tente;
    showInteractPromptTente = 0;
    if(IsLocationObjet(24, 0, 55, &distance_tente, -1, -1)){
        showInteractPromptTente = 1;
    }
    show_interact_prompt_statue_haut = 0;
    show_interact_prompt_statue_bas = 0;
    // --- GESTION PROMPT STATUES (NIVEAU 2) ---
    float distStatueHaut = 9999.0f;
    float distStatueBas = 9999.0f;

    if (IsLocationObjet(24, 2, 84, &distStatueHaut, 17, 6))
    {
        show_interact_prompt_statue_haut = 1;
    }
    if (IsLocationObjet(24, 2, 84, &distStatueBas, 17, 10))
    {
        show_interact_prompt_statue_bas = 1;
    }
    show_interact_prompt_eau = 0;
    show_interact_prompt_dessin = 0;
    float distance_carafe = 9999.0f;
    float distance_dessin = 9999.0f;


    if(carafeX == -1 && carafeY == -1)TrouveCoordonnees(&carafeX, &carafeY, 172, 1);
    else{TrouveCoordonnees(&carafeX, &carafeY, 72, 1);}
    
    if (IsLocationObjet(14, 1, 72, &distance_carafe , -1, -1) && interact_statue_haut == 1)
    {
        show_interact_prompt_eau = 1;
    }

    TrouveCoordonnees(&dessinX, &dessinY, 174, 9);
    if (IsLocationObjet(14, 9, 174, &distance_dessin , -1, -1) && interact_statue_bas == 1)
    {
        show_interact_prompt_dessin = 1;
    }
    

    

    // --- Calcul pour les pièces du tableau dans le labyrinthe ---
    showInteractPromptObjetTableau=0;

    // Calcul distance entre joueur et première pièce du tableau (117)
    int morceauTableau1x;
    int morceauTableau1y;
    TrouveCoordonnees(&morceauTableau1x, &morceauTableau1y, 117, 5);

    float distance_morceauTableau1;
    if(IsLocationObjet(16, 5, 117, &distance_morceauTableau1, -1, -1)) showInteractPromptObjetTableau = 1;

    // Calcul distance entre joueur et deuxième pièce du tableau (118)
    int morceauTableau2x;
    int morceauTableau2y;
    TrouveCoordonnees(&morceauTableau2x, &morceauTableau2y, 118, 6);

    float distance_morceauTableau2;
    if(IsLocationObjet(16, 6, 118, &distance_morceauTableau2, -1, -1)) showInteractPromptObjetTableau = 1;

    // Calcul distance entre joueur et troisième pièce du tableau (115)
    int morceauTableau3x;
    int morceauTableau3y;
    TrouveCoordonnees(&morceauTableau3x, &morceauTableau3y, 115, 7);

    float distance_morceauTableau3;
    if(IsLocationObjet(16, 7, 115, &distance_morceauTableau3, -1, -1)) showInteractPromptObjetTableau = 1;

    // Calcul distance entre joueur et quatrième pièce du tableau (116)
    int morceauTableau4x;
    int morceauTableau4y;
    TrouveCoordonnees(&morceauTableau4x, &morceauTableau4y, 116, 8);

    float distance_morceauTableau4;
    if(IsLocationObjet(16, 8, 116, &distance_morceauTableau4, -1, -1)) showInteractPromptObjetTableau = 1;

    // Ouverture de la salle du niveau 3
    if(cpt_piece_tableau == 4){
        maps_patern[2][7][0] = 138;
        maps_patern[2][8][0] = 138; 
    }
    // Calcul distance entre joueur et le tableau

    showInteractTableau=0;
    float distance_Tableau;
    if( (IsLocationObjet(24, 2, 113, &distance_Tableau, -1, -1) || IsLocationObjet(24, 2, 109, &distance_Tableau, -1, -1) ) && cpt_piece_tableau != 4){
        showInteractTableau = 1;
    }

    // Transition tente à la chambre
    if(IsLocationLeft(6, 10, 9, 6*TILE_SIZE-8)){
        player.x = 15 * TILE_SIZE;
        currentLevel = 0;
    }
    if (interact_statue_haut == 1 && maps[1][carafeY][carafeX] == 172)
    {
        maps[1][carafeY][carafeX] = 72;
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

            if (currentLevel == 1 && distance_carafe < 14 && has_drawing == 0) {
                maps[1][carafeY][carafeX] = 173;
                has_water = 1;
            }

            if (currentLevel == 2 && distStatueHaut < 24 && has_water == 0) {
                dialogue_statue_haut = 1;   
            }
            else if (currentLevel == 2 && distStatueHaut < 24 && has_water == 1)
            {
                maps[2][5][17] = 86;
                statue_has_water = 1;
                has_water = 0;
            }
            

            if (currentLevel == 2 && distStatueBas < 24 && has_drawing == 0) {
                 dialogue_statue_bas = 1;
                 toucheE_Relache = 0;
            }
            if (currentLevel == 9 && distance_dessin < 16 && has_drawing == 0 && has_water == 0 && interact_statue_bas == 1) {
                maps[9][dessinY][dessinX] = 1;
                has_drawing = 1;
            }
            if (currentLevel == 9 && distance_dessin < 16 && has_drawing == 0 && has_water == 1) {
                dialogue_max_objet = 1;
            }
            if (currentLevel == 1 && distance_carafe < 14 && has_drawing == 1 && has_water == 0) {
                dialogue_max_objet = 1;
            }
            else if (currentLevel == 2 && distStatueBas < 24 && has_drawing == 1)
            {
                maps[2][9][17] = 86;
                statue_has_drawing = 1;
                has_drawing = 0;
            }

            else if(distance_tente <= 24 && currentLevel == 0 && maps[0][6][16] == 55){
                currentLevel = 9;
                player.x = 7 * TILE_SIZE;
                player.y = 7 * TILE_SIZE;
            }

            else if(distance_morceauTableau1 <= 16 && currentLevel == 5 && maps[5][morceauTableau1y][morceauTableau1x] == 117){
                if(whichTableauPiece != 0){
                    showInteractImpossibleObjet = 1;
                }
                else{
                    whichTableauPiece = 1;
                    maps[5][1][18] = 82;
                }
                
            }
            else if(distance_morceauTableau2 <= 16 && currentLevel == 6 && maps[6][morceauTableau2y][morceauTableau2x] == 118){
                if(whichTableauPiece != 0){
                    showInteractImpossibleObjet = 1;
                }
                else{
                    whichTableauPiece = 2;
                    maps[6][3][13] = 82;
                }
            }
            else if(distance_morceauTableau3 <= 16 && currentLevel == 7 && maps[7][morceauTableau3y][morceauTableau3x] == 115){
                if(whichTableauPiece != 0){
                    showInteractImpossibleObjet = 1;
                }
                else{
                    whichTableauPiece = 3;
                    maps[7][morceauTableau3y][morceauTableau3x] = 82;
                }
            }
            else if(distance_morceauTableau4 <= 16 && currentLevel == 8 && maps[8][morceauTableau4y][morceauTableau4x] == 116){
                if(whichTableauPiece != 0){
                    showInteractImpossibleObjet = 1;
                }
                else{
                    whichTableauPiece = 4;
                    maps[8][morceauTableau4y][morceauTableau4x] = 82;
                }
            }
            else if(distance_Tableau <= 24 && currentLevel == 2 && whichTableauPiece != 0){
                if(whichTableauPiece == 1){
                    maps[2][1][5] = 109;
                }
                else if(whichTableauPiece == 2){
                    maps[2][1][6] = 110;
                }
                else if(whichTableauPiece == 3){
                    maps[2][2][5] = 107;
                }
                else if(whichTableauPiece == 4){
                    maps[2][2][6] = 108;
                }
                whichTableauPiece = 0;
                cpt_piece_tableau++;
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
            player.y = 5;
        }
    }
    if (IsLocationUp(8, 13, 0, 10) && hasDoudou == 0){
        dialogueStep_sortie1 = 1;

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
    
    if (IsLocationLeft(5, 9, 2, 5)) {
            currentLevel = 3; 
            player.x = (MAP_WIDTH * TILE_SIZE) - 20;
    }

    else if (IsLocationRight(5, 9, 3, 20)) {
        currentLevel = 2;  
        player.x = 5;     
        
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
    if(IsLocationRight(6, 10, 2, 20) && (statue_has_drawing == 1 && statue_has_water == 1)){
        currentLevel = 5;
        player.x = 5;
        SpawnFantomeRandom(); // <--- NOUVEAU
    }
    if(IsLocationRight(6, 10, 2, 20) && (statue_has_drawing == 0 || statue_has_water == 0)){

        dialogue_entree_labyrinthe = 1;

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
    // currentLevel = 10;
    // --- GESTION COLLISION JOUEUR / FANTOME (GAME OVER / RESET) ---
    if (currentLevel >= 5 && currentLevel <=8) {
        
        // 1. On définit la hitbox d'attaque du fantôme
        // On veut qu'il attrape plus haut (pour la perspective) et un peu plus large
        float killZoneHaut = fantome.y - 12; 
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

            if(whichTableauPiece != 0){
                if (whichTableauPiece == 1) maps[5][1][18] = 117;
                
                if (whichTableauPiece == 2) maps[6][3][13] = 118;
            
                if (whichTableauPiece == 3) maps[7][13][17] = 115; 

                if (whichTableauPiece == 4) maps[8][3][1] = 116;

                whichTableauPiece = 0;
            }
        }
    }

    if (IsLocationUp(8, 13, 4, 10))
    {
        currentLevel = 10;
        player.y = (MAP_HEIGHT * TILE_SIZE) - 20;
    }
    else if(IsLocationDown(8, 13, 10, 20)){
        currentLevel = 4;
        player.y = 10;
    }
    hasDoudou = 1;
    // if(currentLevel == 10)GestionPapa();



    // printf("lvl: %d \n", currentLevel);
}

void GestionPapa() {
    static Uint32 dernierTemps = 0; 
    Uint32 tempsActuel = SDL_GetTicks();

    if (papaReveil == 0) {
        if (tempsActuel - dernierTemps > 5000) {
            papaReveil = 1;          
            dernierTemps = tempsActuel; 
        }
    } 
    else {
        if (tempsActuel - dernierTemps > 2000) {
            papaReveil = 0;          
            dernierTemps = tempsActuel; 
        }
    }
}

int IsLocationObjet(int rayon, int CurrLvl, int indexTuile, float *distance, int x, int y){
    if(x == -1 || y == -1){
        TrouveCoordonnees(&x, &y, indexTuile, CurrLvl);
    }

    if (x == -1 || y == -1) {
        *distance = 9999.0f; 
        return 0;
    }

    float targetX = x * TILE_SIZE + 8;
    float targetY = y * TILE_SIZE + 8;

    switch (indexTuile) {
        case 8:   
        case 12:  
        case 16: 
            targetX += 8; 
            targetY += 16; 
            break;
        case 55:
            targetY += 8;
            break;
        case 113:
        case 109:
            targetY += 8;
            targetX += 8;
    }

    // 3. Calcul de la distance avec la position CORRIGÉE
    float dx = (player.x + player.w / 2) - targetX;
    float dy = (player.y + player.h / 2) - targetY;
    *distance = sqrt(dx*dx + dy*dy);

    return (*distance <= rayon && currentLevel == CurrLvl);
}

void TrouveCoordonnees(int *nvx, int *nvy, int indexObjet, int CurrLvl){
    for (int y = 0; y < MAP_HEIGHT; ++y)
    {
        for (int x = 0; x < MAP_WIDTH; ++x)
        {
            if(maps[CurrLvl][y][x] == indexObjet){
                *nvx = x;
                *nvy = y;
                return;
            }
        }
    }
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
             if (maps[currentLevel][ly][lx] == 21 || (maps[currentLevel][ly][lx] >= 75 && maps[currentLevel][ly][lx] <= 76) || maps[currentLevel][ly][lx] == 85 || maps[currentLevel][ly][lx] == 86 || maps[currentLevel][ly][lx] == 148) { // Si c'est une lampe
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

    // --- EFFET SOMMEIL / REVEIL PARENTS ---
    // Uniquement dans la chambre des parents (Index 10)
    if (currentLevel == 10) { 
        
        // Coordonnées approximatives du lit des parents (à ajuster selon ta map)
        // Disons que le lit est vers le milieu de la pièce
        int litX = 15 * TILE_SIZE; 
        int litY = 3 * TILE_SIZE;  

        // Calcul de distance pour savoir si on réveille les parents
        float dx = player.x - litX;
        float dy = player.y - litY;
        float distParents = sqrt(dx*dx + dy*dy);

        // Si le joueur est proche (< 50 pixels) -> REVEIL (!)
        if (distParents < 50 && estEclaire(15, 3, rayon) && papaReveil) {
            SDL_Color cRouge = {255, 0, 0, 255};
            // On utilise la grande police pour un gros "!"
            SDL_Surface *sBang = TTF_RenderText_Solid(font, "!", cRouge); 
            if (sBang) {
                SDL_Texture *tBang = SDL_CreateTextureFromSurface(renderer, sBang);
                // Le "!" s'affiche juste au-dessus du lit
                SDL_Rect rBang = { litX, litY - 20, sBang->w, sBang->h }; 
                SDL_RenderCopy(renderer, tBang, NULL, &rBang);
                SDL_FreeSurface(sBang);
                SDL_DestroyTexture(tBang);
            }
        } 
        // Sinon -> DORT (Zzz...)
        else if(estEclaire(15, 3, rayon) && !papaReveil) {
            // Animation : Le texte monte de 0 à 10 pixels en boucle
            // (SDL_GetTicks() / 200) ralentit le temps, % 10 crée la boucle
            int offsetAnim = (SDL_GetTicks() / 200) % 10; 

            SDL_Color cBlanc = {200, 200, 255, 255}; // Bleu très clair
            SDL_Surface *sZzz = TTF_RenderText_Solid(fontMini, "Zzz...", cBlanc);
            if (sZzz) {
                SDL_Texture *tZzz = SDL_CreateTextureFromSurface(renderer, sZzz);
                // On dessine le texte qui flotte (Y - offsetAnim)
                SDL_Rect rZzz = { litX, litY - 10 - offsetAnim, sZzz->w, sZzz->h }; 
                SDL_RenderCopy(renderer, tZzz, NULL, &rZzz);
                SDL_FreeSurface(sZzz);
                SDL_DestroyTexture(tZzz);
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
    if (dialogue_statue_haut > 0) {
        show_interact_prompt_statue_haut = 0;
        char *texteAffiche = "";
        if (dialogue_statue_haut == 1) texteAffiche = "Cette statue tient une coupe vide,";
        if (dialogue_statue_haut == 2) texteAffiche = "elle doit avoir soif...";

        DrawTexte(texteAffiche, renderer, font, 20, 180 ,280, 50);
    }
    if (dialogue_statue_bas > 0) {
        show_interact_prompt_statue_bas = 0;
        char *texteAffiche = "";
        if (dialogue_statue_bas == 1) texteAffiche = "Son visage est tordu par la haine.";
        if (dialogue_statue_bas == 2) texteAffiche = "Un sourir ne ferait pas de mal...";

        DrawTexte(texteAffiche, renderer, font, 20, 180 ,280, 50);
    }
    if (dialogue_entree_labyrinthe > 0) {
        char *texteAffiche = "";
        if (dialogue_entree_labyrinthe == 1) texteAffiche = "Les statues bloquent le passage...";

        DrawTexte(texteAffiche, renderer, font, 20, 180 ,280, 50);
    }
    if (dialogue_max_objet > 0) {
        show_interact_prompt_dessin = 0;
        show_interact_prompt_eau = 0;
        char *texteAffiche = "";
        if (dialogue_max_objet == 1) texteAffiche = "Je n'ai que deux mains...";

        DrawTexte(texteAffiche, renderer, font, 20, 180 ,280, 50);
    }

    

    SDL_Rect srcPlayer = { 112, 0, 16, 16 };
    SDL_Rect destPlayer = { (int)player.x - 2, (int)player.y - 2, 16, 16 };
    SDL_RenderCopy(renderer, tilesetTexture, &srcPlayer, &destPlayer);  


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
    if(showInteractImpossibleObjet == 1){
        char *texteAffiche = "j'ai deja une piece";
        DrawTexte(texteAffiche, renderer, font, 20, 180 ,280, 50);
    }

    if(showInteractPromptObjetTableau == 1){
        SDL_Color cBlanc = {255, 255, 255, 255};
        SDL_Surface *sText = TTF_RenderText_Solid(fontMini, "[E] Recuperer", cBlanc);
        
        if (sText) DrawInteractions(renderer, sText);
    }
    
    if(showInteractTableau == 1){
        char Prompt[100] = "";
        if(cpt_piece_tableau == 0){
            sprintf(Prompt, "[E] Ajouter la %d ere piece", cpt_piece_tableau+1);
        }
        else{
            sprintf(Prompt, "[E] Ajouter la %d eme piece", cpt_piece_tableau+1);
        }
        SDL_Color cBlanc = {255, 255, 255, 255};
        SDL_Surface *sText = TTF_RenderText_Solid(fontMini, Prompt, cBlanc);
        
        if (sText) DrawInteractions(renderer, sText);
    }
    if (show_interact_prompt_statue_haut == 1 && has_water == 0 && statue_has_water == 0) {
        SDL_Color cBlanc = {255, 255, 255, 255};
        SDL_Surface *sText = TTF_RenderText_Solid(fontMini, "[E] Interagir", cBlanc);
        if (sText) DrawInteractions(renderer, sText);
    }
    if (show_interact_prompt_statue_haut == 1 && has_water == 1 && statue_has_water == 0){
        SDL_Color cBlanc = {255, 255, 255, 255};
        SDL_Surface *sText = TTF_RenderText_Solid(fontMini, "[E] Donner", cBlanc);
        if (sText) DrawInteractions(renderer, sText);
    }
    if (show_interact_prompt_statue_bas == 1 && has_drawing == 0 && statue_has_drawing == 0) {
        SDL_Color cBlanc = {255, 255, 255, 255};
        SDL_Surface *sText = TTF_RenderText_Solid(fontMini, "[E] Interagir", cBlanc);
        if (sText) DrawInteractions(renderer, sText);
    }
    if (show_interact_prompt_statue_bas == 1 && has_drawing == 1 && statue_has_drawing == 0) {
        SDL_Color cBlanc = {255, 255, 255, 255};
        SDL_Surface *sText = TTF_RenderText_Solid(fontMini, "[E] Donner", cBlanc);
        if (sText) DrawInteractions(renderer, sText);
    }
     if (show_interact_prompt_eau == 1)            
    {
        SDL_Color cBlanc = {255, 255, 255, 255};
        SDL_Surface *sText = TTF_RenderText_Solid(fontMini, "[E] Recuperer", cBlanc);
        
        if (sText) DrawInteractions(renderer, sText);
    }
    if(show_interact_prompt_dessin == 1){
        SDL_Color cBlanc = {255, 255, 255, 255};
        SDL_Surface *sText = TTF_RenderText_Solid(fontMini, "[E] Recuperer", cBlanc);
        
        if (sText) DrawInteractions(renderer, sText);
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