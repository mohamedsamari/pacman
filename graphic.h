#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_JOUEURS 10 // Nombre maximum de joueurs

extern Joueur joueurs[MAX_JOUEURS]; // Déclaration externe pour les joueurs
extern int nombreJoueurs; // Déclaration externe pour le compteur de joueurs

// Prototypes de fonctions
SDL_Renderer *initializeSDL(SDL_Window **window);
TTF_Font **loadFonts();
void renderText(SDL_Renderer *renderer, const char *text, TTF_Font *font, SDL_Color color, int x, int y);
void renderMainMenu(SDL_Renderer *renderer, TTF_Font **fonts, SDL_Color yellow, SDL_Color black, SDL_Color white, SDL_Color hoverColor);
void renderDifficultySelection(SDL_Renderer *renderer, TTF_Font **fonts, SDL_Color yellow, SDL_Color black, SDL_Color white, SDL_Color hoverColor);
void renderScorePage(SDL_Renderer *renderer, TTF_Font **fonts, SDL_Color yellow, SDL_Color black, SDL_Color white, SDL_Color hoverColor);
int handleMainMenuEvents(SDL_Event event, SDL_Rect rectStartGame, SDL_Rect rectScore);
const char* handleDifficultySelectionEvents(SDL_Event event);
int handleScorePageEvents(SDL_Event event);
void cleanup(SDL_Window *window, SDL_Renderer *renderer, TTF_Font **fonts);

#endif // GRAPHIC_H
