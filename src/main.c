#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "intro.h"
#include "menu.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define LOGICAL_WIDTH 320
#define LOGICAL_HEIGHT 240

typedef enum {
    ETAT_MENU,
    ETAT_INTRO,
    ETAT_JEU
} GameState;

int main(int argc, char* argv[]) {
    (void)argc; (void)argv;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) return 1;
    if (TTF_Init() < 0) return 1;

    SDL_Window *window = SDL_CreateWindow("nom du jeu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_RenderSetLogicalSize(renderer, LOGICAL_WIDTH, LOGICAL_HEIGHT);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

    TTF_Font *fontGrand = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSansMono-Bold.ttf", 20);
    TTF_Font *fontPetit = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSansMono-Bold.ttf", 12);

    if (!fontGrand || !fontPetit) { printf("Erreur Font\n"); return 1; }

    GameState etat = ETAT_MENU;
    InitIntro();
    InitMenu();

    int running = 1;
    SDL_Event event;

    // --- BOUCLE PRINCIPALE ---
    while (running) {
        
        // A. GESTION DES EVENEMENTS (Clavier / Souris)
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;

            if (etat == ETAT_MENU) {
                int action = UpdateMenu(&event);
                if (action == 1) { 
                    etat = ETAT_INTRO; 
                    InitIntro();
                }
                if (action == 2) running = 0;
            }
            else if (etat == ETAT_INTRO) {
                if (HandleIntroInput(&event) == 1) {
                    etat = ETAT_JEU;
                }
            }
            else if (etat == ETAT_JEU) {
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                    etat = ETAT_MENU;
                }
            }
        }

        if (etat == ETAT_INTRO) {
            UpdateIntroTimer();
        }

        // C. DESSIN
        if (etat == ETAT_MENU) {
            DrawMenu(renderer, fontGrand, fontPetit);
        }
        else if (etat == ETAT_INTRO) {
            DrawIntro(renderer, fontPetit);
        }
        else if (etat == ETAT_JEU) {
            SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
            SDL_RenderClear(renderer);
        }

        SDL_RenderPresent(renderer);
    }

    // Nettoyage...
    TTF_CloseFont(fontGrand);
    TTF_CloseFont(fontPetit);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}