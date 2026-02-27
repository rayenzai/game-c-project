#include "game.h"
#include "game_reveille.h"
#include "map_reveille.h"

// int currentLevel = 0;

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
468 = avion
469 = dino
470(haut),471 = commode pres du lit
472,473 = cubes
474,475,476,477 = bureau de travail
478 = haut de la chaise
479,480,481 = bas de la tente
482,483,484 = haut de la tente
485 = coussin
486,487 = coussins
501 = panier de basket
502 = ballon de basket
503(bas),504 = commode coin 
597,598,599 = bas tapis de course 
600, 601,602 = haut tapis de course
603 = legos jaunes
604 = carre marron secours (table)
606 ???
616,617 ???

------ TENTE -------
505,506,507,508,509 = bas tapis 
510,511,512,513,514 = mid v1
515,516,517,518,519 = mid v2
520,521,522,523,524 = haut tapis 
557 = duolingo
558 = lampe éteinte
559 = coockie
560 = mur 

------ COULOIR ------
488,489 = bas tapis 
490,491 = mid tapis
492,493 = fin tapis
494(bas),495 = commode avec broc
496 = vase
497,498 = lumiere couloir a refaire 
499, 500= limiere couloir a refaire v2
607 = bougie

------ HALL D'ENTREE ------
525,526 = bas tableau
527,528 = haut tableau
529,530,531,532,533,534 = bas tapis
535,536,537,538,539,540 = haut tapis
541 = tableau 
542(haut),543 = commode de coté
544(g),545 = haut de la table 
546,547 = bas de la table
548 = sol
549,550 = bas mirroir
551,552 = haut mirroir
553,554 = pot de la plante
555,556 = haut de la plante
608(gauche), 609(droite) 610(haut g), 611 = fauteuil de coté
612(bas), 613,614(haut g),615  = fauteuil de face

------ CUISINE -------
561 = sol 
562-568 = table
569,570 = lavabo 
571,572 = plaque cuisson
573 = bloc seul
574 = pain 
575 = tirroir mur gauche
576,577,578 = bas table
579(haut),580 = chaise
581,582 = bas du four
583,584 = haut du four
585(bas g), 586, 587(haut d)= table bordel
588 = petite plante 
589 = grande plante
618(bas),619 = decors meuble de droite
620,621 = sacs 
622 = poele

------ SALLON -------
590,591 = haut cheminée 
592,593,594,595 = tableau 
596 = lampe
605 = commode livres
623 = haut de la lampe 2
624 = bas de la lampe 2
625(bas g),626(bas d),627,628 = radio 
629(haut),630, 631= haut billard
632,633,634 = bas billard
635,636,637 = pieds billard
638(bas),639(mid),640 = horloge vecna

------ JARDIN ---------
641 = terre 
642 = plantes

*/

static int playerDir = 0;       // 0=Bas, 1=Gauche, 2=Droite, 3=Haut
static int isPlayerMoving = 0;  // 0=Immobile, 1=Marche

static SDL_Texture *tilesetTexture = NULL;

static int toucheE_Relache = 1;

int showInteractPromptTente2 = 0;

