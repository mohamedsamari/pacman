#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL_ttf.h>
#include "logic.h"

#define SCREEN_WIDTH 880
#define SCREEN_HEIGHT 440
#define CELL_SIZE 40

// Prototypes de fonctions
void startGame();

int main(int argc, char *argv[]) {
    startGame();
    return 0;
}

void startGame() {
    // Initialisation de SDL et TTF
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erreur SDL : %s\n", SDL_GetError());
        return;
    }
    if (TTF_Init() == -1) {
        printf("Erreur d'initialisation de SDL_ttf: %s\n", TTF_GetError());
        return;
    }

    // Création de la fenêtre et du renderer
    SDL_Window *window = SDL_CreateWindow("Pac-Man", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Erreur création fenêtre : %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erreur création renderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    // Chargement des textures
    SDL_Texture *pacmanTexture = loadTexture(renderer, "pacman_ferme_droit.bmp");
    SDL_Texture *ghostTextures[4];
    ghostTextures[0] = loadTexture(renderer, "fantome_bleu.bmp");
    ghostTextures[1] = loadTexture(renderer, "fantome_jaune.bmp");
    ghostTextures[2] = loadTexture(renderer, "fantome_rose.bmp");
    ghostTextures[3] = loadTexture(renderer, "fantome_vert.bmp");

    if (!pacmanTexture || !ghostTextures[0] || !ghostTextures[1] || !ghostTextures[2] || !ghostTextures[3]) {
        printf("Erreur chargement textures : %s\n", SDL_GetError());
        cleanup_game(pacmanTexture, ghostTextures, renderer, window);
        return;
    }

    // Initialisation des entités
    struct Entity pacman = {CELL_SIZE, CELL_SIZE, 0, 0, pacmanTexture};
    struct Entity ghosts[4] = {
        {5 * CELL_SIZE, 5 * CELL_SIZE, 1, 0, ghostTextures[0]},
        {10 * CELL_SIZE, 5 * CELL_SIZE, -1, 0, ghostTextures[1]},
        {15 * CELL_SIZE, 5 * CELL_SIZE, 0, 1, ghostTextures[2]},
        {20 * CELL_SIZE, 5 * CELL_SIZE, 0, -1, ghostTextures[3]}
    };

    int totalPoints = countTotalPoints();
    int collectedPoints = 0;

    // Sélection de la difficulté  
    char difficulty[10];
    printf("Select difficulty (easy, medium, hard): ");
    scanf("%s", difficulty);

    int ghostCount = setGhostCount(difficulty);
    if (ghostCount == 0) {
        printf("Invalid difficulty level. Defaulting to easy.\n");
        ghostCount = 2;
    }

    for (int i = 0; i < ghostCount; i++) {
        ghosts[i] = (struct Entity){(i + 5) * CELL_SIZE, 5 * CELL_SIZE, 1, 0, ghostTextures[i]};
    }

    // Boucle principale
    int running = 1;
    int score = 0;
    TTF_Font *font = TTF_OpenFont("arial.ttf", 24);
    if (!font) {
        printf("Erreur de chargement de la police: %s\n", TTF_GetError());
        cleanup_game(pacmanTexture, ghostTextures, renderer, window);
        return;
    }

    while (running) {
        handlePacmanMovement(&pacman);
        updateGhosts(ghosts, ghostCount);

        int pacmanRow = pacman.y / CELL_SIZE;
        int pacmanCol = pacman.x / CELL_SIZE;
        if (maze[pacmanRow][pacmanCol] == 0) {
            maze[pacmanRow][pacmanCol] = -1;
            score += 10;
            collectedPoints++;
        } else if (maze[pacmanRow][pacmanCol] == 2) {
            maze[pacmanRow][pacmanCol] = -1;
            score += 50;
            collectedPoints++;
        }

        if (collectedPoints >= totalPoints) {
            printf("Tous les points ont été collectés ! Fin du jeu.\n");
            running = 0;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        drawMaze(renderer);
        drawPoints(renderer);

        SDL_Rect pacmanRect = {pacman.x, pacman.y, CELL_SIZE, CELL_SIZE};
        SDL_RenderCopy(renderer, pacman.texture, NULL, &pacmanRect);

        for (int i = 0; i < ghostCount; i++) {
            SDL_Rect ghostRect = {ghosts[i].x, ghosts[i].y, CELL_SIZE, CELL_SIZE};
            SDL_RenderCopy(renderer, ghosts[i].texture, NULL, &ghostRect);
        }

        renderScore(renderer, font, score);
        checkCollisions(&pacman, ghosts, ghostCount, &running);
        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }

    cleanup_game(pacmanTexture, ghostTextures, renderer, window);
    TTF_CloseFont(font);
}
