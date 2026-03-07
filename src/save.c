#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "save.h"

// Fonction utilitaire pour générer le chemin dans AppData
void ObtenirCheminSauvegarde(char* cheminComplet, const char* nomFichier) {
    // getenv("APPDATA") récupère le chemin "C:\Users\TonNom\AppData\Roaming"
    const char* appData = getenv("APPDATA"); 
    
    if (appData != NULL) {
        // On crée un fichier unique pour ton jeu directement dans AppData
        snprintf(cheminComplet, 512, "%s\\LightsOut_%s", appData, nomFichier);
    } else {
        // Sécurité si l'ordinateur a un bug avec AppData
        strcpy(cheminComplet, nomFichier);
    }
}

int SaveGame(const char* filepath, SaveData data) {
    char cheminComplet[512];
    ObtenirCheminSauvegarde(cheminComplet, filepath);

    FILE* file = fopen(cheminComplet, "wb"); 

    if (file == NULL) {
        printf("Erreur : Impossible de créer le fichier de sauvegarde : %s\n", cheminComplet);
        return 0; 
    }

    size_t written = fwrite(&data, sizeof(SaveData), 1, file); 
    fclose(file);

    if (written == 1) {
        printf("Sauvegarde réussie dans : %s\n", cheminComplet);
        return 1; 
    } else {
        printf("Erreur lors de l'écriture des données.\n");
        return 0; 
    }
}

int LoadGame(const char* filepath, SaveData* data) {
    char cheminComplet[512];
    ObtenirCheminSauvegarde(cheminComplet, filepath);

    FILE* file = fopen(cheminComplet, "rb"); 

    if (file == NULL) {
        printf("Save introuvable : %s\n", cheminComplet);
        return 0; 
    }

    // On lit les données dans la structure
    size_t nb_lus = fread(data, sizeof(SaveData), 1, file);
    fclose(file);

    if (nb_lus == 1) {
        // --- LA VÉRIFICATION MAGIQUE ---
        // Si le nombre n'est pas 12345, c'est que le fichier est ancien ou corrompu
        if (data->magic != 12345) {
            printf("Erreur : Sauvegarde corrompue ou version incompatible !\n");
            return 0; // On refuse de charger pour éviter les bugs
        }
        
        printf("Chargement réussi depuis : %s\n", cheminComplet);
        return 1; 
    } else {
        printf("Erreur de lecture : fichier vide ou illisible\n");
        return 0; 
    }
}