#include "pause.h"
#include "menu.h" 

#define LOGICAL_WIDTH 320
#define LOGICAL_HEIGHT 240

static int selectionPause = 0;
static const int nbOptionsPause = 4;
static const char *optionsPause[] = {"CONTINUER", "MENU PRINCIPAL", "SAUVEGARDER", "QUITTER"};

int UpdatePause(SDL_Event *event) {
    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
            case SDLK_UP:
                selectionPause--;
                if (selectionPause < 0) selectionPause = nbOptionsPause - 1;
                break;
            case SDLK_DOWN:
                selectionPause++;
                if (selectionPause >= nbOptionsPause) selectionPause = 0;
                break;
            case SDLK_RETURN:
                if (selectionPause == 0) return 1; // Continuer
                if (selectionPause == 1) return 2; // Menu Principal
                if (selectionPause == 2) return 3; // Sauvegarder
                if (selectionPause == 3) return 4; // Quitter
                break;
            case SDLK_ESCAPE:
                return 1; // Enlève la pause
        }
    }
    return 0; // Reste en pause
}

void DrawPause(SDL_Renderer *renderer, TTF_Font *fontTitre, TTF_Font *fontOptions) {
    
    // 1. Assombrir l'écran
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200); 
    SDL_RenderFillRect(renderer, NULL); 

    // 2. Titre "MENU PAUSE" (On peut réutiliser dessinerTexteMenu même s'il n'est pas "sélectionné")
    // On met 'selected' à 0 (faux) pour qu'il s'affiche en blanc sans fond.
    dessinerTexteMenu(renderer, fontTitre, "MENU PAUSE", 30, 0);

    // 3. Dessiner les options en réutilisant les fonctions de menu.c
    int startY = 100; 
    for (int i = 0; i < nbOptionsPause; i++) {
        int itemY = startY + (i * 25); 
        
        dessinerTexteMenu(renderer, fontOptions, optionsPause[i], itemY, (i == selectionPause));
        
        if (i == selectionPause) {
            int textW, textH;
            TTF_SizeText(fontOptions, optionsPause[i], &textW, &textH);
            int croixX = (LOGICAL_WIDTH - textW) / 2 - 15; 
            
            dessinerCroix(renderer, croixX, itemY + 2);
        }
    }
}