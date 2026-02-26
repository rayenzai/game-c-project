#include "game.h"
#include "game_reveille.h"
#include "map_reveille.h"

// int currentLevel = 0;

static int playerDir = 0;       // 0=Bas, 1=Gauche, 2=Droite, 3=Haut
static int isPlayerMoving = 0;  // 0=Immobile, 1=Marche

static SDL_Texture *tilesetTexture = NULL;

/*------- CHAMBRE ---------
444,445 = sol
446 = mur en brique 
447,448 = cubes seuls
449 = affiche lapin 
450 = livre 
451 = lampe 
452,453,454 = bas du tapis 
455,456,457 = haut tapis
458,459 = tapis rond
460,461 = haut lit 
462,463 = bas lit
464 = cars
465 = batman 
466,467 = petit train


*/

static int toucheE_Relache = 1;

int showInteractPromptTente2 = 0;


int maps_reveille[NB_LEVELS][MAP_HEIGHT][MAP_WIDTH] = {
	{      //carte 1 (chambre)
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2}, 
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2}, 
        {2, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 2},
        {2, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 2},
        {2, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 2},
        {2, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 2},
        {2, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 55, 56, 57, 2},
        {2, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 52, 53, 54, 2},
        {2, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 2},
        {2, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 2},
        {2, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 2},
        {2, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 2},
        {2, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 2}, 
        {2, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}  
    },
    // Couloir niveau 1
    {
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0,  0, 2, 2, 2, 2, 2, 2, 2, 2}, // Haut (Suite)
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0,  0, 2, 2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0,  0, 2, 2, 2, 2, 2, 2, 2, 2}, // Toits cotés
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0,  0, 2, 2, 2, 2, 2, 2, 2, 2}, // Murs cotés
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0,  0, 2, 2, 2, 2, 2, 2, 2, 2}, // Vide (Sol)
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0,  0, 2, 2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0,  0, 2, 2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0,  0, 2, 2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0,  0, 2, 2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0,  0, 2, 2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0,  0, 2, 2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0,  0, 2, 2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0,  0, 2, 2, 2, 2, 2, 2, 2, 2}, // Bas (Entrée)
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0,  0, 2, 2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0,  0, 2, 2, 2, 2, 2, 2, 2, 2}
    }, 
    // CARTE 3 : HALL (Niveau 2)
    {
        {   2,    2,    2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,  2,    2 },
        {   2,    2,    2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,  2,    2 },
        {   2,    2,    2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,  2,    2 },
        {   2,    2,    2, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138,  2,    2 },
        {   2,    2,    2, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138,  2,    2 },
        {   2,    2,    2, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138,  2,    2 },
        {   2,    2,    2, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138,  2,    2 },
        { 138,  138,  138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138,  138,138 },
        { 138,  138,  138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138,  138,138 },
        {   2,    2,    2, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138,  2,    2 },
        {   2,    2,    2, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138,  2,    2 },
        {   2,    2,    2, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138,  2,    2 },
        {   2,    2,    2, 138,  138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138,  2,    2 },
        {   2,    2,    2, 138,  138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138, 138,  2,    2 },
        {   2,    2,    2,   2,   2,   2,   2,   2, 138, 138, 138, 138,   2,   2,   2,   2,   2,   2,  2,    2 }
     },
    // CARTE 4 CUISINE (Niveau 3)
    {       
        {2, 2,   2,   2,   2,   2,   2,   2, 155, 155, 155, 155,   2,   2,   2,   2,   2,   2,   2,   2}, // Trou en haut
        {2, 2,   2,   2,   2,   2,   2,   2, 155, 155, 155, 155,   2,   2,   2,   2,   2,   2,   2,   2},
        {2, 2, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155,   2,   2},
        {2, 2, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155,   2,   2},
        {2, 2, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155,   2,   2},
        {2, 2, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155,   2,   2},
        {2, 2, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155,   2,   2},
        {2, 2, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155},
        {2, 2, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155},
        {2, 2, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155,   2,   2},
        {2, 2, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155,   2,   2},
        {2, 2, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155,   2,   2},
        {2, 2, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155,   2,   2}, // Bas fermé
        {2, 2, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155,   2,   2},
        {2, 2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2}
    },
    // CARTE 5 (Niveau 4 - dernière du bloc précédent)
    {    
        {2, 2, 2, 2, 2, 2, 2, 2,  0,  0,  0,  0,  2, 2, 2, 2, 2, 2, 2, 2}, // Trou en haut
        {2, 2, 2, 2, 2, 2, 2, 2,  0,  0,  0,  0,  2, 2, 2, 2, 2, 2, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0},
        {2, 2, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0},
        {2, 2, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0},
        {2, 2, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 2, 2}, // Bas fermé
        {2, 2, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2,  0,  0,  0,  0,  2, 2, 2, 2, 2, 2, 2, 2}
    },
       {      // Jardin Niveau 5
        {2, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83}, 
        {2, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83}, 
        {2, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83},
        {2, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83},
        {2, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83},
        {2, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83},
        {2, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83},
        {82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83},
        {82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83},
        {2, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83},
        {2, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83},
        {2, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83},
        {2, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83}, 
        {2, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 83},
        {2, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83}  
    },
    // Chambre des parents (index 6)
    {    
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}, // Trou en haut
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
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
    // Salon (index 7)
    {    
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}, // Trou en haut
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2}, // Bas fermé
        {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}
    },
    // TENTE (index 8)
    {
        {50, 50, 50, 50, 50,  50,  50,  50,  50,  50,  50,  50,  50,  50, 50, 50, 50, 50, 50, 50}, // Marge Haut
        {50, 50, 50, 50, 50,  50,  50,  50,  50,  50,  50,  50,  50,  50, 50, 50, 50, 50, 50, 50},
        {50, 50, 50, 50, 50,  50,  50,  50,  50,  50,  50,  50,  50,  50, 50, 50, 50, 50, 50, 50},
        {50, 50, 50, 50, 50, 151, 152, 151, 152, 151, 152, 151, 152, 151, 50, 50, 50, 50, 50, 50}, // Début Rectangle
        {50, 50, 50, 50, 50, 366,   1,   0,   1,   0,  58,  41,   1, 364, 50, 50, 50, 50, 50, 50},
        {50, 50, 50, 50, 50, 366,  44,   0,   1,   0,   1,   0, 147, 364, 50, 50, 50, 50, 50, 50},
        {50, 50, 50, 50, 50,   0,   1,   0,   1,   0,   1,   0,   1, 364, 50, 50, 50, 50, 50, 50},
        {50, 50, 50, 50, 50,   0,   1,   0, 148,  59,  60,   0,   1, 364, 50, 50, 50, 50, 50, 50},
        {50, 50, 50, 50, 50,   0,   1,   0,   1,   0, 149,  45, 174, 364, 50, 50, 50, 50, 50, 50}, 
        {50, 50, 50, 50, 50, 366,  58,   0,   1,   0,   1,  40,   1, 364, 50, 50, 50, 50, 50, 50},
        {50, 50, 50, 50, 50, 366,   1,   0,   1,   0,   1,   0,   1, 364, 50, 50, 50, 50, 50, 50},
        {50, 50, 50, 50, 50, 365, 365, 365, 365, 365, 365, 365, 365, 364, 50, 50, 50, 50, 50, 50}, // Fin Rectangle
        {50, 50, 50, 50, 50,  50,  50,  50,  50,  50,  50,  50,  50,  50, 50, 50, 50, 50, 50, 50},          // Marge Bas
        {50, 50, 50, 50, 50,  50,  50,  50,  50,  50,  50,  50,  50,  50, 50, 50, 50, 50, 50, 50},
        {50, 50, 50, 50, 50,  50,  50,  50,  50,  50,  50,  50,  50,  50, 50, 50, 50, 50, 50, 50}},

};

