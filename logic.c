#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL_ttf.h>

#define SCREEN_WIDTH 880
#define SCREEN_HEIGHT 440
#define CELL_SIZE 40

// Labyrinthe (1 = mur, 0 = espace, 2 = espace spécial pour grands points)
int maze[11][22] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 1, 0, 1, 2, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
    {1, 2, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

// Structures pour Pac-Man et les fantômes
struct Entity {
    int x, y; // Position en pixels
    int dx, dy; // Direction
    SDL_Texture *texture; // Texture SDL
};



int isValidPosition(int x, int y) {
    int row = y / CELL_SIZE;
    int col = x / CELL_SIZE;
    return (maze[row][col] != 1); // Retourne 1 si la position est valide
}
int setGhostCount(const char *difficulty) {
    if (strcmp(difficulty, "easy") == 0) {
        return 2; // 2 ghosts
    } else if (strcmp(difficulty, "medium") == 0) {
        return 3; // 3 ghosts
    } else if (strcmp(difficulty, "hard") == 0) {
        return 4; // 4 ghosts
    }
    return 0; // Default case
}
int countTotalPoints() {
    int totalPoints = 0;
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 22; j++) {
            if (maze[i][j] == 0 || maze[i][j] == 2) { // Count small and large points
                totalPoints++;
            }
        }
    }
    return totalPoints;
}

// Fonction pour dessiner les points
void drawPoints(SDL_Renderer *renderer) {
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 22; j++) {
            if (maze[i][j] == 0) { // Petits points blancs
                SDL_Rect point = {j * CELL_SIZE + CELL_SIZE / 3, i * CELL_SIZE + CELL_SIZE / 3, CELL_SIZE / 4, CELL_SIZE / 4};
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &point);
            } else if (maze[i][j] == 2) { // Grands points rouges
                SDL_Rect point = {j * CELL_SIZE + CELL_SIZE / 4, i * CELL_SIZE + CELL_SIZE / 4, CELL_SIZE / 2, CELL_SIZE / 2};
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderFillRect(renderer, &point);
            }
        }
    }
}

// Fonction pour dessiner le labyrinthe
void drawMaze(SDL_Renderer *renderer) {
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 22; j++) {
            if (maze[i][j] == 1) { // Murs bleus
                SDL_Rect wall = {j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE};
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                SDL_RenderFillRect(renderer, &wall);
            }
        }
    }
}

// Fonction pour rendre le score
void renderScore(SDL_Renderer *renderer, TTF_Font *font, int score) {
    char scoreText[20];
    sprintf(scoreText, "Score: %d", score);

    SDL_Color textColor = {255, 255, 255}; // Couleur blanche
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, scoreText, textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect textRect;
    textRect.x = 10; // Position X
    textRect.y = 10; // Position Y
    textRect.w = textSurface->w; // Largeur du texte
    textRect.h = textSurface->h; // Hauteur du texte

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    // Libérer la surface et la texture
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

// Fonction pour charger une texture
SDL_Texture* loadTexture(SDL_Renderer *renderer, const char *filePath) {
    SDL_Surface *surface = SDL_LoadBMP(filePath);
    if (!surface) {
        printf("Erreur chargement texture : %s\n", SDL_GetError());
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

// Fonction pour mettre à jour les fantômes
void updateGhosts(struct Entity ghosts[], int ghostCount) {
    for (int i = 0; i < ghostCount; i++) {
        if (isValidPosition(ghosts[i].x + ghosts[i].dx * CELL_SIZE, ghosts[i].y + ghosts[i].dy * CELL_SIZE)) {
            ghosts[i].x += ghosts[i].dx * CELL_SIZE;
            ghosts[i].y += ghosts[i].dy * CELL_SIZE;
        } else {
            // Changement de direction aléatoire
            int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
            int newDir = rand() % 4;
            ghosts[i].dx = directions[newDir][0];
            ghosts[i].dy = directions[newDir][1];

            // Vérifiez si la nouvelle direction est valide avant de déplacer
            if (isValidPosition(ghosts[i].x + ghosts[i].dx * CELL_SIZE, ghosts[i].y + ghosts[i].dy * CELL_SIZE)) {
                ghosts[i].x += ghosts[i].dx * CELL_SIZE;
                ghosts[i].y += ghosts[i].dy * CELL_SIZE;
            }
        }
    }
}

// Fonction pour gérer le mouvement de Pac-Man
void handlePacmanMovement(struct Entity *pacman) {
    // Gestion des événements pour le mouvement de Pac-Man
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            exit(0);
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    if (isValidPosition(pacman->x, pacman->y - CELL_SIZE)) {
                        pacman->dx = 0;
                        pacman->dy = -1;
                    }
                    break;
                case SDLK_DOWN:
                    if (isValidPosition(pacman->x, pacman->y + CELL_SIZE)) {
                        pacman->dx = 0;
                        pacman->dy = 1;
                    }
                    break;
                case SDLK_LEFT:
                    if (isValidPosition(pacman->x - CELL_SIZE, pacman->y)) {
                        pacman->dx = -1;
                        pacman->dy = 0;
                    }
                    break;
                case SDLK_RIGHT:
                    if (isValidPosition(pacman->x + CELL_SIZE, pacman->y)) {
                        pacman->dx = 1;
                        pacman->dy = 0;
                    }
                    break;
            }
        }
    }

    // Mise à jour des positions de Pac-Man
    if (isValidPosition(pacman->x + pacman->dx * CELL_SIZE, pacman->y + pacman->dy * CELL_SIZE)) {
        pacman->x += pacman->dx * CELL_SIZE;
        pacman->y += pacman->dy * CELL_SIZE;
    }
}

// Fonction pour vérifier les collisions
void checkCollisions(struct Entity *pacman, struct Entity ghosts[], int ghostCount, int *running) {
    for (int i = 0; i < ghostCount; i++) {
        if (pacman->x == ghosts[i].x && pacman->y == ghosts[i].y) {
            printf("Collision détectée ! Fin du jeu.\n");
            *running = 0;
        }
    }
}

// Fonction pour nettoyer les ressources
void cleanup_game(SDL_Texture *pacmanTexture, SDL_Texture *ghostTextures[], SDL_Renderer *renderer, SDL_Window *window) {
    SDL_DestroyTexture(pacmanTexture);
    for (int i = 0; i < 4; i++) {
        SDL_DestroyTexture(ghostTextures[i]);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}