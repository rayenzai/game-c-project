#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>


void InitGame(SDL_Renderer *renderer);
void UpdateGame(void);
void DrawGame(SDL_Renderer *renderer);

#endif