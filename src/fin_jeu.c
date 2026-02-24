#include "fin_jeu.h"

void lancer_scene_fin(SDL_Renderer *renderer, int id_choix) {
    switch(id_choix) {
        case 1:
            // FIN 1 il part du cauchemars
            printf("Lancement de la cinématique : Sortie du cauchemar\n");
            break;
        case 2:
            // FIN 2 il reste avec sa mère
            printf("Lancement de la cinématique : Rester avec maman\n");
            break;
        default:
            // Fin par défaut
            break;
    }
}