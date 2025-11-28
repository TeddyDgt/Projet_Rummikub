<p align="right">
  <img src="/docs/img/logo_isty.png" alt="Logo ISTY" width="180"/>
</p>

# Rapport d’analyse et de conception

## Projet Algorithmique 2025/2026 – Développement du jeu Rummikub en C

### Groupe ... – IATIC3

**Membres de l’équipe :**

- BENBACHA Kamar
- CHOISY Alexis
- DA SILVA FERREIRA Lucas
- DEGAT Teddy
- FOURNIE Baptiste

**Encadrant :** ABOUDA Dhekra
**Date de remise :** 30 novembre 2025

---

## Sommaire

- [Rapport d’analyse et de conception](#rapport-danalyse-et-de-conception)
  - [Projet Algorithmique 2025/2026 – Développement du jeu Rummikub en C](#projet-algorithmique-20252026--développement-du-jeu-rummikub-en-c)
    - [Groupe ... – IATIC3](#groupe----iatic3)
  - [Sommaire](#sommaire)
  - [I. Analyse des besoins de l’utilisateur](#i-analyse-des-besoins-de-lutilisateur)
    - [1. Contexte du projet](#1-contexte-du-projet)
    - [2. Objectifs fonctionnels](#2-objectifs-fonctionnels)
    - [3. Public visé](#3-public-visé)
    - [4. Contraintes](#4-contraintes)
  - [II. Définition du système à réaliser](#ii-définition-du-système-à-réaliser)
    - [1. Fonctionnement général](#1-fonctionnement-général)
  - [III. Cahier des charges](#iii-cahier-des-charges)
    - [1. Description globale des fonctions](#1-description-globale-des-fonctions)
    - [2. Fonctions bonus](#2-fonctions-bonus)
  - [IV. Structures de données prévues](#iv-structures-de-données-prévues)
  - [V. Liste des fonctions principales](#v-liste-des-fonctions-principales)
  - [VI. Répartition des tâches](#vi-répartition-des-tâches)
  - [VII. Conclusion](#vii-conclusion)

---

## I. Analyse des besoins de l’utilisateur

### 1. Contexte du projet

Le projet s’inscrit dans un projet de première année d'école d'ingénieur (IATIC3).  
Il consiste à développer en langage C une version fonctionnelle et jouable du jeu Rummikub, avec une interface graphique interactive et la possibilité de jouer à plusieurs, y compris contre l’ordinateur.

### 2. Objectifs fonctionnels

Le programme doit permettre :

- de jouer à partir d’une interface graphique
- d’entrer et de sauvegarder les pseudos des joueurs
- de jouer à plusieurs joueurs humains (2 à 4)
- de jouer contre un ou plusieurs ordinateurs
- de mémoriser les scores dans un fichier

Des fonctionnalités bonus peuvent être ajoutées :

- chronomètre limitant la durée d’un tour à une minute ;
- IA stratégique (plutôt que purement aléatoire) ;
- animations ou effets visuels.

### 3. Public visé

L’application vise deux publics :

1. Les joueurs, qui utilisent le programme comme jeu complet.
2. L’encadrant, qui évalue la conception algorithmique, la clarté du code et la compréhension globale.

### 4. Contraintes

- Langage imposé : C uniquement
- Interface graphique obligatoire (bibliothèque libre : GTK, SDL2, raylib, etc.)
- Groupe de 5 étudiants
- Date limite du rapport d’analyse : 30 novembre 2025
- Soutenance prévue le 30 janvier 2026

---

## II. Définition du système à réaliser

### 1. Fonctionnement général

Le système est un programme interactif simulant le jeu Rummikub.  
Il comprend trois parties principales :

1. Le moteur de jeu : logique, règles, tours, vérifications
2. L’interface graphique : affichage du plateau, tuiles, menus
3. Les modules de persistance : sauvegarde des scores, pseudos, etc.

Lorsqu’on lance le programme, le menu principal permet :

- de démarrer une partie (2 à 4 joueurs, humains ou IA)
- d’afficher les scores précédents
- ou de quitter

Le jeu se déroule en tours successifs :

- tirage aléatoire de 14 tuiles par joueur
- détermination de l’ordre de jeu
- gestion des actions : poser, piocher, réorganiser, vérifier
- calcul des scores à la fin de la partie

La partie se termine quand un joueur n’a plus de tuiles ou que la pioche est vide.

---

## III. Cahier des charges

### 1. Description globale des fonctions

| Fonctionnalité                | Description                                                        |
| ----------------------------- | ------------------------------------------------------------------ |
| Lancement du jeu              | Affichage d’un menu principal : _Jouer_, _Scores_, _Quitter_       |
| Saisie des joueurs            | Enregistrement des pseudos dans un fichier                         |
| Création du plateau           | Génération des 106 tuiles (104 + 2 jokers)                         |
| Distribution des tuiles       | Donne 14 tuiles par joueur                                         |
| Tour de jeu                   | Alternance entre joueurs, avec vérification automatique des règles |
| Vérification des combinaisons | Validation des séries et séquences posées sur la table             |
| Gestion du joker              | Règles spécifiques de remplacement et d’utilisation immédiate      |
| Calcul des points             | Détermination des scores positifs et négatifs                      |
| Fin de partie                 | Détection du gagnant et mise à jour du fichier de scores           |
| Jeu contre IA                 | L’ordinateur joue automatiquement (aléatoire ou stratégie simple)  |

### 2. Fonctions bonus

- Limite de 60 secondes par tour
- Animation graphique des tuiles
- Sauvegarde et reprise de partie

---

## IV. Structures de données prévues

Les structures principales modélisent les tuiles, les joueurs et la partie
Elles doivent être simples, dynamiques et réutilisables dans les modules du projet

![conception](img/conception.jpg)
Description fonctionnelle des modèles de données :

- **Constantes** : `IS_SUITE` et `IS_GROUPE` sont des valeurs définies pour qualifier le type d'une combinaison.
- La structure **Tile** représente une tuile, définie par une valeur de 1 à 13 (13 étant un joker) et une couleur de 1 à 4.
- La **Draw** (Pioche) correspond à une liste de `Tuiles` disponible pour les joueurs.
- La structure **Combinaison** représente un ensemble de `Tuiles` et possède un champ `type`.
  - **Groupe** et **Suite** ne sont pas des structures distinctes : ce sont des `Combinaison` dont le champ `type` vaut respectivement `IS_GROUPE` ou `IS_SUITE`.
- La structure **Player** représente un joueur. Il possède un nom d'utilisateur, un score ainsi qu'une main (liste de `Tuiles`).
- Le **Board** n'est pas une structure en soi, mais désigne la liste des `Combinaison` actuellement posées sur le jeu.

---

## V. Liste des fonctions principales

Nous avons imaginé ces fonctions pour le bon fonctionnement du code :

```C
int is_valid_group(Combinaison *combinaison)
```

- **But** : Vérifie si une combinaison est un Groupe valide. Elle doit s'assurer que toutes les tuiles ont la même valeur, que les couleurs sont différentes et qu'il y a au moins 3 tuiles.

- **Retour** : 1 (Vrai) si valide, 0 (Faux) sinon.

```C
int is_valid_suite(Combinaison *combinaison)
```

- **But** : Vérifie si une combinaison est une Suite valide. Elle doit s'assurer que toutes les tuiles sont de la même couleur, que les valeurs se suivent (ex: 3, 4, 5) et qu'il y a au moins 3 tuiles.

- **Retour** : 1 (Vrai) si valide, 0 (Faux) sinon.

```C
Tile draw_tile(Tile *draw)
```

- **But** : Simule l'action de piocher. Sélectionne une tuile aléatoire dans la liste deck (la pioche), la retire du tableau, décrémente deck_size et retourne la tuile piochée.

- **Retour** : La tuile tiré aléatoirement dans la pioche

```C
void player_play_combinaison(Player *player, Combinaison *combinaison, Combinaison **board, int *board_size)
```

- **But** : Gère le tour de jeu où un joueur pose une combinaison complète.

- **Logique** :

  1. Vérifie si le joueur possède bien les tuiles dans sa main.

  2. Vérifie la validité de la combinaison (via `is_valid_group` ou `is_valid_suite`).

  3. Retire les tuiles de la main du joueur.

  4. Ajoute la combinaison au plateau et incrémente la taille du plateau.

---

## VI. Répartition des tâches

Pour mener à bien ce projet dans les délais impartis, nous avons divisé l'équipe en pôles de compétences. Chaque binôme ou membre est responsable d'une partie du code, mais la communication reste constante pour faciliter l'intégration finale.

### 1. Pôle "Moteur de Jeu & Logique" (Backend)

**Membres :** BENBACHA Kamar, CHOISY Alexis

Ce pôle est chargé de développer le cœur du programme en langage C pur (indépendant de l'affichage).

- **Définition des structures :** Implémentation de `Tile`, `Player`, `Combinaison`.
- **Algorithmes de validation :** Développement des fonctions critiques `is_valid_group` et `is_valid_suite`.
- **Gestion de la partie :** Fonctions de distribution (`draw_tile`), gestion des tours et calcul des scores.
- **Tests unitaires :** Vérification des règles via une interface console provisoire.

### 2. Pôle "Interface Graphique" (Frontend)

**Membres :** DA SILVA FERREIRA Lucas, DEGAT Teddy

Ce pôle s'occupe de la bibliothèque graphique (SDL2, Raylib ou GTK) et de l'expérience utilisateur.

- **Affichage :** Rendu du plateau (`Board`), de la main du joueur et des menus.
- **Gestion des événements :** Clics souris, sélection des tuiles, boutons "Valider" ou "Piocher".
- **Ressources :** Création ou intégration des images (sprites des tuiles) et animations simples.
- **Liaison :** Appel des fonctions du moteur de jeu lors des actions graphiques.

### 3. Pôle "IA & Persistance des données"

**Membre :** FOURNIE Baptiste

Ce pôle gère les fonctionnalités annexes et l'intelligence artificielle.

- **Gestion des fichiers :** Sauvegarde et chargement des profils joueurs et du tableau des scores (File I/O).
- **Intelligence Artificielle :**
  - Niveau 1 : IA Aléatoire (pioche ou joue une tuile au hasard si possible).
  - Niveau 2 (Bonus) : Algorithme simple pour compléter des combinaisons existantes sur le plateau.
- **Intégration finale :** Assemblage des modules Backend et Frontend.

### Planning prévisionnel

| Phase       | Description                                                 | Échéance      |
| :---------- | :---------------------------------------------------------- | :------------ |
| **Phase 1** | Conception, choix des outils et structures de données       | Semaine 1     |
| **Phase 2** | Dév. du moteur (règles) et initialisation fenêtre graphique | Semaine 2-3   |
| **Phase 3** | Liaison Interface-Moteur (jouer un coup graphiquement)      | Vacances Noël |
| **Phase 4** | Implémentation de l'IA et gestion des scores                | Janvier       |
| **Phase 5** | Tests finaux, débogage et préparation soutenance            | Fin Janvier   |

---

## VII. Conclusion

Ce rapport constitue la base de conception du projet Rummikub.  
Il décrit les besoins fonctionnels et techniques, la structure du système et l’organisation de l’équipe.  
Le développement suivra une approche incrémentale, avec un premier prototype prévu avant les vacances de Noël.

La version finale devra proposer un jeu stable, fidèle aux règles du Rummikub, intégrant une interface graphique fluide et une intelligence artificielle fonctionnelle.  
Le code sera documenté et testé pour garantir sa clarté, sa robustesse et sa conformité aux consignes de l’encadrant.
