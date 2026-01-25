#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>

void InitGame(SDL_Renderer *renderer);
void UpdateGame(void);
void DrawGame(SDL_Renderer *renderer, TTF_Font *font, TTF_Font *fontMini);
extern int currentLevel; 

#endif