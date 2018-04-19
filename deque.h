/*
 * deque.h
 *
 *  Created on: 26 févr. 2018
 *      Author: Pierre
 */

#ifndef DEQUE_H_
#define DEQUE_H_

/**
 * Liste doublement chainée
 */
typedef struct s_deque Deque;

/**
 * Crée et initialise une liste doublement chaînée.
 * @return la liste initialisée
 */
Deque *deque();

/**
 * Vérifie si la liste donnée est vide.
 * @param d liste à vérifier
 * @return 1 si la liste est vide, 0 sinon
 */
int dequeEmpty(const Deque *d);

/**
 * Vérifie le nombre d'éléments de la liste donnée.
 * @param d liste à vérifier
 * @return le nombre d'éléments dans la lsite
 */
int dequeSize(const Deque *d);

/**
 * Ajoute un élément au fond de la liste.
 * @param d Liste à modifier
 * @param elem Pointeur vers l'élément à ajouter
 * @return Liste modifiée
 */
Deque *push_back(Deque *d, void *elem);

/**
 * Ajoute un élément à l'avant de la liste.
 * @param d Liste à modifier
 * @param elem Pointeur vers l'élément à ajouter
 * @return Liste modifiée
 */
Deque *push_front(Deque *d, void *elem);

/**
 * Supprime le premier élément de la liste.
 * @param d Liste à modifier
 * @return Liste modifiée
 */
Deque *pop_front(Deque *d);

/**
 * Supprime le dernier élément de la liste.
 * @param d Liste à modifier
 * @return Liste modifiée
 */
Deque *pop_back(Deque *d);

/**
 * Insère un élément à la position donnée dans la liste.
 * @param d Liste à modifier
 * @param elem Élément à ajouter
 * @param pos Position du nouvel élément dans la liste
 * @return Liste modifiée
 */
Deque *insert_at(Deque *d, void *elem, int pos);

/**
 * Supprime l'élément de la liste à la position donnée.
 * @param d Liste à modifier
 * @param pos Position de l'élément à supprimer
 * @return Liste modifiée
 */
Deque *remove_at(Deque *d, int pos);

/**
 * Retourne le premier élément de la liste.
 * @param d Liste à lire
 * @return Premier élément
 */
void *front(const Deque *d);

/**
 * Retourne le dernier élément de la liste.
 * @param d Liste à lire
 * @return Dernier élément
 */
void *back(const Deque *d);

/**
 * Retourne l'élément de la liste à la position donnée
 * @param d Liste à lire
 * @param pos Position de l'élément à trouver
 * @return Élément à la position donnée
 */
void *ith(const Deque *d, int pos);

/**
 * Effectue une fonction sur chacun des éléments de la liste
 * @param d Liste à utiliser
 * @param f Fonction à appeler sur chaque élément
 */
void dequeMap(const Deque *d, void (*f) (void *));

/**
 * Supprime la liste et tout son contenu en le désallouant
 * @param d Liste à supprimer
 */
void delete_deque(Deque *d);

#endif /* DEQUE_H_ */
