#ifndef LOGIC_H
#define LOGIC_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL_ttf.h>


struct Entity {
    int x, y; // Position en pixels
    int dx, dy; // Direction
    SDL_Texture *texture; // Texture SDL
};

int isValidPosition(int x, int y);
void drawPoints(SDL_Renderer *renderer);
void drawMaze(SDL_Renderer *renderer);
void renderScore(SDL_Renderer *renderer, TTF_Font *font, int score);
SDL_Texture* loadTexture(SDL_Renderer *renderer, const char *filePath);
void updateGhosts(struct Entity ghosts[], int ghostCount);
void handlePacmanMovement(struct Entity *pacman);
void checkCollisions(struct Entity *pacman, struct Entity ghosts[], int ghostCount, int *running);
void cleanup_game(SDL_Texture *pacmanTexture, SDL_Texture *ghostTextures[], SDL_Renderer *renderer, SDL_Window *window);
int countTotalPoints(); // Ajout de la déclaration
int setGhostCount(const char *difficulty); // Ajout de la déclaration

extern int maze[11][22]; 


#endif