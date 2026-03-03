#include "intro.h"
#include "game.h"
#include "game_reveille.h" 
#include <stdio.h>
#include <stdlib.h>

extern int dialogueStep;
extern int fin_jeu;
extern SDL_Texture *tilesetTexture;

int isColorMode = 0; 
static Uint32 debutIntro = 0;
static SDL_Texture* renderTarget = NULL;

void InitIntro(void) {
    debutIntro = 0;
    dialogueStep = 0;
    isColorMode = 0;
    renderTarget = NULL;
}

void StartIntro(SDL_Renderer *renderer) {
    debutIntro = SDL_GetTicks();
    dialogueStep = 0; 
    currentLevel = 0;
    
    player.x = 3 * 16 + 8; 
    player.y = 2 * 16 + 12; 

    if (!renderTarget) {
        renderTarget = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 320, 240);
    }
}

void SkipIntro(void) {
    if (renderTarget) {
        SDL_DestroyTexture(renderTarget);
        renderTarget = NULL;
    }
    player.x = 5 * 16; 
    player.y = 3 * 16;
    isColorMode = 0;   
    dialogueStep = 2; 
}

int UpdateIntroTimer(void) {
    Uint32 temps = SDL_GetTicks() - debutIntro;

    if (temps < 12000) {
        player.x = 3 * 16 + 8; 
        player.y = 2 * 16 + 12; 
    }
 
    else if (temps < 18000) {
        player.x = 5 * 16; 
        player.y = 3 * 16;
        isColorMode = 0; 
    }
    else {
        SkipIntro();
        return 1; 
    }
    return 0;
}

