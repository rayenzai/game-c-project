#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "intro.h"
#include "menu.h"
#include "game.h"
#include "sons.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define LOGICAL_WIDTH 320
#define LOGICAL_HEIGHT 240
#define FPS 60

typedef enum {
    ETAT_MENU,
    ETAT_INTRO,
    ETAT_JEU
} GameState;

int main(int argc, char* argv[]) {
    (void)argc; (void)argv;

    // -- INIT SONS --
    SDL_SetHint(SDL_HINT_AUDIO_RESAMPLING_MODE, "3"); 
    
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 3072) < 0) {
        printf("Erreur SDL_mixer : %s\n", Mix_GetError());
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) return 1;
    if (TTF_Init() < 0) return 1;

    SDL_Window *window = SDL_CreateWindow("nom du jeu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE); //acceleration materiel : SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC

    SDL_RenderSetLogicalSize(renderer, LOGICAL_WIDTH, LOGICAL_HEIGHT);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest"); //nearest pour une sorte de Vsync 

    TTF_Font *fontGrand = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSansMono-Bold.ttf", 20);
    TTF_Font *fontPetit = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSansMono-Bold.ttf", 12);
    TTF_Font *fontMini = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSansMono-Bold.ttf", 11);

    if (!fontGrand || !fontPetit || !fontMini) { printf("Erreur Font\n"); return 1; }

    Mix_Music *bgm = chargement_son_ambiance();

    GameState etat = ETAT_MENU;
    InitIntro();
    InitMenu();

    int running = 1;
    SDL_Event event;

    Uint32 frameStart;
    int frameTime;
    const int frameDelay = 1000 / FPS;

    // --- BOUCLE PRINCIPALE ---
    while (running) {
        frameStart = SDL_GetTicks();
        // A. GESTION DES EVENEMENTS (Clavier / Souris)
        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT) running = 0;

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F11) {
                // On récupère l'état actuel
                Uint32 flags = SDL_GetWindowFlags(window);
                
                // Si on est déjà en plein écran, on repasse en fenêtré (0)
                if (flags & SDL_WINDOW_FULLSCREEN_DESKTOP) {
                    SDL_SetWindowFullscreen(window, 0);
                } else {
                    // Sinon, on passe en plein écran "Bureau" (Borderless)
                    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                }
            }
            // -----------------------------------------

            if (etat == ETAT_MENU) {
                int action = UpdateMenu(&event);
                if (action == 1) { 
                    etat = ETAT_JEU; 
                    InitGame(renderer);
                }
                if (action == 2) running = 0;
            }
            else if (etat == ETAT_INTRO) {
                if (HandleIntroInput(&event) == 1) {
                    etat = ETAT_JEU;
                    InitGame(renderer);
                }
            }
            else if (etat == ETAT_JEU) {
                
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                    etat = ETAT_MENU;
                    currentLevel = 0;
                    
                }
            }
        }

        if (etat == ETAT_INTRO) {
            UpdateIntroTimer();
        }

        else if (etat == ETAT_JEU)
        {
            UpdateGame();
        }
        

        // C. DESSIN
        if (etat == ETAT_MENU) {
            DrawMenu(renderer, fontGrand, fontPetit);
        }
        else if (etat == ETAT_INTRO) {
            DrawIntro(renderer, fontPetit);
        }
        else if (etat == ETAT_JEU) {
            DrawGame(renderer,fontPetit, fontMini);
        }

        
    // 4. Bordure
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect bordure = {2, 2, LOGICAL_WIDTH - 4, LOGICAL_HEIGHT - 4};
    SDL_RenderDrawRect(renderer, &bordure);

    SDL_RenderPresent(renderer);
    frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    // Nettoyage sons
    Mix_FreeMusic(bgm);
    Mix_CloseAudio();

    // Nettoyage...
    TTF_CloseFont(fontGrand);
    TTF_CloseFont(fontPetit);
    TTF_CloseFont(fontMini);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}