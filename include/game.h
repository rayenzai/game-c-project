#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void InitGame(SDL_Renderer *renderer);
void UpdateGame(void);
void DrawGame(SDL_Renderer *renderer, TTF_Font *font);
extern int currentLevel; 
#endif