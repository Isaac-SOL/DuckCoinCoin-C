/*
 * util.c
 *
 *  Created on: 17 mars 2018
 *      Author: Pierre
 *
 *  Fonctions utilitaires
 *
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

/**
 * Renvoie la date à l'appel de la fonction.
 * @return date actuelle dans une chaîne de caractères
 */
char *getTimeStamp() {
	time_t ltime;
	time(&ltime);
	char *t = ctime(&ltime);
	t[strlen(t) - 1] = '\0';
	return t;
}

/**
 * Choisit un élément aléatoire dans un tableau.
 * @param elements Tableau d'éléments à choisir
 * @param elementSize Taille d'un élément
 * @param size Nombre d'éléments
 * @return pointeur vers l'élément choisi aléatoirement
 */
void *randomElement(void **elements, size_t elementSize, int size) {
	srand(time(NULL));
	return (void *) ((int *) elements + elementSize * (rand()%size));
}
