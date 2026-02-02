# Projet Rummikub 2025-2026

## Membres de l'equipe
- BENBACHA Kamar
- CHOISY Alexis
- DA SILVA FERREIRA Lucas
- DEGAT Teddy
- FOURNIE Baptiste

---

## Installation (Windows / MinGW32)

### Prerequis
- MinGW32 avec `gcc` et `mingw32-make` dans le PATH.

### Compilation
1) Ouvrir un terminal.
2) Se placer dans le dossier `src` :
   `cd ./src`
3) Lancer la compilation :
   `mingw32-make`

L'executable est genere a la racine du projet : `rummikub_gui.exe`.

---

## Installation (Linux)

### Prerequis
- `gcc` et `make`
- `pkg-config`
- GLFW (paquet systeme) ou GLFW depuis `third_party`

Exemple Debian/Ubuntu :
`sudo apt install build-essential pkg-config libglfw3-dev`

### Compilation
1) Ouvrir un terminal.
2) Se placer dans le dossier `src` :
   `cd ./src`
3) Lancer la compilation :
   `make`

L'executable est genere a la racine du projet : `rummikub_gui`.

---

## Comment jouer
- Lancer `rummikub_gui.exe` (Windows) ou `rummikub_gui` (Linux).
- Choisir le nombre de joueurs (2 a 4).
- Saisir un nom pour chaque joueur.
- Activer/desactiver un joueur IA avec le bouton "IA"/"HUM".
- Cliquer sur "CREER UNE PARTIE".
- Bouton "COMMENT JOUER ?" disponible dans le menu et en haut a gauche pendant la partie.
- Pendant la partie :
  - Trier la main avec "TRIER COULEUR" ou "TRIER VALEUR".
  - Glisser-deposer les tuiles pour former des combinaisons.
  - "JOUER" pose une combinaison selectionnee.
  - "VALIDER" termine le tour si la table est valide.
  - "PIOCHER" prend une tuile et termine le tour si aucun coup n'a ete joue.
- La premiere pose d'un joueur doit totaliser au moins 30 points.

---

### Documentation

L'ensemble de la documentation du projet est disponible dans le dossier [`/docs`](./docs).

Le rapport d'analyse et de conception (a remettre le 30 novembre 2025) est accessible ici :  
 [`/docs/rapport_analyse.md`](./docs/rapport_analyse.md)