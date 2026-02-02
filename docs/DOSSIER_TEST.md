---
title:  Dossier de tests
author: Lucas DA SILVA FERREIRA
---

# Dossier de tests

<table>
    <tbody>
        <tr>
            <td style='border: 1px solid black;text-align: left'> Nom du projet : PROJET_RUMMIKUB </td>
            <td style='border: 1px solid black;text-align: left'> Version : 0.1.0 </td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'> Document : Dossier de tests </td>
            <td style='border: 1px solid black;text-align: left'> Date : Janvier 2026 </td>
        </tr>
        <tr>
            <td colspan=2 style='border: 1px solid black;text-align: left'> Responsable de la redaction : Lucas DA SILVA FERREIRA </td>
        </tr>
    </tbody>
</table>

## <a id=home></a>Sommaire<!-- omit in toc -->
- [Dossier de tests](#dossier-de-tests)
    - [1. Introduction](#1-introduction)
    - [2. Description de la procedure de test](#2-description-de-la-procedure-de-test)
    - [3. Description des informations a enregistrer pour le test](#3-description-des-informations-a-enregistrer-pour-le-test)
        - [3.1. Campagne de test](#3.1.-campagne-de-test)
        - [3.2. Tests](#3.2.-tests)
        - [3.3. Resultats](#3.3.-resultats)
        - [3.4. Conclusions](#3.4.-conclusions)

## 1. Introduction
Ce travail est un projet lie au module Algorithmique (IATIC3). Il consiste a la realisation du jeu de societe **Rummikub** en langage **C**, avec une interface graphique utilisant **OpenGL** et la bibliotheque **GLFW**. Ce document est un dossier de tests qui consiste a decrire les procedures de tests mises en oeuvre afin de verifier le bon fonctionnement des differentes fonctionnalites du projet.

Les tests decrits concernent :
- l'interface graphique,
- la boucle principale du jeu,
- la logique de jeu Rummikub,
- la gestion des joueurs,
- la gestion des fichiers,
- le comportement de l'intelligence artificielle.

## 2. Description de la procedure de test
Ces tests seront appliques de maniere recursive a chaque ajout de fonctionnalite. La strategie adoptee suit une demarche proche d'un cycle en V recursif. Les tests effectues sont principalement des tests fonctionnels, des tests manuels et des tests visuels pour la partie graphique. L'ensemble des tests repose sur la methode de la "boite noire" : le comportement observe est compare au comportement attendu, sans dependre de l'implementation interne.

## 3. Description des informations a enregistrer pour le test

* ### 3.1. Campagne de test

<table>
    <tbody>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Produit teste : </td>
            <td style='border: 1px solid black;text-align: left'>Jeu RUMMIKUB</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Configuration logicielle : </td>
            <td style='border: 1px solid black;text-align: left'>Langage C, OpenGL / GLFW, Windows (MSYS2 MinGW64) / Linux</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Configuration materielle : </td>
            <td style='border: 1px solid black;text-align: left'>Ordinateur personnel de developpement</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Date de debut : Janvier 2026</td>
            <td style='border: 1px solid black;text-align: left'>Date de finalisation : Janvier 2026</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Tests a appliquer : </td>
            <td style='border: 1px solid black;text-align: left'>
                <ul>
                    <h4 style="font-weight: bold">MODULE Interface Graphique <a href="#classtest0"> TESTS </a> / <a href="#classresult0">RESULTATS</a></h4>
                    <li><a href="#test00">Test</a> de l'ouverture de la fenetre OpenGL. <a href="#result00">Resultat ici</a></li>
                    <li><a href="#test01">Test</a> du menu principal (hover + clic boutons). <a href="#result01">Resultat ici</a></li>
                    <li><a href="#test02">Test</a> ecran "Comment jouer ?". <a href="#result02">Resultat ici</a></li>
                    <li><a href="#test03">Test</a> interactions UI en match (tri, jouer, valider, piocher). <a href="#result03">Resultat ici</a></li>
                    <h4 style="font-weight: bold">MODULE Logique du jeu</h4>
                    <li><a href="#test04">Test</a> combinaisons suites valides. <a href="#result04">Resultat ici</a></li>
                    <li><a href="#test05">Test</a> combinaisons groupes valides. <a href="#result05">Resultat ici</a></li>
                    <li><a href="#test06">Test</a> regles des jokers. <a href="#result06">Resultat ici</a></li>
                    <li><a href="#test07">Test</a> validation globale de table. <a href="#result07">Resultat ici</a></li>
                    <h4 style="font-weight: bold">MODULE Joueurs / IA / Fichiers</h4>
                    <li><a href="#test08">Test</a> gestion des tours. <a href="#result08">Resultat ici</a></li>
                    <li><a href="#test09">Test</a> IA simple (pioche / pose valide). <a href="#result09">Resultat ici</a></li>
                    <li><a href="#test10">Test</a> sauvegarde des scores. <a href="#result10">Resultat ici</a></li>
                </ul>
            </td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Responsable de la campagne de test : </td>
            <td style='border: 1px solid black;text-align: left'>Lucas DA SILVA FERREIRA</td>
        </tr>
    </tbody>
</table>

### 3.2. Tests

#### <a id=classtest0></a><p style="font-weight: bold">*MODULE "Interface Graphique" :*</p>

##### <a id=test00></a>**TEST "Boucle principale OpenGL" :**

* Choix de partition :
La creation de la fenetre et l'execution de la boucle principale peuvent etre partitionnees en deux cas :
- P0 : la fenetre s'ouvre correctement et la boucle s'execute
- P1 : la fenetre ne s'ouvre pas ou provoque une erreur

<table>
    <tbody>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Identification du test : GUI_LOOP_TEST</td>
            <td style='border: 1px solid black;text-align: left'>Version : 1.0</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Description du test :</td>
            <td style='border: 1px solid black;text-align: left'>Test visuel visant a verifier l'ouverture de la fenetre OpenGL et l'execution correcte de la boucle principale.</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Ressources requises : </td>
            <td style='border: 1px solid black;text-align: left'>GLFW, OpenGL, environnement de compilation fonctionnel</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Responsable de la campagne de test : </td>
            <td style='border: 1px solid black;text-align: left'>Lucas DA SILVA FERREIRA</td>
        </tr>
    </tbody>
</table>

| Classe | Environnement | Resultat attendu |
|:------:|:-------------:|:----------------:|
|   P0   | Windows/Linux | Fenetre visible, fond colore stable |
|   P1   | Windows/Linux | Erreur ou absence de fenetre |

| Classe | Environnement | Resultat |
|:------:|:-------------:|:--------:|
|   P0   | Windows       | OK |
|   P0   | Linux         | OK |

- **/!\ absence de tableau de donnees, ceci est un test visuel /!\**

<a href="#home">retour au sommaire</a>

<hr>

##### <a id=test01></a>**TEST "Menu principal - hover et clics" :**

* Choix de partition :
Le comportement du menu est partitionne en deux cas :
- P0 : les boutons changent d'apparence au survol et reagissent au clic
- P1 : aucun effet hover ou clic ne fonctionne

<table>
    <tbody>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Identification du test : GUI_MENU_HOVER</td>
            <td style='border: 1px solid black;text-align: left'>Version : 1.0</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Description du test :</td>
            <td style='border: 1px solid black;text-align: left'>Verifier les effets de survol et les clics sur +, -, IA/HUM, CREER UNE PARTIE, COMMENT JOUER.</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Ressources requises : </td>
            <td style='border: 1px solid black;text-align: left'>Executable GUI, souris</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Responsable de la campagne de test : </td>
            <td style='border: 1px solid black;text-align: left'>Lucas DA SILVA FERREIRA</td>
        </tr>
    </tbody>
</table>

| Classe | Environnement | Resultat attendu |
|:------:|:-------------:|:----------------:|
|   P0   | Windows/Linux | Survol visible + clics reactifs |
|   P1   | Windows/Linux | Aucun changement visuel ou clic |

| Classe | Environnement | Resultat |
|:------:|:-------------:|:--------:|
|   P0   | Windows       | OK |
|   P0   | Linux         | OK |

<a href="#home">retour au sommaire</a>

<hr>

##### <a id=test02></a>**TEST "Ecran Comment jouer" :**

* Choix de partition :
- P0 : l'ecran s'ouvre, les exemples sont visibles, le bouton Retour fonctionne
- P1 : l'ecran ne s'ouvre pas ou le bouton Retour ne fonctionne pas

<table>
    <tbody>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Identification du test : GUI_HOWTO_SCREEN</td>
            <td style='border: 1px solid black;text-align: left'>Version : 1.0</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Description du test :</td>
            <td style='border: 1px solid black;text-align: left'>Verifier l'ouverture de l'ecran "Comment jouer ?" depuis le menu et depuis un match.</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Ressources requises : </td>
            <td style='border: 1px solid black;text-align: left'>Executable GUI, souris</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Responsable de la campagne de test : </td>
            <td style='border: 1px solid black;text-align: left'>Lucas DA SILVA FERREIRA</td>
        </tr>
    </tbody>
</table>

| Classe | Environnement | Resultat attendu |
|:------:|:-------------:|:----------------:|
|   P0   | Windows/Linux | Ecran visible + retour au bon ecran |
|   P1   | Windows/Linux | Ecran absent ou retour incorrect |

| Classe | Environnement | Resultat |
|:------:|:-------------:|:--------:|
|   P0   | Windows       | OK |
|   P0   | Linux         | OK |

<a href="#home">retour au sommaire</a>

<hr>

##### <a id=test03></a>**TEST "UI match - boutons d'action" :**

* Choix de partition :
- P0 : tri, jouer, valider, piocher reagissent correctement
- P1 : un ou plusieurs boutons ne reagissent pas

<table>
    <tbody>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Identification du test : GUI_MATCH_CONTROLS</td>
            <td style='border: 1px solid black;text-align: left'>Version : 1.0</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Description du test :</td>
            <td style='border: 1px solid black;text-align: left'>Verifier les boutons TRIER, JOUER, VALIDER, PIOCHER en match.</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Ressources requises : </td>
            <td style='border: 1px solid black;text-align: left'>Executable GUI, souris</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Responsable de la campagne de test : </td>
            <td style='border: 1px solid black;text-align: left'>Lucas DA SILVA FERREIRA</td>
        </tr>
    </tbody>
</table>

| Classe | Environnement | Resultat attendu |
|:------:|:-------------:|:----------------:|
|   P0   | Windows/Linux | Boutons actifs, actions correctes |
|   P1   | Windows/Linux | Actions absentes ou incorrectes |

| Classe | Environnement | Resultat |
|:------:|:-------------:|:--------:|
|   P0   | Windows       | OK |
|   P0   | Linux         | OK |

<a href="#home">retour au sommaire</a>

<hr>

##### <a id=test04></a>**TEST "Combinaisons - suites valides" :**

* Choix de partition :
- P0 : une suite valide est acceptee
- P1 : une suite invalide est rejetee

<table>
    <tbody>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Identification du test : LOGIC_SUITE_VALID</td>
            <td style='border: 1px solid black;text-align: left'>Version : 1.0</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Description du test :</td>
            <td style='border: 1px solid black;text-align: left'>Tester is_valid_suite et combinaison_points sur des suites.</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Ressources requises : </td>
            <td style='border: 1px solid black;text-align: left'>Jeu de tuiles en memoire</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Responsable de la campagne de test : </td>
            <td style='border: 1px solid black;text-align: left'>Lucas DA SILVA FERREIRA</td>
        </tr>
    </tbody>
</table>

| Classe | Environnement | Resultat attendu |
|:------:|:-------------:|:----------------:|
|   P0   | Windows/Linux | Suites valides acceptees |
|   P1   | Windows/Linux | Suites invalides rejetees |

| Classe | Environnement | Resultat |
|:------:|:-------------:|:--------:|
|   P0   | Windows       | OK |
|   P0   | Linux         | OK |

<a href="#home">retour au sommaire</a>

<hr>

##### <a id=test05></a>**TEST "Combinaisons - groupes valides" :**

* Choix de partition :
- P0 : un groupe valide est accepte
- P1 : un groupe invalide est rejete

<table>
    <tbody>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Identification du test : LOGIC_GROUP_VALID</td>
            <td style='border: 1px solid black;text-align: left'>Version : 1.0</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Description du test :</td>
            <td style='border: 1px solid black;text-align: left'>Tester is_valid_group sur des groupes de 3/4 tuiles.</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Ressources requises : </td>
            <td style='border: 1px solid black;text-align: left'>Jeu de tuiles en memoire</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Responsable de la campagne de test : </td>
            <td style='border: 1px solid black;text-align: left'>Lucas DA SILVA FERREIRA</td>
        </tr>
    </tbody>
</table>

| Classe | Environnement | Resultat attendu |
|:------:|:-------------:|:----------------:|
|   P0   | Windows/Linux | Groupes valides acceptes |
|   P1   | Windows/Linux | Groupes invalides rejetes |

| Classe | Environnement | Resultat |
|:------:|:-------------:|:--------:|
|   P0   | Windows       | OK |
|   P0   | Linux         | OK |

<a href="#home">retour au sommaire</a>

<hr>

##### <a id=test06></a>**TEST "Joker - substitution" :**

* Choix de partition :
- P0 : un joker valide une combinaison correcte
- P1 : un joker utilise hors regles est rejete

<table>
    <tbody>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Identification du test : LOGIC_JOKER_RULES</td>
            <td style='border: 1px solid black;text-align: left'>Version : 1.0</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Description du test :</td>
            <td style='border: 1px solid black;text-align: left'>Verifier la substitution de joker dans suites et groupes.</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Ressources requises : </td>
            <td style='border: 1px solid black;text-align: left'>Jeu de tuiles en memoire</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Responsable de la campagne de test : </td>
            <td style='border: 1px solid black;text-align: left'>Lucas DA SILVA FERREIRA</td>
        </tr>
    </tbody>
</table>

| Classe | Environnement | Resultat attendu |
|:------:|:-------------:|:----------------:|
|   P0   | Windows/Linux | Joker accepte si combinaison valide |
|   P1   | Windows/Linux | Joker rejete si combinaison invalide |

| Classe | Environnement | Resultat |
|:------:|:-------------:|:--------:|
|   P0   | Windows       | OK |
|   P0   | Linux         | OK |

<a href="#home">retour au sommaire</a>

<hr>

##### <a id=test07></a>**TEST "Validation globale de table" :**

* Choix de partition :
- P0 : table valide acceptee
- P1 : table invalide rejetee

<table>
    <tbody>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Identification du test : LOGIC_TABLE_VERIFY</td>
            <td style='border: 1px solid black;text-align: left'>Version : 1.0</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Description du test :</td>
            <td style='border: 1px solid black;text-align: left'>Tester verify_whole_table apres deplacements multiples.</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Ressources requises : </td>
            <td style='border: 1px solid black;text-align: left'>Etat de table pre-charge</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Responsable de la campagne de test : </td>
            <td style='border: 1px solid black;text-align: left'>Lucas DA SILVA FERREIRA</td>
        </tr>
    </tbody>
</table>

| Classe | Environnement | Resultat attendu |
|:------:|:-------------:|:----------------:|
|   P0   | Windows/Linux | Validation OK si table correcte |
|   P1   | Windows/Linux | Validation KO si table invalide |

| Classe | Environnement | Resultat |
|:------:|:-------------:|:--------:|
|   P0   | Windows       | OK |
|   P0   | Linux         | OK |

<a href="#home">retour au sommaire</a>

<hr>

##### <a id=test08></a>**TEST "Gestion des tours" :**

* Choix de partition :
- P0 : passage de tour correct (joueur suivant, pioche si besoin)
- P1 : ordre de tour ou main incoherents

<table>
    <tbody>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Identification du test : PLAYER_TURN_FLOW</td>
            <td style='border: 1px solid black;text-align: left'>Version : 1.0</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Description du test :</td>
            <td style='border: 1px solid black;text-align: left'>Tester begin_turn / end_turn et mise a jour du joueur courant.</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Ressources requises : </td>
            <td style='border: 1px solid black;text-align: left'>Partie en cours, 2 a 4 joueurs</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Responsable de la campagne de test : </td>
            <td style='border: 1px solid black;text-align: left'>Lucas DA SILVA FERREIRA</td>
        </tr>
    </tbody>
</table>

| Classe | Environnement | Resultat attendu |
|:------:|:-------------:|:----------------:|
|   P0   | Windows/Linux | Tour suivant correct |
|   P1   | Windows/Linux | Tour bloque ou incoherent |

| Classe | Environnement | Resultat |
|:------:|:-------------:|:--------:|
|   P0   | Windows       | OK |
|   P0   | Linux         | OK |

<a href="#home">retour au sommaire</a>

<hr>

##### <a id=test09></a>**TEST "IA simple" :**

* Choix de partition :
- P0 : l'IA joue une combinaison valide ou pioche
- P1 : l'IA joue une combinaison invalide

<table>
    <tbody>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Identification du test : AI_SIMPLE_TURN</td>
            <td style='border: 1px solid black;text-align: left'>Version : 1.0</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Description du test :</td>
            <td style='border: 1px solid black;text-align: left'>Verifier le comportement de l'IA (pose ou pioche).</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Ressources requises : </td>
            <td style='border: 1px solid black;text-align: left'>Partie avec au moins un joueur IA</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Responsable de la campagne de test : </td>
            <td style='border: 1px solid black;text-align: left'>Lucas DA SILVA FERREIRA</td>
        </tr>
    </tbody>
</table>

| Classe | Environnement | Resultat attendu |
|:------:|:-------------:|:----------------:|
|   P0   | Windows/Linux | IA pose ou pioche sans erreur |
|   P1   | Windows/Linux | IA provoque une erreur de table |

| Classe | Environnement | Resultat |
|:------:|:-------------:|:--------:|
|   P0   | Windows       | OK |
|   P0   | Linux         | OK |

<a href="#home">retour au sommaire</a>

<hr>

##### <a id=test10></a>**TEST "Sauvegarde des scores" :**

* Choix de partition :
- P0 : scores sauvegardes dans scores.txt a la fin
- P1 : fichier absent ou incomplet

<table>
    <tbody>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Identification du test : FILE_SCORES_SAVE</td>
            <td style='border: 1px solid black;text-align: left'>Version : 1.0</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Description du test :</td>
            <td style='border: 1px solid black;text-align: left'>Verifier la sauvegarde des scores en fin de partie.</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Ressources requises : </td>
            <td style='border: 1px solid black;text-align: left'>Partie terminee</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Responsable de la campagne de test : </td>
            <td style='border: 1px solid black;text-align: left'>Lucas DA SILVA FERREIRA</td>
        </tr>
    </tbody>
</table>

| Classe | Environnement | Resultat attendu |
|:------:|:-------------:|:----------------:|
|   P0   | Windows/Linux | Fichier scores.txt correct |
|   P1   | Windows/Linux | Fichier absent ou errone |

| Classe | Environnement | Resultat |
|:------:|:-------------:|:--------:|
|   P0   | Windows       | OK |
|   P0   | Linux         | OK |

<a href="#home">retour au sommaire</a>

<hr>

### 3.3. Resultats

#### <a id=classresult0></a><p style="font-weight: bold">*MODULE "Interface Graphique" :*</p>

##### <a id=result00></a>**TEST "Boucle principale OpenGL" :**

<table>
    <tbody>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Identification du test :</td>
            <td style='border: 1px solid black;text-align: left'>GUI_LOOP_TEST</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Responsable :</td>
            <td style='border: 1px solid black;text-align: left'>Lucas DA SILVA FERREIRA</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Date d'application du test :</td>
            <td style='border: 1px solid black;text-align: left'>12 Janvier 2026</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Resultat :</td>
            <td style='border: 1px solid black;text-align: left'>OK</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Occurences des resultats :</td>
            <td style='border: 1px solid black;text-align: left'>Systematique</td>
        </tr>
    </tbody>
</table>

<a href="#home">retour au sommaire</a>

<hr>

##### <a id=result01></a>**TEST "Menu principal - hover et clics" :**

<table>
    <tbody>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Identification du test :</td>
            <td style='border: 1px solid black;text-align: left'>GUI_MENU_HOVER</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Responsable :</td>
            <td style='border: 1px solid black;text-align: left'>Lucas DA SILVA FERREIRA</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Date d'application du test :</td>
            <td style='border: 1px solid black;text-align: left'>12 Janvier 2026</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Resultat :</td>
            <td style='border: 1px solid black;text-align: left'>OK</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Occurences des resultats :</td>
            <td style='border: 1px solid black;text-align: left'>Systematique</td>
        </tr>
    </tbody>
</table>

<a href="#home">retour au sommaire</a>

<hr>

##### <a id=result02></a>**TEST "Ecran Comment jouer" :**

<table>
    <tbody>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Identification du test :</td>
            <td style='border: 1px solid black;text-align: left'>GUI_HOWTO_SCREEN</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Responsable :</td>
            <td style='border: 1px solid black;text-align: left'>Lucas DA SILVA FERREIRA</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Date d'application du test :</td>
            <td style='border: 1px solid black;text-align: left'>12 Janvier 2026</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Resultat :</td>
            <td style='border: 1px solid black;text-align: left'>OK</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Occurences des resultats :</td>
            <td style='border: 1px solid black;text-align: left'>Systematique</td>
        </tr>
    </tbody>
</table>

<a href="#home">retour au sommaire</a>

<hr>

##### <a id=result03></a>**TEST "UI match - boutons d'action" :**

<table>
    <tbody>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Identification du test :</td>
            <td style='border: 1px solid black;text-align: left'>GUI_MATCH_CONTROLS</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Responsable :</td>
            <td style='border: 1px solid black;text-align: left'>Lucas DA SILVA FERREIRA</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Date d'application du test :</td>
            <td style='border: 1px solid black;text-align: left'>12 Janvier 2026</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Resultat :</td>
            <td style='border: 1px solid black;text-align: left'>OK</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Occurences des resultats :</td>
            <td style='border: 1px solid black;text-align: left'>Systematique</td>
        </tr>
    </tbody>
</table>

<a href="#home">retour au sommaire</a>

<hr>
#### <a id=classresult1></a><p style="font-weight: bold">*MODULE "Logique du jeu" :*</p>

##### <a id=result04></a>**TEST "Combinaisons - suites valides" :**

<table>
    <tbody>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Identification du test :</td>
            <td style='border: 1px solid black;text-align: left'>LOGIC_SUITE_VALID</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Responsable :</td>
            <td style='border: 1px solid black;text-align: left'>Lucas DA SILVA FERREIRA</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Date d'application du test :</td>
            <td style='border: 1px solid black;text-align: left'>12 Janvier 2026</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Resultat :</td>
            <td style='border: 1px solid black;text-align: left'>OK</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Occurences des resultats :</td>
            <td style='border: 1px solid black;text-align: left'>Systematique</td>
        </tr>
    </tbody>
</table>

<a href="#home">retour au sommaire</a>

<hr>

##### <a id=result05></a>**TEST "Combinaisons - groupes valides" :**

<table>
    <tbody>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Identification du test :</td>
            <td style='border: 1px solid black;text-align: left'>LOGIC_GROUP_VALID</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Responsable :</td>
            <td style='border: 1px solid black;text-align: left'>Lucas DA SILVA FERREIRA</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Date d'application du test :</td>
            <td style='border: 1px solid black;text-align: left'>12 Janvier 2026</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Resultat :</td>
            <td style='border: 1px solid black;text-align: left'>OK</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Occurences des resultats :</td>
            <td style='border: 1px solid black;text-align: left'>Systematique</td>
        </tr>
    </tbody>
</table>

<a href="#home">retour au sommaire</a>

<hr>

##### <a id=result06></a>**TEST "Joker - substitution" :**

<table>
    <tbody>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Identification du test :</td>
            <td style='border: 1px solid black;text-align: left'>LOGIC_JOKER_RULES</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Responsable :</td>
            <td style='border: 1px solid black;text-align: left'>Lucas DA SILVA FERREIRA</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Date d'application du test :</td>
            <td style='border: 1px solid black;text-align: left'>12 Janvier 2026</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Resultat :</td>
            <td style='border: 1px solid black;text-align: left'>OK</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Occurences des resultats :</td>
            <td style='border: 1px solid black;text-align: left'>Systematique</td>
        </tr>
    </tbody>
</table>

<a href="#home">retour au sommaire</a>

<hr>

##### <a id=result07></a>**TEST "Validation globale de table" :**

<table>
    <tbody>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Identification du test :</td>
            <td style='border: 1px solid black;text-align: left'>LOGIC_TABLE_VERIFY</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Responsable :</td>
            <td style='border: 1px solid black;text-align: left'>Lucas DA SILVA FERREIRA</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Date d'application du test :</td>
            <td style='border: 1px solid black;text-align: left'>12 Janvier 2026</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Resultat :</td>
            <td style='border: 1px solid black;text-align: left'>OK</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Occurences des resultats :</td>
            <td style='border: 1px solid black;text-align: left'>Systematique</td>
        </tr>
    </tbody>
</table>

<a href="#home">retour au sommaire</a>

<hr>

#### <a id=classresult2></a><p style="font-weight: bold">*MODULE "Joueurs / IA / Fichiers" :*</p>

##### <a id=result08></a>**TEST "Gestion des tours" :**

<table>
    <tbody>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Identification du test :</td>
            <td style='border: 1px solid black;text-align: left'>PLAYER_TURN_FLOW</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Responsable :</td>
            <td style='border: 1px solid black;text-align: left'>Lucas DA SILVA FERREIRA</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Date d'application du test :</td>
            <td style='border: 1px solid black;text-align: left'>12 Janvier 2026</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Resultat :</td>
            <td style='border: 1px solid black;text-align: left'>OK</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Occurences des resultats :</td>
            <td style='border: 1px solid black;text-align: left'>Systematique</td>
        </tr>
    </tbody>
</table>

<a href="#home">retour au sommaire</a>

<hr>

##### <a id=result09></a>**TEST "IA simple" :**

<table>
    <tbody>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Identification du test :</td>
            <td style='border: 1px solid black;text-align: left'>AI_SIMPLE_TURN</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Responsable :</td>
            <td style='border: 1px solid black;text-align: left'>Lucas DA SILVA FERREIRA</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Date d'application du test :</td>
            <td style='border: 1px solid black;text-align: left'>12 Janvier 2026</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Resultat :</td>
            <td style='border: 1px solid black;text-align: left'>OK</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Occurences des resultats :</td>
            <td style='border: 1px solid black;text-align: left'>Systematique</td>
        </tr>
    </tbody>
</table>

<a href="#home">retour au sommaire</a>

<hr>

##### <a id=result10></a>**TEST "Sauvegarde des scores" :**

<table>
    <tbody>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Identification du test :</td>
            <td style='border: 1px solid black;text-align: left'>FILE_SCORES_SAVE</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Responsable :</td>
            <td style='border: 1px solid black;text-align: left'>Lucas DA SILVA FERREIRA</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Date d'application du test :</td>
            <td style='border: 1px solid black;text-align: left'>12 Janvier 2026</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Resultat :</td>
            <td style='border: 1px solid black;text-align: left'>OK</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Occurences des resultats :</td>
            <td style='border: 1px solid black;text-align: left'>Systematique</td>
        </tr>
    </tbody>
</table>

<a href="#home">retour au sommaire</a>

<hr>

### 3.4. Conclusions
Les tests effectues valident le bon fonctionnement de la base du projet, notamment l'interface graphique et la boucle principale. Ce dossier de tests servira de reference pour l'ajout de fonctionnalites.