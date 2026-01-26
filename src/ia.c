#include "game.h"
#include "map.h"
#include <stdio.h>
#include <math.h>

Joueur player;
Fantome fantome;

float FANTOME_SPEED;

void SpawnFantomeRandom() {
    int validPosition = 0;
    int maxEssais = 100; // Sécurité pour ne pas boucler à l'infini

    while (!validPosition && maxEssais > 0) {
        // 1. On choisit une case au hasard (en évitant les bords extérieurs)
        int rx = (rand() % (MAP_WIDTH - 2)) + 1;
        int ry = (rand() % (MAP_HEIGHT - 2)) + 1;

        // 2. On vérifie le type de la case dans le niveau actuel
        int type = maps[currentLevel][ry][rx];

        // Si c'est un sol (0 ou 1)
        if (type == 0 || type == 1) {
            
            // 3. On vérifie que ce n'est pas trop près du joueur (Spawn kill)
            float fx = rx * TILE_SIZE;
            float fy = ry * TILE_SIZE;
            
            float dx = player.x - fx;
            float dy = player.y - fy;
            float dist = sqrt(dx*dx + dy*dy);

            // Si on est à plus de 5 tuiles (80 pixels) du joueur
            if (dist > 80) {
                fantome.x = fx;
                fantome.y = fy;
                fantome.direction = 0; // Reset direction
                fantome.timer = 0;     // Reset cerveau
                validPosition = 1;     // C'est bon, on sort de la boucle
            }
        }
        maxEssais--;
    }
    
    // Si après 100 essais on n'a pas trouvé (très improbable), on le met au milieu par défaut
    if (!validPosition) {
        fantome.x = 10 * TILE_SIZE;
        fantome.y = 7 * TILE_SIZE;
    }
}

int isWallSimple(float x, float y) {
    int caseX = (int)(x / TILE_SIZE);
    int caseY = (int)(y / TILE_SIZE);

    // 1. Sécurité : Si hors de la carte, c'est un mur
    if (caseX < 0 || caseX >= MAP_WIDTH || caseY < 0 || caseY >= MAP_HEIGHT) {
        return 1;
    }

    // 2. Lecture de la tuile
    int type = maps[currentLevel][caseY][caseX];

    // 3. Logique binaire pour le labyrinthe :
    // Les sols sont 0 et 1. Tout le reste (2, meubles, etc.) bloque le fantôme.
    if (type == 0 || type == 1) {
        return 0; // C'est libre
    }
    
    // Si tu as des passages secrets ou portes ouvertes (12, 13..), ajoute-les ici :
    // if (type >= 12 && type <= 19) return 0; 

    return 1; // C'est un mur
}

// Fonction utilitaire collision (avec marge de sécurité +1 pixel)
int CheckCollisionFantome(float x, float y) {
    // On utilise isWallSimple au lieu de isWall
    // On vérifie les 4 coins du fantôme
    if (isWallSimple(x + 1, y + 1)) return 0;
    if (isWallSimple(x + fantome.w - 1, y + 1)) return 0;
    if (isWallSimple(x + 1, y + fantome.h - 1)) return 0;
    if (isWallSimple(x + fantome.w - 1, y + fantome.h - 1)) return 0;
    
    return 1; // 1 = La voie est libre (c'est inversé dans ta logique actuelle)
}