// int TuilesNotSpecial[] = {0, 1, 2};
// int tailleTuilesNotSpecial = (int)sizeof(TuilesNotSpecial) / (int)sizeof(TuilesNotSpecial[0]);


int InitGameStepByStepReveille(SDL_Renderer *renderer){
	player.x = 80;
    player.y = 50; 
    player.w = 12; 
    player.h = 12;

    SDL_Surface *surface = SDL_LoadBMP("assets/tuille_into.bmp");
    if (surface) {
        SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 0, 255));
        tilesetTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }	
    return 0;
}

int IsWallReveille(float x, float y){
	int caseX = (int)x / TILE_SIZE;
    int caseY = (int)y / TILE_SIZE;

	// int type = maps_reveille[currentLevel][caseY][caseX];
    int type_pattern = maps_patern_reveille[currentLevel][caseY][caseX];

    if(type_pattern == 2 || type_pattern == 83)return 1;
    return 0;
}

void UpdateGameReveille(void){
	const Uint8 *state = SDL_GetKeyboardState(NULL);

	float dirX = 0;
    float dirY = 0;

    if (state[SDL_SCANCODE_UP])
        dirY -= 1;
    if (state[SDL_SCANCODE_DOWN])
        dirY += 1;
    if (state[SDL_SCANCODE_LEFT])
        dirX -= 1;
    if (state[SDL_SCANCODE_RIGHT])
        dirX += 1;

    isPlayerMoving = 0;
    if (dirX != 0 || dirY != 0) {
        isPlayerMoving = 1;
        
        if (dirY > 0)      playerDir = 0; // Vers le Bas
        else if (dirY < 0) playerDir = 3; // Vers le Haut
        else if (dirX < 0) playerDir = 1; // Vers la Gauche
        else if (dirX > 0) playerDir = 2; // Vers la Droite
    }

    if (dirX != 0 && dirY != 0)
    {
        // On multiplie par 0.707 (environ 1/racine(2)) pour ralentir
        dirX *= 0.7071f;
        dirY *= 0.7071f;
    }

    float nextX = player.x + (dirX * PLAYER_SPEED);
    float nextY = player.y + (dirY * PLAYER_SPEED);
    // Collision X
    int touchWallX = 0;
    if (IsWallReveille(nextX, player.y))
        touchWallX = 1;
    if (IsWallReveille(nextX + player.w, player.y))
        touchWallX = 1;
    if (IsWallReveille(nextX, player.y + player.h))
        touchWallX = 1;
    if (IsWallReveille(nextX + player.w, player.y + player.h))
        touchWallX = 1;

    if (!touchWallX)
        player.x = nextX;

    // Collision Y
    int touchWallY = 0;
    if (IsWallReveille(player.x, nextY))
        touchWallY = 1;
    if (IsWallReveille(player.x + player.w, nextY))
        touchWallY = 1;
    if (IsWallReveille(player.x, nextY + player.h))
        touchWallY = 1;
    if (IsWallReveille(player.x + player.w, nextY + player.h))
        touchWallY = 1;

    if (!touchWallY)
        player.y = nextY;

    float distance_tente;
    showInteractPromptTente2 = 0;
    if (IsLocationObjet(24, 0, 55, &distance_tente, -1, -1))
    {
        showInteractPromptTente2 = 1;
    }

    if(state[SDL_SCANCODE_E]){
    	if(toucheE_Relache){
    		if (distance_tente <= 24 && currentLevel == 0 && maps[0][6][16] == 55)
            {
                currentLevel = 8;
                player.x = 7 * TILE_SIZE;
                player.y = 7 * TILE_SIZE;
            }
    		toucheE_Relache = 0;
    	}else{
    		toucheE_Relache = 1;
    	}
    }

    // Changement tente <->chambre 
    if (IsLocationLeft(6, 10, 8, 6 * TILE_SIZE - 8))
    {
        player.x = 15 * TILE_SIZE;
        currentLevel = 0;
    }

    // Changement chambre <-> couloir
    if (IsLocationUp(8, 13, 0, 5))
    {
        currentLevel = 1;
        player.y = (MAP_HEIGHT * TILE_SIZE) - 20;
    }
	else if (IsLocationDown(8, 13, 1, 20))
    {
        currentLevel = 0; // On retourne à la CHAMBRE
        player.y = 10;    // On apparaît tout en HAUT de la chambre
    }

    // Changement couloir <-> Hall d'entrée
    if (IsLocationUp(8, 13, 1, 5))
    {
        currentLevel = 2;
        player.y = (MAP_HEIGHT * TILE_SIZE) - 20;
    }

    else if (IsLocationDown(8, 13, 2, 20))
    {
        currentLevel = 1;
        player.y = 10;
    }

    // Changement Hall d'entrée <-> cuisine
    if (IsLocationLeft(5, 9, 2, 5))
    {
        currentLevel = 3;
        player.x = (MAP_WIDTH * TILE_SIZE) - 20;
    }

    else if (IsLocationRight(5, 9, 3, 20))
    {
        currentLevel = 2;
        player.x = 5;
    }

    // Changement Hall d'entrée <-> jardin
    if (IsLocationRight(6, 10, 2, 20))
    {
        currentLevel = 5;
        player.x = 5;
    }
	else if (IsLocationLeft(6, 10, 5, 5))
    {
        currentLevel = 2;
        player.x = (MAP_WIDTH * TILE_SIZE) - 20;
    }

    // Changement Cuisine <-> salle à manger
    if (IsLocationUp(8, 13, 3, 10))
    {
        currentLevel =  4;
        player.y = (MAP_HEIGHT * TILE_SIZE) - 20;
    }
    else if (IsLocationDown(8, 13, 4, 20))
    {
        currentLevel = 3;
        player.y = 10;
    }

    // Changement salle à manger <-> chambre des parents
    if (IsLocationUp(8, 13, 4, 10))
    {
        currentLevel = 6;
        player.y = (MAP_HEIGHT * TILE_SIZE) - 20;
    }
    else if (IsLocationDown(8, 13, 6, 20))
    {
        currentLevel = 4;
        player.y = 10;
    }

    // Changement salle à manger <-> salon
    if (IsLocationRight(6, 10, 4, 20))
    {
        currentLevel = 7;
        player.x = 5;
    }
	else if (IsLocationLeft(6, 10, 7, 5))
    {
        currentLevel = 4;
        player.x = (MAP_WIDTH * TILE_SIZE) - 20;
    }


}

