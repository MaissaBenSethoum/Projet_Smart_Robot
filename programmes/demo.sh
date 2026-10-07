#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Erreur : Vous devez fournir le nom du fichier ville."
    echo "Usage : ./demo.sh <fichier_ville.txt>"
    exit 1
fi

FICHER_VILLE="$1"

if [ ! -f "$FICHER_VILLE" ]; then
    echo "Erreur : Le fichier $FICHER_VILLE n'existe pas."
    exit 1
fi

echo "=== Préparation de la démonstration ==="
echo "Fichier ville : $FICHER_VILLE"

sudo 

echo "=== Lancement du robot... ==="
echo "-----------------------------"

time ( ./bin/cerveau "$FICHER_VILLE" | sudo ./bin/robot )

echo "-----------------------------"
echo "=== Démonstration terminée ==="