int maps_reveille[NB_LEVELS][MAP_HEIGHT][MAP_WIDTH] = {
	{      //carte 1 (chambre)
        {2,  2,  2,  2,  2,  2,  2,  2,  0,  0,  0,  0, 2,  2,449,  2,  8,  9,  2, 2}, // Trou en haut
        {2,  2,  2,464,465,  2,  2,  2,  0,  0,  0,  0, 2,  2,469,  2, 10, 11,  2, 2},
        {2,  1,  0,460,461,451,  0,  1,  0,  1,  0,  0, 0,230,  0,  1,  0,  1,  0, 2},
        {2,  1,  0,462,463,  1,  0,  1,  0,  1, 447,  1, 0,  1,  0,  1,  0,603,  0, 2},
        {2,  1,458,459,  0,  1,  0,  1,  0,  1,  0,  1, 0,  1,  0,  1,  0,  1,  0, 2},
        {2,  1,  0,450,  0,  1,  0,  1,600,601,602,  1, 0,  1,  0,  1,  0,  1,  0, 2},
        {2,470,  0,  1,  0,  1,  0,  1,597,598,599, 448, 0,  1,  0,  1,482,483,484, 2},
        {2,471,  0,  1,  0,  1,  0,  1,  0,  1,  0,  1, 0,  1,  0,  1,479,480,481, 2},
        {2,  1,  0,  1,  0,472,473,  1,  0,  1,  0,  1, 0,  1,  0,485,  0,486,487, 2}, 
        {2,  1,  0,468,  0,   1,447,  1,  0,217,  0,  1, 0,  1,  0,  1,  0,  1,  0, 2},
        {2,  1,  0,  1,466,467,  0,  1,  0,  1,  0,  1, 0,  1,  0,  1,468,  1,  0, 2},
        {2,501,  0,  1,  0,  1,  0,  1,  0,  1,  0,  1, 0,455,456,457,  0,  1,504, 2},
        {2,  1,502,  1,  0,  1,478,  1,  0,  1,  0,  1, 0,452,453,454,  0,  1,503, 2}, // Bas fermé
        {2,  1,  0,  1,  0,  1,474,475,476,477,  0,  1, 0, 22, 23,  0,  0,230,  0, 2},
        {2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 2,  2,  2,  2,  2,  2,  2, 2} // Bas fermé
    },
    // Couloir niveau 1
    {
        {2, 2, 2, 2, 2, 2, 2,  2,   0,492,493,  0,  2,  2, 2, 2, 2, 2, 2, 2}, // Haut (Suite)
        {2, 2, 2, 2, 2, 2, 2,  2,   0,490,491,  0,  2,  2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2,  2,   0,490,491,  0,  2,  2, 2, 2, 2, 2, 2, 2},   // Toits cotés
        {2, 2, 2, 2, 2, 2, 2,  2,   0,490,491,  0,  2,  2, 2, 2, 2, 2, 2, 2},   // Murs cotés
        {2, 2, 2, 2, 2, 2, 2,  2,   0,490,491,607,  2,  2, 2, 2, 2, 2, 2, 2}, // Vide (Sol)
        {2, 2, 2, 2, 2, 2, 2,  2,   0,490,491,  0,  2,  2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2,  2,   0,490,491,376,  2,  2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2,  2,   0,490,491,  0,  2,  2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2,  2, 172,490,491,  0,  2,  2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2,  2,  71,490,491,  0,  2,  2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2,  2,   0,490,491,  0,  2,  2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2,  2,   0,490,491,  0,  2,  2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2,  2,   0,490,491,496,  2,  2, 2, 2, 2, 2, 2, 2}, // Bas (Entrée)
        {2, 2, 2, 2, 2, 2, 2,  2,   0,490,491,  0,  2,  2, 2, 2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2, 2, 2,  2,   0,488,489,  0,  2,  2, 2, 2, 2, 2, 2, 2}
    }, 
    // CARTE 3 : HALL (Niveau 2)
    {
        {2,  2,  2,  2,  2,  2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2, 2}, // Trou en haut (tout fermé en 2 ici ?)
        {2,  2,  2,  2,  2,  527, 528, 2,   0,   0,   0,   0,   2,   2,   541, 2,   551, 552, 2, 2},
        {2,  2,  2,  2,  2,  525, 526, 544, 545, 0,   0,   0,   2,   544, 545, 2,   549, 550, 2, 2},
        {2,  2,  2,  556, 555, 0,   0,   546, 547, 0,   0,   0,   0,   546, 547, 0,   0,   0,   2, 2},
        {2,  2,  2,  553, 554, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2, 2},
        {2,  2,  2,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2, 2},
        {2,  2,  2,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2, 2},
        {2,  2,  2,  535, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 538, 540, 0, 0},
        {2,  2,  2,  529, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 532, 534, 0, 0},
        {2,  2,  2,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2, 2},
        {2,  2,  2,  0,   0,   614, 615, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2, 2},
        {2,  2,  2,  610, 611, 612, 613, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2, 2},
        {2,  2,  2,  608, 609, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   555, 556, 0,   2, 2}, // Bas fermé
        {2,  2,  2,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   553, 554, 0,   2, 2},
        {2,  2,  2,  2,   2,   2,   2,   2,   0,   0,   0,   0,   2,   2,   2,   2,   2,   2,   2, 2}
     },
    // CARTE 4 CUISINE (Niveau 3)
    {       
        {2,   2,   2,   2,   2,   2,   2,   2,   0, 427, 428, 0, 2, 251, 252, 253,   2,   2, 2, 2}, // Trou en haut
        {2,   2, 270, 268, 269,   2, 273,   2,   0, 425, 426, 0, 2, 248, 250, 249,   2, 184, 2, 2},
        {2,   2, 242, 266, 267, 271, 272,   0,   0, 423, 424, 0, 0, 245, 246, 247, 244,   0, 2, 2},
        {2,   2,   0,   0,   0,   0,   0,   0,   0,   0,   0, 0, 0,   0,   0,   0,   0, 395, 2, 2},
        {2,   2,   0,   0,   0,   0,   0,   0,   0,   0,   0, 0, 0, 158, 159,   0,   0, 396, 2, 2},
        {2,   2,   0,   0,   0,   0,   0,   0,   0,   0,   0, 0, 0, 156, 157,   0,   0,   0, 2, 2},  
        {2, 184,   0,   0,   0,   0,   0,   0,   0,   0,   0, 0, 0,   0,   0,   0,   0,   0,397, 2},
        {2,   2,   0,   0, 263, 176,   0, 175,   0,   0,   0, 0, 0,   0,   0,   0,   0,   0, 0, 0},
        {2,   2, 275,   0, 264,   0,   0,   0,   0,   0,   0, 0, 0,   0,   0,   0,   0,   0, 0, 0},
        {2,   2, 265,   0, 263, 179, 180,   0, 177,   0,   0, 0, 0,   0,   0,   0,   0,   0, 2, 2},
        {2,   2, 171,   0, 264, 260, 261,   0, 262,   0,   0, 0, 0,   0,   0,   0,   0,   0, 2, 2},
        {2,   2, 181,   0,   0, 257, 258, 258, 259,   0,   0, 0, 0,   0,   0,   0,   0, 394, 2, 2},
        {2,   2,   0,   0,   0, 254, 255, 255, 256,   0,   0, 0, 0,   0,   0,   0,   0, 393, 2, 2}, // Bas fermé
        {2,   2,   0, 178, 169, 170, 167, 168, 243, 274,   0, 0, 0,   0,   0,   0,   0,   0, 2, 2},
        {2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2, 2, 2,   2,   2,   2,   2,   2, 2, 2}
    },
    // CARTE 5 SALLE A MANGER (Niveau 4 - dernière du bloc précédent)
    {    
        {2, 2, 2, 2, 2, 2, 2, 2,  2,425,426,  2,  2, 2, 2, 2, 2, 2, 2, 2}, // Trou en haut
        {2, 2, 2, 2, 2, 2, 2, 2,  2,423,424,  2,  2, 2, 2, 2, 2, 8, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0,283,281,277,292,  0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0,284,  0,291,239,285, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0,283,  0,290,  0,286, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0,284,  0,  0,  0,285, 0, 0, 0, 0, 0, 0, 0},
        {2, 2, 0, 0, 0, 0, 0, 0,283,239,  0,288,286, 0, 0, 0, 0, 0, 0, 0},
        {2, 2, 0, 0, 0, 0, 0, 0,284,  0,  0,239,285, 0, 0, 0, 0, 0, 0, 0},
        {2, 2, 0, 0, 0, 0, 0, 0,283,  0,287,  0,286, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0,284,  0,  0,  0,285, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0,283,239,  0,289,286, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 0, 0, 0, 0, 0, 0,284,276,277,278,  0, 0, 0, 0, 0, 0, 2, 2}, // Bas fermé
        {2, 2, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 2, 2},
        {2, 2, 2, 2, 2, 2, 2, 2,  0,  0,  0,  0,  2, 2, 2, 2, 2, 2, 2, 2}
    },
       {      // Jardin Niveau 5
        {446, 642, 642, 642, 642, 642, 642, 642, 642, 642, 642, 642, 642, 642, 642, 642, 642, 642, 642, 642}, 
        {446, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 642}, 
        {446, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 642},
        {446, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 642},
        {446, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 642},
        {446, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 642},
        {446, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 642},
        {641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 642},
        {641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 642},
        {446, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 642},
        {446, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 642},
        {446, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 642},
        {446, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 642}, 
        {446, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 641, 642},
        {446, 642, 642, 642, 642, 642, 642, 642, 642, 642, 642, 642, 642, 642, 642, 642, 642, 642, 642, 642}  
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
        {2, 2, 2,338,339, 2,  2,  2,334, 335,  2,  2, 2, 2,   2,   2,415,  2, 2}, // Trou en haut
        {2, 2, 2,336,337, 2,  2,  2,332, 333,  2,  2, 2, 2,   2,   2,414,  0,  2, 2},
        {2, 2, 0,  0,  0, 0,  0,   0,  0,  0,  0,  0, 0, 0,   0,   0,413,  0,  2, 2},
        {2, 2, 0,  0,  0, 0,398, 308,309,310,311,398, 0, 0, 304, 305,  0,  0,  2, 2},
        {2, 2, 0,  0,  0, 0,399, 312,313,314,315,399, 0, 0, 302, 303,  0,  0,  2, 2},
        {2, 2, 0,  0,  0, 0,  0,   0,  0,  0,  0,  0, 0, 0, 324, 325,  0,  0,  2, 2},
        {0, 0, 0,  0,  0, 0,  0,   0,  0,  0,  0,  0, 0, 0,   0,   0,  0,  0,  2, 2},
        {0, 0, 0,  0,  0, 0,  0,   0,  0,  0,  0,  0, 0, 0,   0,   0,  0,437,438, 2},
        {0, 0, 0,  0,  0, 0,  0,   0,  0,  0,  0,  0, 0, 0,   0,   0,  0,435,436, 2},
        {2, 2,402,403, 0, 0,  0,   0,  0,  0,  0,  0, 0, 0,   0,   0,  0,  0,  2, 2},
        {2, 2,400,401, 0, 0,  0,   0,  0,  0,  0,  0, 0, 0,   0,   0,384,385,  2, 2},
        {2, 2,324,325, 0,404,405,406,  0,  0,  0,  0, 0, 0, 380, 381,382,383,  2, 2},
        {2, 2, 0,  0,  0,407,408,409,  0,  0,  0,  0, 0, 0, 378, 379,  0,  0,  2, 2}, // Bas fermé
        {2, 2, 0,  0,  0,410,411,412,  0,  0,  0,  0, 0, 0,   0,   0,  0,  0,  2, 2},
        {2, 2, 2,  2,  2, 2,  2,   2,  2,  2,  2,  2, 2, 2,   2,   2,  2,  2,  2, 2}
    },
    // TENTE (index 8)
    {
        {50, 50, 50, 50, 50,  50,  50,  50,  50,  50,  50,  50,  50,  50, 50, 50, 50, 50, 50, 50}, // Marge Haut
        {50, 50, 50, 50, 50,  50,  50,  50,  50,  50,  50,  50,  50,  50, 50, 50, 50, 50, 50, 50},
        {50, 50, 50, 50, 50,  50,  50,  50,  50,  50,  50,  50,  50,  50, 50, 50, 50, 50, 50, 50},
        {50, 50, 50, 50, 50, 151, 152, 151, 152, 151, 152, 151, 152, 151, 50, 50, 50, 50, 50, 50}, // Début Rectangle
        {50, 50, 50, 50, 50, 366,   1,   0,   1,   0, 485, 469,   1, 364, 50, 50, 50, 50, 50, 50},
        {50, 50, 50, 50, 50, 366, 472,   0,   1,   0,   1,   0, 557, 364, 50, 50, 50, 50, 50, 50},
        {50, 50, 50, 50, 50,   0,   1,   0,   1,   0,   1,   0,   1, 364, 50, 50, 50, 50, 50, 50},
        {50, 50, 50, 50, 50,   0,   1,   0, 558, 486, 487,   0,   1, 364, 50, 50, 50, 50, 50, 50},
        {50, 50, 50, 50, 50,   0,   1,   0,   1,   0, 447, 559, 174, 364, 50, 50, 50, 50, 50, 50}, 
        {50, 50, 50, 50, 50, 366, 485,   0,   1,   0,   1, 468,   1, 364, 50, 50, 50, 50, 50, 50},
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