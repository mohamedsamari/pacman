#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h> // Ajouté pour malloc
#include <string.h> // Ajouté pour strcpy
#include <time.h>
#include "logic.h"

#include "scores.h"
#include "graphic.h"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define GAME_WIDTH 850
#define GAME_HEIGHT 440


#define MAX_JOUEURS 10 // Nombre maximum de joueurs
#define CELL_SIZE 40



void startGame();
// Fonction principale
// Fonction principale
int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = initializeSDL(&window);
    if (renderer == NULL)
    {
        printf("Erreur lors de l'initialisation de SDL : %s\n", SDL_GetError());
        return 1;
    }

    TTF_Font **fonts = loadFonts();
    if (fonts == NULL)
    {
        cleanup(window, renderer, fonts);
        printf("Erreur lors du chargement des polices.\n");
        return 1;
    }

    // Définir les couleurs
    SDL_Color yellow = {255, 255, 0};
    SDL_Color black = {0, 0, 0};
    SDL_Color white = {255, 255, 255};
    SDL_Color hoverColor = {200, 200, 200};

    // Définir les rectangles des boutons de la page d'accueil
    SDL_Rect rectStartGame = {150, 200, 500, 50};
    SDL_Rect rectScore = {150, 300, 500, 50};

    int running = 1;
    int showDifficultySelection = 0; // Contrôle l'affichage de la sélection de difficulté
    int showScorePage = 0; // Contrôle l'affichage de la page de score
    int gameStarted = 0;
    SDL_Event event;
    char selectedDifficulty[10];

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
            else if (!showDifficultySelection && !showScorePage)
            {
                int result = handleMainMenuEvents(event, rectStartGame, rectScore);
                if (result == 1)
                {
                    showDifficultySelection = 1; // Afficher la page de sélection de difficulté
                }
                else if (result == 2)
                {
                    showScorePage = 1; // Afficher la page de score
                }
            }
            else if (showScorePage)
            {
                showScorePage = handleScorePageEvents(event);
            }
            else
            {
                const char *difficulty = handleDifficultySelectionEvents(event);
                if (difficulty != NULL) {
                    strcpy(selectedDifficulty, difficulty); // Copier la difficulté sélectionnée
                    gameStarted = 1; // Indiquer que le jeu doit commencer
                    showDifficultySelection = 0; // Masquer la sélection de difficulté
                }
            }
        }

        // Effacer l'écran
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (!showDifficultySelection && !showScorePage)
        {
            renderMainMenu(renderer, fonts, yellow, black, white, hoverColor);
        }
        else if (showScorePage)
        {
            renderScorePage(renderer, fonts, yellow, black, white, hoverColor);
        }
        else
        {
            renderDifficultySelection(renderer, fonts, yellow, black, white, hoverColor);
        }

        if (gameStarted) {
            startGame(selectedDifficulty); // Passer la difficulté sélectionnée
            gameStarted = 0; // Réinitialiser le drapeau après le démarrage du jeu
        }

        // Mettre à jour l'écran
        SDL_RenderPresent(renderer);
    }

    // Nettoyer et quitter
    cleanup(window, renderer, fonts);

    // Pause pour voir les messages d'erreur
    printf("Appuyez sur une touche pour quitter...\n");
    getchar(); // Attendre une entrée de l'utilisateur

    return 0;
}



void startGame(const char *difficulty) {
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
    SDL_Window *gameWindow = SDL_CreateWindow("Pac-Man", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GAME_WIDTH, GAME_HEIGHT, SDL_WINDOW_SHOWN);

    if (!gameWindow) {
        printf("Erreur création fenêtre : %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erreur création renderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(gameWindow);
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
        cleanup_game(pacmanTexture, ghostTextures, renderer, gameWindow);
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

    // Utilisation de la difficulté
    int ghostCount = setGhostCount(difficulty);
    if (ghostCount == 0) {
        printf("Niveau de difficulté invalide. Par défaut à facile.\n");
        ghostCount = 2; // Valeur par défaut
    }

    for (int i = 0; i < ghostCount; i++) {
        ghosts[i] = (struct Entity){(i + 5) * CELL_SIZE, 5 * CELL_SIZE, 1, 0, ghostTextures[i]};
    }

    char nomJoueur[20];
    const char *prenoms[] = {"John", "Jane", "Bob", "Alice", "Tom", "Emily", "Jack", "Lily"};
    sprintf(nomJoueur, "%s", prenoms[rand() % (sizeof(prenoms) / sizeof(prenoms[0]))]);

    // Boucle principale
    int running = 1;
    int score = 0;
    TTF_Font *font = TTF_OpenFont("arial.ttf", 24);
    if (!font) {
        printf("Erreur de chargement de la police: %s\n", TTF_GetError());
        cleanup_game(pacmanTexture, ghostTextures, renderer, gameWindow);
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

         if (!running) {
            printf("Fin du jeu. Score final : %d\n", score);

            Joueur joueur = {0}; // Initialisation
            strcpy(joueur.nom, nomJoueur); // Copier le nom
            joueur.score = score; // Assigner le score

    // Sauvegarder le score dans le fichier scores.txt
    sauvegarderScore("scores.txt", &joueur);
        }
    }

    

    cleanup_game(pacmanTexture, ghostTextures, renderer, gameWindow);
    TTF_CloseFont(font);
}
