#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "menu.h"
#include <string.h>

#define LOGICAL_WIDTH 320
#define LOGICAL_HEIGHT 240

// Couleurs inversées (Style Minit)
static SDL_Color NOIR = {255, 255, 255, 255}; // Visuellement Blanc (Fond)
static SDL_Color BLANC = {0, 0, 0, 255};      // Visuellement Noir (Encre)

static int selection = 0;
static const char *options[] = {"JOUER", "OPTIONS", "QUITTER"};
static int nbOptions = 3;

// Fonction interne pour dessiner le texte du menu
static void dessinerTexteMenu(SDL_Renderer *renderer, TTF_Font *font, const char *texte, int y, int selected) {
    SDL_Color colorTexte = selected ? BLANC : NOIR;
    SDL_Surface *surface = TTF_RenderText_Solid(font, texte, colorTexte);
    if (!surface) return;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = { (LOGICAL_WIDTH - surface->w) / 2, y, surface->w, surface->h };

    if (selected) {
        SDL_SetRenderDrawColor(renderer, NOIR.r, NOIR.g, NOIR.b, 255);
        SDL_Rect bgRect = {rect.x - 4, rect.y - 2, rect.w + 8, rect.h + 4};
        SDL_RenderFillRect(renderer, &bgRect);
    }
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// Fonction pour l'épée
static void dessinerEpee(SDL_Renderer *renderer, int y) {
    SDL_SetRenderDrawColor(renderer, NOIR.r, NOIR.g, NOIR.b, 255);
    int x = (LOGICAL_WIDTH / 2) - 60;
    SDL_Rect p1 = {x+2, y, 2, 6};   SDL_RenderFillRect(renderer, &p1);
    SDL_Rect p2 = {x, y+4, 6, 2};   SDL_RenderFillRect(renderer, &p2);
    SDL_Rect p3 = {x+2, y+6, 2, 3}; SDL_RenderFillRect(renderer, &p3);
}

void InitMenu(void) {
    selection = 0;
}

int UpdateMenu(SDL_Event *event) {
    if (event->type == SDL_KEYDOWN) {
        switch (event->key.keysym.sym) {
            case SDLK_UP:
                selection--;
                if (selection < 0) selection = nbOptions - 1;
                break;
            case SDLK_DOWN:
                selection++;
                if (selection >= nbOptions) selection = 0;
                break;
            case SDLK_RETURN:
                if (selection == 0) return 1; // Lancer JEU
                if (selection == 2) return 2; // QUITTER
                break;
        }
    }
    return 0; // Rien ne change
}

void DrawMenu(SDL_Renderer *renderer, TTF_Font *fontTitre, TTF_Font *fontOptions) {
    // 1. Fond NOIR (Visuellement Blanc)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // 2. Titre
    dessinerTexteMenu(renderer, fontTitre, "LE JEU DU CHATEAU", 30, 0);

    // 3. Options
    int startY = 100;
    for (int i = 0; i < nbOptions; i++) {
        int itemY = startY + (i * 25);
        dessinerTexteMenu(renderer, fontOptions, options[i], itemY, (i == selection));
        
        if (i == selection) {
            dessinerEpee(renderer, itemY + 2);
        }
    }

}