// Vérifie si un mur bloque la vue entre (x1,y1) et (x2,y2)
int CheckLineOfSight(float x1, float y1, float x2, float y2) {
    int x0 = (int)(x1 / TILE_SIZE);
    int y0 = (int)(y1 / TILE_SIZE);
    int xEnd = (int)(x2 / TILE_SIZE);
    int yEnd = (int)(y2 / TILE_SIZE);

    int dx = abs(xEnd - x0);
    int dy = -abs(yEnd - y0);
    int sx = x0 < xEnd ? 1 : -1;
    int sy = y0 < yEnd ? 1 : -1;
    int err = dx + dy, e2;

    while (1) {
        if (x0 < 0 || x0 >= MAP_WIDTH || y0 < 0 || y0 >= MAP_HEIGHT) return 0;
        
        // On utilise ta fonction isWallSimple ou on vérifie le type directement
        int type = maps[currentLevel][y0][x0];
        if (type != 0 && type != 1) return 0; // Mur détecté !

        if (x0 == xEnd && y0 == yEnd) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
    return 1; // Vue dégagée
}

void ActionFantome(int rayonDetection) {
    static int isChasing = 0; 
    
    // --- 1. ANALYSE (CHASSE OU PAS ?) ---
    float dx = player.x - fantome.x;
    float dy = player.y - fantome.y;
    double distance = sqrt(dx*dx + dy*dy);

    if (isChasing) {
        FANTOME_SPEED = 0.75;
        // Arrêt chasse : trop loin ou changement de niveau
        if (distance > (5 * TILE_SIZE) || currentLevel < 4) {
            isChasing = 0;
            fantome.timer = 0; 
            FANTOME_SPEED = 0.5;
        }
    } 
    else {
        // Début chasse
        if (distance < rayonDetection && currentLevel >= 4) {
            if (CheckLineOfSight(fantome.x + fantome.w/2, fantome.y + fantome.h/2, 
                                 player.x + player.w/2, player.y + player.h/2)) {
                isChasing = 1;
                fantome.timer = 0; 
            }
        }
    }

    // --- 2. PRE-CALCUL COLLISION ACTUELLE ---
    float testX = fantome.x;
    float testY = fantome.y;

    if (fantome.direction == 0) { testY -= FANTOME_SPEED; }
    else if (fantome.direction == 1) { testY += FANTOME_SPEED; }
    else if (fantome.direction == 2) { testX -= FANTOME_SPEED; }
    else if (fantome.direction == 3) { testX += FANTOME_SPEED; }

    int hitWall = !CheckCollisionFantome(testX, testY);
    fantome.timer--;

    int onGrid = (((int)fantome.x % TILE_SIZE == (TILE_SIZE - fantome.w)/2) && 
                  ((int)fantome.y % TILE_SIZE == (TILE_SIZE - fantome.h)/2));
    
    if (isChasing && onGrid) {
        fantome.timer = 0;
    }

    // --- 3. CERVEAU (PRISE DE DECISION) ---
    if (hitWall || fantome.timer <= 0) {
        
        // Marge pour voir les murs venir (indispensable pour vitesse 0.5)
        float marge = 4.0f; 

        int dirs[4]; int nbDirs = 0;
        if (CheckCollisionFantome(fantome.x, fantome.y - marge)) { dirs[nbDirs++] = 0; }
        if (CheckCollisionFantome(fantome.x, fantome.y + marge)) { dirs[nbDirs++] = 1; }
        if (CheckCollisionFantome(fantome.x - marge, fantome.y)) { dirs[nbDirs++] = 2; }
        if (CheckCollisionFantome(fantome.x + marge, fantome.y)) { dirs[nbDirs++] = 3; }

        if (nbDirs > 0) {
            int bestDir = -1;

            // >>> MODE CHASSEUR <<<
            if (isChasing) {
                double minDst = 999999.0;
                
                for (int i = 0; i < nbDirs; i++) {
                    int d = dirs[i];
                    
                    int oppose = -1;
                    if (fantome.direction == 0) oppose = 1;
                    else if (fantome.direction == 1) oppose = 0;
                    else if (fantome.direction == 2) oppose = 3;
                    else if (fantome.direction == 3) oppose = 2;

                    // Simulation position
                    float simX = fantome.x; float simY = fantome.y;
                    if (d == 0) { simY -= TILE_SIZE; }
                    else if (d == 1) { simY += TILE_SIZE; }
                    else if (d == 2) { simX -= TILE_SIZE; }
                    else if (d == 3) { simX += TILE_SIZE; }
                    
                    double dSim = sqrt(pow(player.x - simX, 2) + pow(player.y - simY, 2));
                    
                    // --- CORRECTION CRUCIALE ICI ---
                    // On remet la condition "hitWall &&".
                    // SI on cogne un mur ET que c'est un demi-tour -> Pénalité (Anti-Vibration)
                    // SINON (couloir libre) -> Pas de pénalité -> Demi-tour autorisé immédiat !
                    if (hitWall && d == oppose && nbDirs > 1) {
                        dSim += 32.0; 
                    }

                    if (dSim < minDst) { 
                        minDst = dSim; 
                        bestDir = d; 
                    }
                }
                // Timer court pour réactivité
                fantome.timer = (int)(16 / FANTOME_SPEED); 
            }
            
            // >>> MODE PATROUILLE <<<
            if (bestDir == -1) { 
                int oppose = -1;
                if (fantome.direction == 0) oppose = 1;
                else if (fantome.direction == 1) oppose = 0;
                else if (fantome.direction == 2) oppose = 3;
                else if (fantome.direction == 3) oppose = 2;

                int validDirs[4]; int nbValid = 0;
                for(int i=0; i<nbDirs; i++) {
                    if (dirs[i] != oppose) { validDirs[nbValid++] = dirs[i]; }
                }

                if (nbValid > 0) { bestDir = validDirs[rand() % nbValid]; } 
                else { bestDir = dirs[0]; }
                
                fantome.timer = 30 + rand() % 60; 
            }

            fantome.direction = bestDir;

            // Recalage (Snapping) avec float pour supporter les 0.5px
            if (fantome.direction <= 1) { // Vertical
                int col = (int)((fantome.x + fantome.w/2) / TILE_SIZE);
                fantome.x = (float)(col * TILE_SIZE + (TILE_SIZE - fantome.w)/2.0f);
            } else { // Horizontal
                int lig = (int)((fantome.y + fantome.h/2) / TILE_SIZE);
                fantome.y = (float)(lig * TILE_SIZE + (TILE_SIZE - fantome.h)/2.0f);
            }

        } else {
            fantome.timer = 10; // Bloqué
        }
    } 
    
    // --- 4. MOUVEMENT ---
    float moveX = fantome.x;
    float moveY = fantome.y;

    if (fantome.direction == 0) { moveY -= FANTOME_SPEED; }
    else if (fantome.direction == 1) { moveY += FANTOME_SPEED; }
    else if (fantome.direction == 2) { moveX -= FANTOME_SPEED; }
    else if (fantome.direction == 3) { moveX += FANTOME_SPEED; }

    if (CheckCollisionFantome(moveX, moveY)) {
        fantome.x = moveX;
        fantome.y = moveY;
    }
}