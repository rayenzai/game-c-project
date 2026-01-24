#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void InitMenu(void);
// Retourne : 0 (Rien), 1 (Jouer), 2 (Quitter)
int UpdateMenu(SDL_Event *event);
void DrawMenu(SDL_Renderer *renderer, TTF_Font *fontTitre, TTF_Font *fontOptions);

#endif