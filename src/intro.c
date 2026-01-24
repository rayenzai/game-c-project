#include "intro.h"
#include <string.h>
#include <stdio.h>

#define LOGICAL_WIDTH 320
#define VITESSE_TEXTE 50

static const char *histoire[] = {
    "ANNEE 1537...",
    "LE ROI EST MORT.",
    "SON FANTOME HANTE LE CHATEAU.",
    "VOUS AVEZ 60 SECONDES...",
    "POUR BRISER LA MALEDICTION.",
    "Sinon c'est la mort...",
    "Bonne chance chevalier",
    "(Oui j'avais pas d'inspi)",
    NULL
};

static int phraseIndex = 0;
static int charIndex = 0;
static Uint32 dernierAjout = 0;
static char bufferTexte[256];
static SDL_Color BLANC_VISUEL = {255, 255, 255, 255};

static void dessinerTexteInterne(SDL_Renderer *renderer, TTF_Font *font, const char *texte, int y) {
    if (strlen(texte) == 0) return;
    SDL_Surface *surface = TTF_RenderText_Solid(font, texte, BLANC_VISUEL);
    if (!surface) return;
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = { (LOGICAL_WIDTH - surface->w) / 2, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void InitIntro(void) {
    phraseIndex = 0;
    charIndex = 0;
    dernierAjout = 0;
    bufferTexte[0] = '\0';
}

// 1. Gestion des touches (uniquement quand on appuie)
int HandleIntroInput(SDL_Event* event) {
    if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_RETURN) {
        if ((size_t)charIndex < strlen(histoire[phraseIndex])) {
            charIndex = strlen(histoire[phraseIndex]); // On affiche tout direct
        } else {
            phraseIndex++;
            charIndex = 0;
            if (histoire[phraseIndex] == NULL) {
                return 1; // Intro finie
            }
        }
    }
    return 0;
}

// 2. Gestion du temps (Appelé à chaque tour de boucle, même sans clavier)
void UpdateIntroTimer(void) {
    if (histoire[phraseIndex] != NULL) {
        // Le Timer avance tout seul
        if ((size_t)charIndex < strlen(histoire[phraseIndex])) {
            if (SDL_GetTicks() > dernierAjout + VITESSE_TEXTE) {
                charIndex++; 
                dernierAjout = SDL_GetTicks();
            }
        }
        // Mise à jour du texte à afficher
        strncpy(bufferTexte, histoire[phraseIndex], charIndex);
        bufferTexte[charIndex] = '\0';
    }
}

void DrawIntro(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    if (histoire[phraseIndex] != NULL) {
        dessinerTexteInterne(renderer, font, bufferTexte, 100); 
    }
}