#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "intro.h"
#include "menu.h"
#include "game.h"
#include "sons.h"
#include "fin_jeu.h"

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 720
#define LOGICAL_WIDTH 320
#define LOGICAL_HEIGHT 240
#define FPS 60

typedef enum {
    ETAT_MENU,
    ETAT_INTRO,
    ETAT_CHARGEMENT,
    ETAT_JEU
} GameState;

int main(int argc, char* argv[]) {
    (void)argc; (void)argv;

    srand(time(NULL));

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    // --- AJOUT IMPORTANT ---
    // On prépare SDL_Mixer pour lire du OGG et du MP3 proprement
    int flags = MIX_INIT_OGG | MIX_INIT_MP3;
    if ((Mix_Init(flags) & flags) != flags) {
        printf("Erreur Mix_Init: %s\n", Mix_GetError());
    }



    if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Erreur SDL_mixer : %s\n", Mix_GetError());
    }

    // Mix_Chunk *sonPasEnBoucle = chargement_son_pas();

    // Mix_PlayChannel(2, sonPasEnBoucle, -1); // -1 = boucle infinie
    // Mix_Volume(2, 0); // On commence volume à 0

    if (SDL_Init(SDL_INIT_VIDEO) < 0) return 1;
    if (TTF_Init() < 0) return 1;

    SDL_Window *window = SDL_CreateWindow("Lights Out", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE); //acceleration materiel : SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC

    // Force la SDL à garder des gros carrés de pixels parfaits (sans flou)
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"); 

    // Force la SDL à utiliser un ratio parfait (x2, x3, etc.) sans jamais déformer l'image
    SDL_RenderSetIntegerScale(renderer, SDL_TRUE);

    SDL_RenderSetLogicalSize(renderer, LOGICAL_WIDTH, LOGICAL_HEIGHT);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest"); //nearest pour une sorte de Vsync 

    TTF_Font *fontGrand = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSansMono-Bold.ttf", 20);
    TTF_Font *fontPetit = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSansMono-Bold.ttf", 12);
    TTF_Font *fontMini = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSansMono-Bold.ttf", 11);

    if (!fontGrand || !fontPetit || !fontMini) { printf("Erreur Font\n"); return 1; }

    GameState etat = ETAT_MENU;
    InitIntro();
    InitMenu(renderer);
    int vraiPourcentage = 0;

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
                    etat = ETAT_CHARGEMENT; 
                    vraiPourcentage = 0;
                }
                if (action == 2) running = 0;
            }
            else if (etat == ETAT_INTRO) {
                if (HandleIntroInput(&event) == 1) {
                    etat = ETAT_CHARGEMENT;
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
        else if (etat == ETAT_CHARGEMENT) {
            int fini = InitGameStepByStep(renderer, &vraiPourcentage);
            
            if (fini == 1) {
                etat = ETAT_JEU;
            }
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
       else if (etat == ETAT_CHARGEMENT) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            int largeurMax = LOGICAL_WIDTH - 40; 
            int hauteurBarre = 4;                
            int posX = 20;                       
            int posY = LOGICAL_HEIGHT - 20;      

            SDL_Color cGris = {150, 150, 150, 255}; 
            SDL_Surface *sText = TTF_RenderText_Solid(fontMini, "Chargement...", cGris);
            if (sText) {
                SDL_Texture *tText = SDL_CreateTextureFromSurface(renderer, sText);
                SDL_Rect rText = { posX + largeurMax - sText->w, posY - sText->h - 4, sText->w, sText->h };
                SDL_RenderCopy(renderer, tText, NULL, &rText);
                SDL_FreeSurface(sText);
                SDL_DestroyTexture(tText);
            }

            SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); 
            SDL_Rect rFondBarre = { posX, posY, largeurMax, hauteurBarre };
            SDL_RenderFillRect(renderer, &rFondBarre);

            int remplissage = (vraiPourcentage * largeurMax) / 100; 
            
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
            SDL_Rect rRemplissage = { posX, posY, remplissage, hauteurBarre };
            SDL_RenderFillRect(renderer, &rRemplissage);
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