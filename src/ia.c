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
    if (type == 0 || type == 1 || type == 82) {
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
        if (type != 0 && type != 1 && type != 82) return 0; // Mur détecté !

        if (x0 == xEnd && y0 == yEnd) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
    return 1; // Vue dégagée
}

void ActionFantome(int rayonDetection) {
    static int isChasing = 0; 
    
    // --- 1. ANALYSE ---
    float dx = player.x - fantome.x;
    float dy = player.y - fantome.y;
    double distance = sqrt(dx*dx + dy*dy);

    // SECURITÉ : Si le fantôme est téléporté (changement salle), on reset son cerveau
    if (distance > 200) { 
        isChasing = 0;
        fantome.timer = 0;
    }

    if (isChasing) {
        FANTOME_SPEED = 0.75f; // Il accélère quand il te voit
        if (distance > (5 * TILE_SIZE) || currentLevel < 4) {
            isChasing = 0;
            fantome.timer = 0; 
            FANTOME_SPEED = 0.5f;
        }
    } 
    else {
        FANTOME_SPEED = 0.5f;
        if (distance < rayonDetection && currentLevel >= 4) {
            if (CheckLineOfSight(fantome.x + fantome.w/2, fantome.y + fantome.h/2, 
                                 player.x + player.w/2, player.y + player.h/2)) {
                isChasing = 1;
                fantome.timer = 0; 
            }
        }
    }

    // --- 2. COLLISIONS & TIMER ---
    float testX = fantome.x;
    float testY = fantome.y;

    if (fantome.direction == 0) testY -= FANTOME_SPEED;
    else if (fantome.direction == 1) testY += FANTOME_SPEED;
    else if (fantome.direction == 2) testX -= FANTOME_SPEED;
    else if (fantome.direction == 3) testX += FANTOME_SPEED;

    int hitWall = !CheckCollisionFantome(testX, testY);
    fantome.timer--;

    // Si on est en chasse, on réfléchit très souvent (dès qu'on touche une case ou un mur)
    int onGrid = (((int)fantome.x % TILE_SIZE == (TILE_SIZE - fantome.w)/2) && 
                  ((int)fantome.y % TILE_SIZE == (TILE_SIZE - fantome.h)/2));
    
    if (isChasing && (onGrid || hitWall)) {
        fantome.timer = 0;
    }

    // --- 3. CERVEAU (NOUVELLE LOGIQUE DE GLISSEMENT) ---
    if (hitWall || fantome.timer <= 0) {
        
        int bestDir = -1;
        float marge = 4.0f; // Marge pour anticiper les murs

        // >>> MODE CHASSEUR INTELLIGENT (AXIS PRIORITY) <<<
        if (isChasing) {
            // 1. Quelles directions mènent vers le joueur ?
            int dirH = (dx > 0) ? 3 : 2; // 3=Droite, 2=Gauche
            int dirV = (dy > 0) ? 1 : 0; // 1=Bas, 0=Haut
            
            // 2. Vérifions si ces directions sont libres (sans murs)
            int freeH = 0;
            // On vérifie si aller horizontalement est possible
            if (dirH == 2 && CheckCollisionFantome(fantome.x - marge, fantome.y)) freeH = 1;
            if (dirH == 3 && CheckCollisionFantome(fantome.x + marge, fantome.y)) freeH = 1;

            int freeV = 0;
            // On vérifie si aller verticalement est possible
            if (dirV == 0 && CheckCollisionFantome(fantome.x, fantome.y - marge)) freeV = 1;
            if (dirV == 1 && CheckCollisionFantome(fantome.x, fantome.y + marge)) freeV = 1;

            // 3. PRISE DE DÉCISION (GLISSEMENT)
            // On privilégie l'axe où le joueur est le plus loin (fabs)
            if (fabs(dx) > fabs(dy)) {
                // Le joueur est plus loin horizontalement que verticalement
                if (freeH) {
                    bestDir = dirH; // On fonce horizontalement
                } else if (freeV) {
                    bestDir = dirV; // Bloqué horizontalement ? On glisse verticalement !
                } else {
                    // Cul de sac ? On tente l'opposé vertical pour débloquer
                    int opposeV = (dirV == 0) ? 1 : 0;
                    if (CheckCollisionFantome(fantome.x, fantome.y + (opposeV==1?marge:-marge))) 
                        bestDir = opposeV;
                }
            } 
            else {
                // Le joueur est plus loin verticalement
                if (freeV) {
                    bestDir = dirV; // On fonce verticalement
                } else if (freeH) {
                    bestDir = dirH; // Bloqué verticalement ? On glisse horizontalement !
                } else {
                    // Cul de sac ? On tente l'opposé horizontal
                    int opposeH = (dirH == 2) ? 3 : 2;
                    if (CheckCollisionFantome(fantome.x + (opposeH==3?marge:-marge), fantome.y)) 
                        bestDir = opposeH;
                }
            }
            
            // Timer très court pour réagir vite aux changements
            fantome.timer = (int)(8 / FANTOME_SPEED); 
        }
        
        // >>> MODE PATROUILLE (ALEATOIRE) <<<
        if (bestDir == -1) { 
            // On liste les directions possibles
            int dirs[4]; int nbDirs = 0;
            if (CheckCollisionFantome(fantome.x, fantome.y - marge)) dirs[nbDirs++] = 0;
            if (CheckCollisionFantome(fantome.x, fantome.y + marge)) dirs[nbDirs++] = 1;
            if (CheckCollisionFantome(fantome.x - marge, fantome.y)) dirs[nbDirs++] = 2;
            if (CheckCollisionFantome(fantome.x + marge, fantome.y)) dirs[nbDirs++] = 3;

            if (nbDirs > 0) {
                // On évite le demi-tour sauf si cul-de-sac
                int oppose = -1;
                if (fantome.direction == 0) oppose = 1; else if (fantome.direction == 1) oppose = 0;
                else if (fantome.direction == 2) oppose = 3; else if (fantome.direction == 3) oppose = 2;

                int validDirs[4]; int nbValid = 0;
                for(int i=0; i<nbDirs; i++) {
                    if (dirs[i] != oppose) validDirs[nbValid++] = dirs[i];
                }

                if (nbValid > 0) bestDir = validDirs[rand() % nbValid];
                else bestDir = dirs[0];
            }
            fantome.timer = 30 + rand() % 60; 
        }

        // 4. APPLICATION
        if (bestDir != -1) {
            fantome.direction = bestDir;
            // Snapping (Centrage) pour éviter de frotter les murs
            if (fantome.direction <= 1) { // Vertical
                int col = (int)((fantome.x + fantome.w/2) / TILE_SIZE);
                fantome.x = (float)(col * TILE_SIZE + (TILE_SIZE - fantome.w)/2.0f);
            } else { // Horizontal
                int lig = (int)((fantome.y + fantome.h/2) / TILE_SIZE);
                fantome.y = (float)(lig * TILE_SIZE + (TILE_SIZE - fantome.h)/2.0f);
            }
        } else {
            fantome.timer = 10; // Bloqué totalement
        }
    } 
    
    // --- 4. MOUVEMENT FINAL ---
    float moveX = fantome.x;
    float moveY = fantome.y;

    if (fantome.direction == 0) moveY -= FANTOME_SPEED;
    else if (fantome.direction == 1) moveY += FANTOME_SPEED;
    else if (fantome.direction == 2) moveX -= FANTOME_SPEED;
    else if (fantome.direction == 3) moveX += FANTOME_SPEED;

    if (CheckCollisionFantome(moveX, moveY)) {
        fantome.x = moveX;
        fantome.y = moveY;
    }
}