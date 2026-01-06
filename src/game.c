#include "game.h"
#include <stdio.h>

// --- CONSTANTES ---
#define TILE_SIZE 16        // Taille d'un bloc (16x16 pixels)
#define PLAYER_SPEED 1.5f   // Vitesse de déplacement
#define LOGICAL_WIDTH 320
#define LOGICAL_HEIGHT 240


// --- STRUCTURES ---
typedef struct {
    float x, y;     // Position précise (float pour la fluidité)
    int w, h;       // Taille du joueur
} joueur;

static joueur player;

// --- 1. INITIALISATION ---
void InitGame(void) {
    player.x = 160; // Au milieu de l'écran (320 / 2)
    player.y = 120; // Au milieu de l'écran (240 / 2)
    player.w = 16;  // Largeur
    player.h = 16;  // Hauteur
}

// --- 2. LOGIQUE (Update) ---

void UpdateGame(void) {
    // On demande à SDL l'état de tout le clavier
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    // On regarde quelles touches sont enfoncées et on modifie x et y
    if (state[SDL_SCANCODE_UP]) {
        player.y -= PLAYER_SPEED;
    }
    if (state[SDL_SCANCODE_DOWN]) {
        player.y += PLAYER_SPEED;
    }
    if (state[SDL_SCANCODE_LEFT]) {
        player.x -= PLAYER_SPEED;
    }
    if (state[SDL_SCANCODE_RIGHT]) {
        player.x += PLAYER_SPEED;
    }

    // Si on dépasse en haut (0), on remet à 0
    if (player.y < 2) {
        player.y = 2;
    }
    // Si on dépasse en bas (240 - taille du joueur), on le colle au bord bas
    if (player.y + player.h > LOGICAL_HEIGHT - 2) {
        player.y = LOGICAL_HEIGHT -2 - player.h;
    }

    // Si on dépasse à gauche
    if (player.x < 2) {
        player.x = 2;
    }
    // Si on dépasse à droite
    if (player.x + player.w > LOGICAL_WIDTH - 2) {
        player.x = LOGICAL_WIDTH - 2 - player.w;
    }
}

// --- 3. DESSIN (Draw) ---

void DrawGame(SDL_Renderer *renderer) {
    // A. On efface tout en GRIS FONCÉ (pour bien voir le carré blanc)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    // B. On dessine le joueur en BLANC
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    // On crée un rectangle SDL temporaire avec les coordonnées du joueur
    // (On convertit les float en int car l'écran a des pixels entiers)
    SDL_Rect rectPlayer = {
        (int)player.x, 
        (int)player.y, 
        player.w, 
        player.h
    };

    // On remplit le rectangle (FillRect)
    SDL_RenderFillRect(renderer, &rectPlayer);
}