static inline int IsTuileDessus(int index) {
    return (index == 78);
}

void DrawGameReveille(SDL_Renderer *renderer, TTF_Font *font, TTF_Font *fontMini)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (tilesetTexture)
    {
        for (int y = 0; y < MAP_HEIGHT; y++)
        {
            for (int x = 0; x < MAP_WIDTH; x++)
            {
                int lum = 255; 
                
                int type_pat = maps_patern_reveille[currentLevel][y][x];
                DrawTuiles(x, y, type_pat, renderer, lum); 

                int type_maps = maps_reveille[currentLevel][y][x];
                if (!IsTuileDessus(type_maps) && IsTuileSpecial(type_maps))
                {
                    if (type_maps >= 219 && type_maps <= 222) continue; 
                    if (type_maps >= 187 && type_maps <= 190) continue; 
                    DrawTuiles(x, y, type_maps, renderer, lum); 
                }
            }
        }

        int indexJoueur = 7; 
        if (isPlayerMoving) {
            int etape = (SDL_GetTicks() / 120) % 4; 
            if (playerDir == 0) {
                if (etape == 0) indexJoueur = 356; 
                else if (etape == 1 || etape == 3) { 
                    if ((SDL_GetTicks() % 3000) < 150) indexJoueur = 355; 
                    else indexJoueur = 7; 
                } 
                else if (etape == 2) indexJoueur = 357; 
            }
            else if (playerDir == 1) {
                if (etape == 0)      indexJoueur = 345; 
                else if (etape == 1) indexJoueur = 347; 
                else if (etape == 2) indexJoueur = 350; 
                else if (etape == 3) indexJoueur = 347; 
            }
            else if (playerDir == 2) {
                if (etape == 0)      indexJoueur = 344; 
                else if (etape == 1) indexJoueur = 346; 
                else if (etape == 2) indexJoueur = 349; 
                else if (etape == 3) indexJoueur = 346; 
            }
            else if (playerDir == 3) {
                if (etape == 0)      indexJoueur = 352; 
                else if (etape == 1) indexJoueur = 348; 
                else if (etape == 2) indexJoueur = 351; 
                else if (etape == 3) indexJoueur = 348; 
            }
        } 
        else {
            int idleTime = SDL_GetTicks() % 2000;
            if (playerDir == 0) {
                if (idleTime < 1200) {
                    if ((SDL_GetTicks() % 3000) < 150) indexJoueur = 355; 
                    else indexJoueur = 7;   
                } 
                else if (idleTime < 1600) indexJoueur = 358; 
                else                      indexJoueur = 7;  
            }
            else if (playerDir == 1) {
                if (idleTime < 1200)      indexJoueur = 347; 
                else if (idleTime < 1600) indexJoueur = 361; 
                else                      indexJoueur = 363; 
            }
            else if (playerDir == 2) {
                if (idleTime < 1200)      indexJoueur = 346; 
                else if (idleTime < 1600) indexJoueur = 360; 
                else                      indexJoueur = 362; 
            }
            else if (playerDir == 3) {
                if (idleTime < 1200)      indexJoueur = 348; 
                else if (idleTime < 1600) indexJoueur = 359; 
                else                      indexJoueur = 348; 
            }
        }

        int dx = (int)roundf(player.x) - 2;
        int dy = (int)roundf(player.y) - 2;
        SDL_Rect srcPlayer = { indexJoueur * 16, 0, 16, 16 };
        SDL_Rect destPlayer = { dx, dy, 16, 16 };
        SDL_RenderCopy(renderer, tilesetTexture, &srcPlayer, &destPlayer);

        for (int y = 0; y < MAP_HEIGHT; y++)
        {
            for (int x = 0; x < MAP_WIDTH; x++)
            {
                int type_maps = maps_reveille[currentLevel][y][x];
                if (IsTuileDessus(type_maps))
                {
                    DrawTuiles(x, y, type_maps, renderer, 255); 
                }
            }
        }

    }
    if (showInteractPromptTente2 == 1)
    {
        SDL_Color cBlanc = {255, 255, 255, 255};
        SDL_Surface *sText = TTF_RenderText_Solid(fontMini, "[E] Entrer", cBlanc);

        if (sText)
            DrawInteractions(renderer, sText);
    }
}