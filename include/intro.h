#ifndef INTRO_H
#define INTRO_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void InitIntro(void);

// Gère le clavier (Entrée pour passer) - Retourne 1 si fini
int HandleIntroInput(SDL_Event *event);

// Gère l'animation du texte (Timer) - A appeler à chaque image
void UpdateIntroTimer(void);

void DrawIntro(SDL_Renderer *renderer, TTF_Font *font);
void dessinerTexteInterne(SDL_Renderer *renderer, TTF_Font *font, const char *texte, int y);

#endif