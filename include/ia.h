#ifndef IA_H
#define IA_H


#include "game.h"
#include "map.h"
#include <stdio.h>
#include <math.h>

void SpawnFantomeRandom();
int isWallSimple(float x, float y);
int CheckCollisionFantome(float x, float y);
int CheckLineOfSight(float x1, float y1, float x2, float y2);
void ActionFantome(int rayonDetection);

#endif 