# PI Smart Robot — Navigation autonome

Projet intégratif réalisé en ITI3 à l’INSA Rouen Normandie, combinant **programmation en C, algorithmique, électronique et systèmes embarqués**.

## Présentation

L’objectif du projet est de concevoir un robot autonome capable de se déplacer dans une ville modélisée sous forme de cases, en suivant des lignes noires et en détectant différents points de passage.

Le robot utilise les informations de navigation pour générer des instructions de déplacement et peut détecter des pastilles de couleur, émettre un signal sonore et afficher des informations sur un écran LCD.

## Fonctionnalités

* Suivi de lignes avec des capteurs infrarouges
* Détection de couleurs avec un capteur TCS34725
* Pilotage de deux moteurs
* Affichage des informations sur un écran LCD
* Signal sonore lors de la détection de points de passage
* Recherche de chemin dans une ville modélisée sous forme de graphe
* Traduction d’un chemin en instructions de déplacement
* Gestion de plusieurs tâches en parallèle pour la détection des couleurs

## Technologies et matériel

* **C**
* **Raspberry Pi**
* TCRT5000 — capteurs de ligne
* TCS34725 — capteur de couleur
* L293D — contrôleur moteur
* Écran LCD
* Buzzer

## Algorithmique

La ville est représentée sous forme de graphe. Un **parcours en largeur (BFS)** est utilisé pour rechercher un chemin entre différentes cases.

Le chemin obtenu est ensuite traduit en instructions compréhensibles par le robot : avancer, tourner à gauche ou tourner à droite.

L’implémentation comprend notamment les structures `Case` et `Ville` ainsi que différentes fonctions dédiées au chargement de la ville, à la recherche de chemin et au pilotage.

## Organisation

```text
PI-Smart-Robot/
├── programmes/
│   ├── include/
│   ├── src/
│   │   └── tests/
│   └── doc/
├── rapport/
└── README.md
```

## Limites et améliorations

La recherche du plus court chemin a été développée et étudiée mais n’a pas pu être pleinement intégrée au fonctionnement final du robot. Le projet a donc privilégié une solution fonctionnelle et robuste.

Parmi les améliorations envisagées : optimiser la recherche de chemin, améliorer la gestion des erreurs et rendre le contrôle du robot moins dépendant des temporisations.

## Auteurs

* Maïssa BEN SETHOUM
* Lilian DUJOURD’HUI
* Karim HAMMOUDI
* Ilyass EL ASSAD
* Marwan GUERROUACHE
* Christopher ROYER

**Projet intégratif ITI3 — INSA Rouen Normandie — 2025**