void dessinerTexteInterne(SDL_Renderer *renderer, TTF_Font *font, const char *texte, int y) {
    SDL_Color colorBlanc = {255, 255, 255, 255};
    SDL_Surface *sSurface = TTF_RenderText_Solid(font, texte, colorBlanc);
    if (sSurface) {
        SDL_Texture *tTexture = SDL_CreateTextureFromSurface(renderer, sSurface);
        int textWidth = sSurface->w;
        int screenWidth = 320;
        int x = (screenWidth - textWidth) / 2;
        SDL_Rect rText = {x, y, sSurface->w, sSurface->h};
        SDL_RenderCopy(renderer, tTexture, NULL, &rText);
        SDL_FreeSurface(sSurface);
        SDL_DestroyTexture(tTexture);
    }
}
void DrawIntro(SDL_Renderer *renderer, TTF_Font *font, TTF_Font *fontMini) {
    Uint32 temps = SDL_GetTicks() - debutIntro;

    if (temps < 12000) {
        isColorMode = 1; 
        
        float zoom = 1.0f;
        float progress = 0.0f;


        if (temps > 6000) {
            float t = (temps - 6000) / 6000.0f; 
            progress = t * t * t * t; 
            zoom = 1.0f + (3.0f * progress); 
        }

        float focusX = (3 * 16) + 8.0f; 
        float focusY = (2 * 16) + 12.0f; 

        float currentScreenX = focusX + (160.0f - focusX) * progress;
        float currentScreenY = focusY + (120.0f - focusY) * progress;

        int shakeX = 0;
        int shakeY = 0;
        if (temps > 10000) {
            float intensite = ((temps - 10000) / 2000.0f) * 6.0f; 
            shakeX = (rand() % (int)(intensite * 2 + 1)) - (int)intensite;
            shakeY = (rand() % (int)(intensite * 2 + 1)) - (int)intensite;
        }

        SDL_Rect destRect = {
            (int)(currentScreenX - (focusX * zoom)) + shakeX,
            (int)(currentScreenY - (focusY * zoom)) + shakeY,
            (int)(320.0f * zoom),
            (int)(240.0f * zoom)
        };

        SDL_SetRenderTarget(renderer, renderTarget);

        DrawGameReveille(renderer, font, fontMini);

      
        if (temps < 6000) {
            float momX = 5.0f * 16.0f;
            float momY = 3.0f * 16.0f; 
        
            if (temps > 1500 && temps <= 3500) {
                float t = (temps - 1500) / 2000.0f; 
                momX += t * (4.5f * 16.0f);
            }
        
            else if (temps > 3500) {
                momX += 4.5f * 16.0f; 
                
                if (temps <= 5500) {
                    float t = (temps - 3500) / 2000.0f;
                    momY -= t * (4.0f * 16.0f);
                } else {
                    momY -= 4.0f * 16.0f; 
                }
            }

            SDL_Rect srcMamanHaut = { 672 * 16, 0, 16, 16 };
            SDL_Rect dstMamanHaut = { (int)momX - 2, (int)momY - 18, 16, 16 };
            SDL_RenderCopy(renderer, tilesetTexture, &srcMamanHaut, &dstMamanHaut);

            SDL_Rect srcMamanBas = { 671 * 16, 0, 16, 16 };
            SDL_Rect dstMamanBas = { (int)momX - 2, (int)momY - 2, 16, 16 };
            SDL_RenderCopy(renderer, tilesetTexture, &srcMamanBas, &dstMamanBas);
        }

        if (temps < 3500) {
            DrawTexte("Bonne nuit maman !", renderer, font, 20, 180, 280, 50);
        }
        if (temps > 5000 && temps < 12000) {
            SDL_Color cBlanc = {200, 200, 255, 255};
            int offsetAnim = (SDL_GetTicks() / 200) % 5;
            SDL_Surface *sZzz = TTF_RenderText_Solid(fontMini, "Zzz...", cBlanc);
            if (sZzz) {
                SDL_Texture *tZzz = SDL_CreateTextureFromSurface(renderer, sZzz);
                SDL_Rect rZzz = { (int)focusX - 5, (int)focusY - 15 - offsetAnim, sZzz->w, sZzz->h };
                SDL_RenderCopy(renderer, tZzz, NULL, &rZzz);
                SDL_FreeSurface(sZzz);
                SDL_DestroyTexture(tZzz);
            }
        }

        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderCopy(renderer, renderTarget, NULL, &destRect);

        int alpha = 0;
        if (temps > 4500 && temps <= 5500) {
            alpha = (temps - 4500) * 180 / 1000; 
        }
        else if (temps > 5500 && temps <= 10000) {
            alpha = 180;
        }
        else if (temps > 10000) {

            alpha = 180 + ((temps - 10000) * 75 / 2000); 
        }

        if (alpha > 255) alpha = 255;

        if (alpha > 0) {
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, alpha);
            SDL_RenderFillRect(renderer, NULL);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        }
    }
    else if (temps < 15000) {
        isColorMode = 0; 
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (temps > 12500 && temps < 14800) {
            SDL_Color cBlanc = {230, 230, 230, 255};
            SDL_Surface *sTexte = TTF_RenderText_Solid(font, "Maman ? Papa ? Il fait tout noir...", cBlanc);
            if (sTexte) {
                SDL_Texture *tTexte = SDL_CreateTextureFromSurface(renderer, sTexte);
                SDL_Rect rTexte = { (320 - sTexte->w) / 2, (240 - sTexte->h) / 2, sTexte->w, sTexte->h };
                SDL_RenderCopy(renderer, tTexte, NULL, &rTexte);
                SDL_FreeSurface(sTexte);
                SDL_DestroyTexture(tTexte);
            }
        }
    }
    else if (temps < 18000) {
        isColorMode = 0; 
        
        float t = (temps - 15000) / 3000.0f; 
        float invT = 1.0f - t;
        float progress = 1.0f - (invT * invT * invT);
        
        float zoom = 4.0f - (3.0f * progress);

        float focusX = (5 * 16) + 8.0f; 
        float focusY = (3 * 16) + 8.0f; 

        float currentScreenX = 160.0f + (focusX - 160.0f) * progress;
        float currentScreenY = 120.0f + (focusY - 120.0f) * progress;

        SDL_Rect destRect = {
            (int)(currentScreenX - (focusX * zoom)),
            (int)(currentScreenY - (focusY * zoom)),
            (int)(320.0f * zoom),
            (int)(240.0f * zoom)
        };

        SDL_SetRenderTarget(renderer, renderTarget);
        DrawGame(renderer, font, fontMini);
        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderCopy(renderer, renderTarget, NULL, &destRect);

        int alpha = 255 - (int)(255 * progress);
        if (alpha < 0) alpha = 0;

        if (alpha > 0) {
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, alpha);
            SDL_RenderFillRect(renderer, NULL);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        }
    }
}