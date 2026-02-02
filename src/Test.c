#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Combinaisons.h"
#include "Draw.h"
#include "GameLoop.h"
#include "Players.h"
#include "structs.h"

/**
 * @brief Function main.
 *
 * @return Result value.
 */
/**
 * @brief Function main.
 *
 * @return Result value.
 */
int main(void) {
  // 1. INITIALISATION DE L'ALÉATOIRE (Fixe le problème n°1)
  srand(time(NULL));

  Deck stack;
  Table table;
  Player players[2];
  int num_players = 2;
  char input[256];

  // 2. INITIALISATION DU JEU
  init_deck(&stack);
  // Initialisation de la table (count = 0)
  table.count = 0;

  // Initialisation des joueurs (Fixe le problème n°2 des IDs fantômes)
  // Note : init_player DOIT faire le malloc de p->hand (voir Players.c)
  init_player(&players[0], "Alice");
  init_player(&players[1], "Bob");

  // 3. PRÉPARATION
  int current = determine_first_player(&stack, num_players);
  printf("\n=== DEBUT DE LA PARTIE ===\n");
  printf("Le joueur %s tire la plus grosse tuile et commence !\n",
         players[current].name);

  distribute_initial_tiles(&stack, players, num_players);

  // 4. BOUCLE DE JEU
  int game_running = 1;
  while (game_running && !is_game_over(players, num_players, &stack)) {
    Player *p = &players[current];

    printf("\n======================================================\n");
    printf("TOUR DE : %s | Score : %d\n", p->name, p->score);
    printf("Tuiles en pioche : %d\n", stack.top);
    printf("------------------------------------------------------\n");

    // Affichage de la main avec IDs
    printf("Main de %s :\n", p->name);
    for (int i = 0; i < p->hand_count; i++) {
      printf("[%d]:%d%c  ", p->hand[i].id, p->hand[i].value,
             (p->hand[i].color == NOIR    ? 'N'
              : p->hand[i].color == ROUGE ? 'R'
              : p->hand[i].color == BLEU  ? 'B'
                                          : 'J'));
      if ((i + 1) % 7 == 0)
        printf("\n");
    }

    printf("\n\nActions possibles :\n");
    printf("  p        : Piocher une tuile et finir le tour\n");
    printf("  sc       : Trier par Couleur\n");
    printf("  sv       : Trier par Valeur\n");
    printf("  j ID1 ID2... : Jouer une combinaison (ex: j 5 12 18)\n");
    printf("> ");

    // Lecture sécurisée de l'entrée
    if (!fgets(input, sizeof(input), stdin))
      break;
    input[strcspn(input, "\n")] = 0; // On enlève le retour à la ligne

    if (strcmp(input, "p") == 0) {
      // PIOCHE
      Tile t = draw_tile(&stack);
      add_tile_to_player(p, t);
      printf("Tu as pioché la tuile %d. Fin du tour.\n", t.value);
      current = (current + 1) % num_players; // Tour suivant
    } else if (strcmp(input, "sc") == 0) {
      // TRI PAR COULEUR (Ta signature : by_color = 1)
      sort_player_hand(p, 1);
    } else if (strcmp(input, "sv") == 0) {
      // TRI PAR VALEUR (Ta signature : by_color = 0)
      sort_player_hand(p, 0);
    } else if (input[0] == 'j') {
      // 1. Préparer une combinaison temporaire
      Combinaison c;
      c.tiles = malloc(sizeof(Tile) * p->hand_count);
      c.count = 0;

      // 2. Extraire les IDs de la chaîne (on saute le 'j' et l'espace)
      char *token = strtok(input + 1, " ");
      while (token != NULL) {
        int id_recherche = atoi(token);

        // Chercher la tuile dans la main du joueur
        Tile t = find_tile(p, id_recherche);
        if (t.id != -1) {
          c.tiles[c.count++] = t;
        } else {
          printf("Erreur : La tuile ID %d n'est pas dans votre main !\n",
                 id_recherche);
        }
        token = strtok(NULL, " ");
      }

      // 3. Valider la combinaison
      if (c.count >= 3 && is_valid_combination(&c)) {
        printf("Combinaison VALIDE (%s) !\n",
               (c.type == IS_SUITE ? "Suite" : "Groupe"));

        // Ajouter à la table (il faut copier les tuiles car c.tiles est
        // temporaire)
        add_to_table(&table, c);

        // Enlever les tuiles de la main du joueur
        for (int i = 0; i < c.count; i++) {
          remove_tile_from_hand(p, c.tiles[i].id);
        }

        printf("Tuiles posées sur la table. Vous pouvez rejouer ou taper 'f' "
               "pour finir.\n");
      } else {
        printf(
            "Combinaison INVALIDE (trop courte ou règles non respectées) !\n");
      }
      free(c.tiles); // On libère la mémoire temporaire
    } else if (strcmp(input, "f") == 0) {
      // ACTION FINIR : on passe au joueur suivant
      printf("Fin du tour de %s.\n", p->name);
      current = (current + 1) % num_players;
    } else if (strcmp(input, "quit") == 0) {
      game_running = 0;
    }
  }

  // 5. FIN DE PARTIE ET NETTOYAGE
  calculate_final_scores(players, num_players);

  // Libération de la mémoire (malloc dans init_player -> free ici)
  free_players(players, num_players);

  printf("\nPartie terminée. Merci d'avoir joué !\n");
  return 0;
}