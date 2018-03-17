/*
 * util.h
 *
 *  Created on: 17 mars 2018
 *      Author: Pierre
 *
 *  Fonctions utilitaires
 *
 */

#ifndef UTIL_H_
#define UTIL_H_

/**
 * Renvoie la date à l'appel de la fonction.
 * @return date actuelle dans une chaîne de caractères
 */
char *getTimeStamp();

/**
 * Choisit un élément aléatoire dans un tableau.
 * @param elements Tableau d'éléments à choisir
 * @param elementSize Taille d'un élément
 * @param size Nombre d'éléments
 * @return pointeur vers l'élément choisi aléatoirement
 */
void *randomElement(void **elements, size_t elementSize, int size);

#endif /* UTIL_H_ */
