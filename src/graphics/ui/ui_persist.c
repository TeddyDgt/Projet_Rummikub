#include "ui_persist.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PLAYERS_FILE "players.txt"

static void trim_newline(char *s) {
    if (!s) {
        return;
    }
    size_t len = strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r')) {
        s[len - 1] = '\0';
        len--;
    }
}

/**
 * @brief Charge la configuration des joueurs depuis un fichier texte.
 * Lit le nombre de joueurs, leurs noms et si ce sont des IA.
 * @param names Tableau de chaînes pour stocker les noms.
 * @param is_ai Tableau de booléens pour le statut IA.
 * @param count Pointeur pour stocker le nombre de joueurs chargés.
 * @return true si le fichier a été lu avec succès, false sinon.
 */
bool ui_persist_load_players(char names[4][16], bool is_ai[4], int *count) {
    FILE *f = fopen(PLAYERS_FILE, "r");
    if (!f) {
        return false;
    }

    char line[64] = {0};
    if (!fgets(line, sizeof(line), f)) {
        fclose(f);
        return false;
    }
    int loaded = 0;
    if (sscanf(line, "%d", &loaded) != 1 || loaded < 2 || loaded > 4) {
        fclose(f);
        return false;
    }

    for (int i = 0; i < loaded; i++) {
        if (!fgets(line, sizeof(line), f)) {
            break;
        }
        trim_newline(line);
        char *comma = strchr(line, ',');
        int ai_flag = 0;
        if (comma) {
            *comma = '\0';
            comma++;
            ai_flag = (int)strtol(comma, NULL, 10);
        }
        strncpy(names[i], line, 15);
        names[i][15] = '\0';
        is_ai[i] = ai_flag ? true : false;
        loaded = i + 1;
    }

    if (count) {
        *count = loaded;
    }
    fclose(f);
    return true;
}

/**
 * @brief Sauvegarde la configuration actuelle des joueurs dans un fichier.
 * @param names Noms des joueurs.
 * @param is_ai Statuts IA.
 * @param count Nombre de joueurs à sauvegarder.
 */
void ui_persist_save_players(const char names[4][16], const bool is_ai[4], int count) {
    FILE *f = fopen(PLAYERS_FILE, "w");
    if (!f) {
        return;
    }
    if (count < 2) {
        count = 2;
    }
    if (count > 4) {
        count = 4;
    }

    fprintf(f, "%d\n", count);
    for (int i = 0; i < count; i++) {
        const char *name = names[i][0] ? names[i] : "Joueur";
        fprintf(f, "%s,%d\n", name, is_ai[i] ? 1 : 0);
    }
    fclose(f);
}
