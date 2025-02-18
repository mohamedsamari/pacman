
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h> // Ajouté pour malloc
#include <string.h> // Ajouté pour strcpy
#include "scores.h"
#include "graphic.h"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define MAX_JOUEURS 10 // Nombre maximum de joueurs

Joueur joueurs[MAX_JOUEURS]; // Définition des joueurs
int nombreJoueurs = 0; // 







void renderScorePage(SDL_Renderer *renderer, TTF_Font **fonts, SDL_Color yellow, SDL_Color black, SDL_Color white, SDL_Color hoverColor)
{
    // Afficher le titre
    renderText(renderer, "SCORES", fonts[0], yellow, SCREEN_WIDTH / 2 - 100, 50);

    // Charger les scores
    nombreJoueurs = chargerScores("scores.txt", joueurs, MAX_JOUEURS);
    if (nombreJoueurs == 0) {
        // Si aucun score n'est chargé, afficher un message
        renderText(renderer, "Aucun score disponible", fonts[1], white, SCREEN_WIDTH / 2 - 100, 100);
    } else {
        trierScores(joueurs, nombreJoueurs); // Trier les scores

        // Afficher le classement (3 premiers joueurs)
        afficherClassement(renderer, fonts[1], joueurs, nombreJoueurs, 3);
        
    }

    // Bouton BACK
    SDL_Rect rectBack = {150, 500, 500, 50};
    SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, 255);
    SDL_RenderFillRect(renderer, &rectBack);
    renderText(renderer, "BACK", fonts[1], black, SCREEN_WIDTH / 2 - 40, 510);
}



// Fonction pour gérer les événements de la page de score
int handleScorePageEvents(SDL_Event event)
{
    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        int mouseX = event.button.x;
        int mouseY = event.button.y;

        // Vérifier si le bouton "BACK" est cliqué
        if (mouseY >= 500 && mouseY <= 550)
        {
            return 0; // Retourner à la page d'accueil
        }
    }
    return 1; // Rester sur la page de score
}

// Fonction pour initialiser SDL et créer la fenêtre et le renderer
SDL_Renderer *initializeSDL(SDL_Window **window)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Erreur SDL : %s\n", SDL_GetError());
        return NULL;
    }

    if (TTF_Init() == -1)
    {
        printf("Erreur SDL_ttf : %s\n", TTF_GetError());
        SDL_Quit();
        return NULL;
    }

    *window = SDL_CreateWindow("Pac-Man - Page d'Accueil", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (*window == NULL)
    {
        printf("Erreur création fenêtre : %s\n", SDL_GetError());
        SDL_Quit();
        return NULL;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("Erreur création renderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return NULL;
    }

    return renderer;
}

// Fonction pour charger les polices
TTF_Font **loadFonts()
{
    TTF_Font **fonts = malloc(2 * sizeof(TTF_Font *));
    if (fonts == NULL)
    {
        printf("Erreur allocation mémoire pour les polices\n");
        return NULL;
    }

    fonts[0] = TTF_OpenFont("arial.ttf", 64); // Police pour le titre
    fonts[1] = TTF_OpenFont("arial.ttf", 32); // Police pour les boutons

    if (fonts[0] == NULL || fonts[1] == NULL)
    {
        printf("Erreur chargement police : %s\n", TTF_GetError());
        free(fonts);
        return NULL;
    }

    return fonts;
}

// Fonction pour afficher du texte
void renderText(SDL_Renderer *renderer, const char *text, TTF_Font *font, SDL_Color color, int x, int y)
{
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect textRect = {x, y, 0, 0};
    SDL_QueryTexture(texture, NULL, NULL, &textRect.w, &textRect.h);
    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    SDL_DestroyTexture(texture);
}

// Fonction pour afficher la page d'accueil
void renderMainMenu(SDL_Renderer *renderer, TTF_Font **fonts, SDL_Color yellow, SDL_Color black, SDL_Color white, SDL_Color hoverColor)
{
    // Afficher le titre
    renderText(renderer, "PAC-MAN", fonts[0], yellow, SCREEN_WIDTH / 2 - 150, 50);

    // Afficher les boutons
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    // Bouton START GAME
    SDL_Rect rectStartGame = {150, 200, 500, 50};
    if (mouseX >= rectStartGame.x && mouseX <= rectStartGame.x + rectStartGame.w &&
        mouseY >= rectStartGame.y && mouseY <= rectStartGame.y + rectStartGame.h)
    {
        SDL_SetRenderDrawColor(renderer, hoverColor.r, hoverColor.g, hoverColor.b, 255);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, 255);
    }
    SDL_RenderFillRect(renderer, &rectStartGame);
    renderText(renderer, "START GAME", fonts[1], black, 300, 210);

    // Bouton SCORE
    SDL_Rect rectScore = {150, 300, 500, 50};
    if (mouseX >= rectScore.x && mouseX <= rectScore.x + rectScore.w &&
        mouseY >= rectScore.y && mouseY <= rectScore.y + rectScore.h)
    {
        SDL_SetRenderDrawColor(renderer, hoverColor.r, hoverColor.g, hoverColor.b, 255);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, 255);
    }
    SDL_RenderFillRect(renderer, &rectScore);
    renderText(renderer, "SCORE", fonts[1], black, 340, 310);
}

