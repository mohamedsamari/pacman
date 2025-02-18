#include "scores.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sauvegarderScore(const char *nomFichier, const Joueur *joueur) {
    FILE *fichier = fopen(nomFichier, "a");
    if (fichier == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier %s\n", nomFichier);
        return;
    }
    fprintf(fichier, "%s %d\n", joueur->nom, joueur->score);
    fclose(fichier);
}

int chargerScores(const char *nomFichier, Joueur *joueurs, int tailleMax) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        return 0;
    }

    int i = 0;
    while (i < tailleMax && fscanf(fichier, "%s %d", joueurs[i].nom, &joueurs[i].score) == 2) {
        i++;
    }
    fclose(fichier);
    return i;
}

void trierScores(Joueur *joueurs, int taille) {
    for (int i = 0; i < taille - 1; i++) {
        for (int j = i + 1; j < taille; j++) {
            if (joueurs[i].score < joueurs[j].score) {
                Joueur temp = joueurs[i];
                joueurs[i] = joueurs[j];
                joueurs[j] = temp;
            }
        }
    }
}

void afficherClassement(SDL_Renderer *renderer, TTF_Font *font, Joueur *joueurs, int taille, int maxAffichage) {
    SDL_Color color = {255, 255, 255};
    int y = 50;

    // Afficher seulement les `maxAffichage` premiers joueurs
    for (int i = 0; i < taille && i < maxAffichage; i++) {
        char texte[100];
        snprintf(texte, sizeof(texte), "%d. %s - %d points", i + 1, joueurs[i].nom, joueurs[i].score);

        SDL_Surface *surface = TTF_RenderText_Solid(font, texte, color);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

        SDL_Rect destRect = {50, y, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &destRect);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        y += 40;
    }
}

