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
            <td colspan=2 style='border: 1px solid black;text-align: left'> Responsable de la rédaction : Lucas DA SILVA FERREIRA </td>
        </tr>
    </tbody>
</table>

## <a id=home></a>Sommaire<!-- omit in toc -->
- [Dossier de tests](#dossier-de-tests)
    - [1. Introduction](#1-introduction)
    - [2. Description de la procédure de test](#2-description-de-la-procédure-de-test)
    - [3. Description des informations à enregistrer pour le test](#3-description-des-informations-à-enregistrer-pour-le-test)
        - [3.1. Campagne de test](#3.1.-campagne-de-test)
        - [3.2. Tests](#3.2.-tests)
        - [3.3. Résultats](#3.3.-résultats)
        - [3.4. Conclusions](#3.4.-conclusions)

## 1. Introduction
Ce travail est un projet lié au module Algorithmique (IATIC3). Il consiste à la réalisation du jeu de société **Rummikub** en langage **C**, avec une interface graphique utilisant **OpenGL** et la bibliothèque **GLFW**. Ce document est un dossier de tests qui consiste à décrire les procédures de tests mises en oeuvre afin de vérifier le bon fonctionnement des différentes fonctionnalités du projet.

Les tests décrits concernent :
- l'interface graphique,
- la boucle principale du jeu,
- la logique de jeu Rummikub,
- la gestion des joueurs,
- la gestion des fichiers,
- le comportement de l'intelligence artificielle.

## 2. Description de la procédure de test
Ces tests seront appliqués de manière récursive à chaque ajout de fonctionnalité. La stratégie adoptée suit une démarche proche d'un cycle en V récursif. Les tests effectués sont principalement des tests fonctionnels, des tests manuels et des tests visuels pour la partie graphique. L'ensemble des tests repose sur la méthode de la "boîte noire" : le comportement observé est comparé au comportement attendu, sans dépendre de l'implémentation interne.

## 3. Description des informations à enregistrer pour le test

* ### 3.1. Campagne de test

<table>
    <tbody>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Produit testé : </td>
            <td style='border: 1px solid black;text-align: left'>Jeu RUMMIKUB</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Configuration logicielle : </td>
            <td style='border: 1px solid black;text-align: left'>Langage C, OpenGL / GLFW, Windows (MSYS2 MinGW64) / Linux</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Configuration matérielle : </td>
            <td style='border: 1px solid black;text-align: left'>Ordinateur personnel de développement</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Date de début : Janvier 2026</td>
            <td style='border: 1px solid black;text-align: left'>Date de finalisation : Janvier 2026</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Tests à appliquer : </td>
            <td style='border: 1px solid black;text-align: left'>
                <ul>
                    <h4 style="font-weight: bold">MODULE Interface Graphique <a href="#classtest0"> TESTS </a> / <a href="#classresult0">RESULTATS</a></h4>
                    <li><a href="#test00">Test</a> de l'ouverture de la fenêtre OpenGL. <a href="#result00">Résultat ici</a></li>
                    <h4 style="font-weight: bold">MODULE Logique du jeu</h4>
                    <li>Tests sur les combinaisons (suites, séries)</li>
                    <li>Tests sur les règles des jokers</li>
                    <h4 style="font-weight: bold">MODULE Joueurs / IA / Fichiers</h4>
                    <li>Gestion des tours</li>
                    <li>IA simple (pioche / pose valide)</li>
                    <li>Sauvegarde des scores</li>
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
La création de la fenêtre et l'exécution de la boucle principale peuvent être partitionnées en deux cas :
- P0 : la fenêtre s'ouvre correctement et la boucle s'exécute
- P1 : la fenêtre ne s'ouvre pas ou provoque une erreur

<table>
    <tbody>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Identification du test : GUI_LOOP_TEST</td>
            <td style='border: 1px solid black;text-align: left'>Version : 1.0</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Description du test :</td>
            <td style='border: 1px solid black;text-align: left'>Test visuel visant à vérifier l'ouverture de la fenêtre OpenGL et l'exécution correcte de la boucle principale.</td>
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

| Classe | Environnement | Résultat attendu |
|:------:|:-------------:|:----------------:|
|   P0   | Windows/Linux | Fenêtre visible, fond coloré stable |
|   P1   | Windows/Linux | Erreur ou absence de fenêtre |

| Classe | Environnement | Résultat |
|:------:|:-------------:|:--------:|
|   P0   | Windows       | OK |
|   P0   | Linux         | OK |

- **/!\ absence de tableau de données, ceci est un test visuel /!\**

<a href="#home">retour au sommaire</a>

<hr>

### 3.3. Résultats

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
            <td style='border: 1px solid black;text-align: left'>Résultat :</td>
            <td style='border: 1px solid black;text-align: left'>OK</td>
        </tr>
        <tr>
            <td style='border: 1px solid black;text-align: left'>Occurences des résultats :</td>
            <td style='border: 1px solid black;text-align: left'>Systèmatique</td>
        </tr>
    </tbody>
</table>

<a href="#home">retour au sommaire</a>

<hr>

### 3.4. Conclusions
Les tests effectués valident le bon fonctionnement de la base du projet, notamment l'interface graphique et la boucle principale. Ce dossier de tests servira de référence pour l'ajout des fonctionnalités suivantes : rendu du plateau, interactions utilisateur, règles du jeu, intelligence artificielle et gestion des scores.