// Fonction pour afficher la page de sélection de difficulté
void renderDifficultySelection(SDL_Renderer *renderer, TTF_Font **fonts, SDL_Color yellow, SDL_Color black, SDL_Color white, SDL_Color hoverColor)
{
    // Afficher le titre
    renderText(renderer, "CHOOSE DIFFICULTY", fonts[0], yellow, SCREEN_WIDTH / 2 - 280, 50);

    // Afficher les boutons
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    // Bouton EASY
    SDL_Rect rectEasy = {150, 200, 500, 50};
    if (mouseX >= rectEasy.x && mouseX <= rectEasy.x + rectEasy.w &&
        mouseY >= rectEasy.y && mouseY <= rectEasy.y + rectEasy.h)
    {
        SDL_SetRenderDrawColor(renderer, hoverColor.r, hoverColor.g, hoverColor.b, 255);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, 255);
    }
    SDL_RenderFillRect(renderer, &rectEasy);
    renderText(renderer, "EASY", fonts[1], black, 340, 210);

    // Bouton MEDIUM
    SDL_Rect rectMedium = {150, 300, 500, 50};
    if (mouseX >= rectMedium.x && mouseX <= rectMedium.x + rectMedium.w &&
        mouseY >= rectMedium.y && mouseY <= rectMedium.y + rectMedium.h)
    {
        SDL_SetRenderDrawColor(renderer, hoverColor.r, hoverColor.g, hoverColor.b, 255);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, 255);
    }
    SDL_RenderFillRect(renderer, &rectMedium);
    renderText(renderer, "MEDIUM", fonts[1], black, 330, 310);

    // Bouton HARD
    SDL_Rect rectHard = {150, 400, 500, 50};
    if (mouseX >= rectHard.x && mouseX <= rectHard.x + rectHard.w &&
        mouseY >= rectHard.y && mouseY <= rectHard.y + rectHard.h)
    {
        SDL_SetRenderDrawColor(renderer, hoverColor.r, hoverColor.g, hoverColor.b, 255);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, 255);
    }
    SDL_RenderFillRect(renderer, &rectHard);
    renderText(renderer, "HARD", fonts[1], black, 340, 410);
}

// Fonction pour gérer les événements de la page d'accueil
int handleMainMenuEvents(SDL_Event event, SDL_Rect rectStartGame, SDL_Rect rectScore)
{
    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        int mouseX = event.button.x;
        int mouseY = event.button.y;

        // Vérifier si le bouton "START GAME" est cliqué
        if (mouseX >= rectStartGame.x && mouseX <= rectStartGame.x + rectStartGame.w &&
            mouseY >= rectStartGame.y && mouseY <= rectStartGame.y + rectStartGame.h)
        {
            return 1; // Afficher la page de sélection de difficulté
        }

        // Vérifier si le bouton "SCORE" est cliqué
        if (mouseX >= rectScore.x && mouseX <= rectScore.x + rectScore.w &&
            mouseY >= rectScore.y && mouseY <= rectScore.y + rectScore.h)
        {
            printf("Score button clicked!\n");
            return 2; // Indique que la page de score doit être affichée
        }
    }
    return 0; // Rester sur la page d'accueil
}

// Fonction pour gérer les événements de la page de sélection de difficulté
const char* handleDifficultySelectionEvents(SDL_Event event)
{
    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        int mouseX = event.button.x;
        int mouseY = event.button.y;

        // Vérifier si le bouton "EASY" est cliqué
        if (mouseY >= 200 && mouseY <= 250)
        {
            printf("EASY difficulty selected!\n");
            return "easy"; // Retourner la difficulté
        }
        // Vérifier si le bouton "MEDIUM" est cliqué
        else if (mouseY >= 300 && mouseY <= 350)
        {
            printf("MEDIUM difficulty selected!\n");
            return "medium"; // Retourner la difficulté
        }
        // Vérifier si le bouton "HARD" est cliqué
        else if (mouseY >= 400 && mouseY <= 450)
        {
            printf("HARD difficulty selected!\n");
            return "hard"; // Retourner la difficulté
        }
    }
    return NULL; // Aucun choix
}

// Fonction pour libérer les ressources
void cleanup(SDL_Window *window, SDL_Renderer *renderer, TTF_Font **fonts)
{
    if (fonts != NULL)
    {
        TTF_CloseFont(fonts[0]);
        TTF_CloseFont(fonts[1]);
        free(fonts);
    }
    if (renderer != NULL)
    {
        SDL_DestroyRenderer(renderer);
    }
    if (window != NULL)
    {
        SDL_DestroyWindow(window);
    }
    TTF_Quit();
    SDL_Quit();
}
