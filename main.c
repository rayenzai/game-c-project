#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

// --- CONFIGURATION STYLE MINIT (INVERSÉ) ---
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define LOGICAL_WIDTH 320
#define LOGICAL_HEIGHT 240

// 3. Palette 1-bit stricte INVERSÉE
// On inverse les valeurs RVB :
// Ce qu'on appelle "NOIR" dans le code sera visuellement BLANC (255,255,255)
// Ce qu'on appelle "BLANC" dans le code sera visuellement NOIR (0,0,0)
SDL_Color NOIR = {255, 255, 255, 255}; // Visuellement BLANC
SDL_Color BLANC = {0, 0, 0, 255};      // Visuellement NOIR

// Fonction utilitaire pour dessiner du texte centré
void dessinerTexte(SDL_Renderer *renderer, TTF_Font *font, const char *texte, int y, int selected) {
    // Si sélectionné : utilise la couleur "BLANC" (qui est noire visuellement). Sinon "NOIR" (blanche).
    SDL_Color colorTexte = selected ? BLANC : NOIR;
    
    SDL_Surface *surface = TTF_RenderText_Solid(font, texte, colorTexte);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    
    SDL_Rect rect;
    rect.x = (LOGICAL_WIDTH - surface->w) / 2;
    rect.y = y;
    rect.w = surface->w;
    rect.h = surface->h;

    if (selected) {
        // Le rectangle de sélection utilise la couleur "NOIR" (donc blanc visuellement)
        SDL_SetRenderDrawColor(renderer, NOIR.r, NOIR.g, NOIR.b, 255);
        SDL_Rect bgRect = {rect.x - 4, rect.y - 2, rect.w + 8, rect.h + 4};
        SDL_RenderFillRect(renderer, &bgRect);
    }

    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// Fonction pour dessiner un curseur "épée"
void dessinerCurseurEpee(SDL_Renderer *renderer, int x, int y) {
    // L'épée utilise la couleur "NOIR" (donc blanche visuellement)
    SDL_SetRenderDrawColor(renderer, NOIR.r, NOIR.g, NOIR.b, 255);
    SDL_Rect p1 = {x+2, y, 2, 6};   SDL_RenderFillRect(renderer, &p1);
    SDL_Rect p2 = {x, y+4, 6, 2};   SDL_RenderFillRect(renderer, &p2);
    SDL_Rect p3 = {x+2, y+6, 2, 3}; SDL_RenderFillRect(renderer, &p3);
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window *window = SDL_CreateWindow("MINIT STYLE DARK", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_RenderSetLogicalSize(renderer, LOGICAL_WIDTH, LOGICAL_HEIGHT);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

    // Assure-toi que le chemin vers ta police est correct !
    TTF_Font *fontTitre = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSansMono-Bold.ttf", 20);
    TTF_Font *fontMenu = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSansMono-Bold.ttf", 12);

    if (!fontTitre || !fontMenu) { printf("Erreur Font - Verifiez le chemin\n"); return 1; }

    const char *options[] = {"JOUER", "OPTIONS", "QUITTER"};
    int nbOptions = 3;
    int selection = 0;
    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:    selection--; if (selection < 0) selection = nbOptions - 1; break;
                    case SDLK_DOWN:  selection++; if (selection >= nbOptions) selection = 0; break;
                    case SDLK_RETURN: if (selection == 2) running = 0; break;
                }
            }
        }

        // 1. Fond NOIR (0,0,0)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // 2. Dessin du Titre
        dessinerTexte(renderer, fontTitre, "nom du jeu", 30, 0);

        // 3. Dessin du Menu
        int startY = 100;
        for (int i = 0; i < nbOptions; i++) {
            int itemY = startY + (i * 25);
            dessinerTexte(renderer, fontMenu, options[i], itemY, (i == selection));
            
            if (i == selection) {
                dessinerCurseurEpee(renderer, LOGICAL_WIDTH/2 - 60, itemY + 2);
            }
        }

        // 4. Bordure BLANCHE (255,255,255)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect bordure = {2, 2, LOGICAL_WIDTH - 4, LOGICAL_HEIGHT - 4};
        SDL_RenderDrawRect(renderer, &bordure);

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(fontTitre);
    TTF_CloseFont(fontMenu);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}