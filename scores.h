#ifndef SCORES_H
#define SCORES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
    char nom[50];
    int score;
} Joueur;

// Prototypes des fonctions
void sauvegarderScore(const char *nomFichier, const Joueur *joueur);
int chargerScores(const char *nomFichier, Joueur *joueurs, int tailleMax);
void trierScores(Joueur *joueurs, int taille);
void afficherClassement(SDL_Renderer *renderer, TTF_Font *font, Joueur *joueurs, int taille, int maxAffichage);

#endif

