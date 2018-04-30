/*
 * deque.c
 *	Majoritairement basée sur les cours de SD de M. VANDERHAEGHE
 *  Created on: 26 févr. 2018
 *      Author: Pierre
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "deque.h"

/**
 * Élément d'une liste doublement chainée
 */
typedef struct s_dequeNode {
	void *data;
	struct s_dequeNode *prev;
	struct s_dequeNode *next;
} DequeNode;

/**
 * Liste doublement chainée
 */
struct s_deque {
	DequeNode *sent;
	int size;
};

/**
 * Crée et initialise une liste doublement chaînée.
 * @return la liste initialisée
 */
Deque *deque() {
	Deque *d = malloc(sizeof(Deque));
	if (d == NULL) {
		printf("Erreur d'allocation mémoire pour deque()\n");
		exit(1);
	}

	d->size = 0;
	d->sent = malloc(sizeof(DequeNode));
	if (d->sent == NULL) {
		printf("Erreur d'allocation mémoire pour deque()\n");
		exit(1);
	}

	d->sent->next = d->sent;
	d->sent->prev = d->sent;

	return d;
}

/**
 * (Locale) Libère la mémoire d'un élément
 * @param dn Élément à libérer
 */
void freeNode(DequeNode *dn) {
	free(dn->data);
	free(dn);
}

/**
 * Vérifie si la liste donnée est vide.
 * @param d liste à vérifier
 * @return 1 si la liste est vide, 0 sinon
 */
int dequeEmpty(const Deque *d) {
	return d->size == 0;
}

/**
 * Vérifie le nombre d'éléments de la liste donnée.
 * @param d liste à vérifier
 * @return le nombre d'éléments dans la lsite
 */
int dequeSize(const Deque *d) {
	return d->size;
}

/**
 * Ajoute un élément au fond de la liste.
 * @param d Liste à modifier
 * @param elem Pointeur vers l'élément à ajouter
 * @return Liste modifiée
 */
Deque *push_back(Deque *d, void *elem) {
	DequeNode *n = malloc(sizeof(DequeNode));

	n->data = elem;
	n->prev = d->sent->prev;
	n->prev->next = n;
	d->sent->prev = n;
	n->next = d->sent;

	(d->size)++;

	return d;
}

/**
 * Ajoute un élément à l'avant de la liste.
 * @param d Liste à modifier
 * @param elem Pointeur vers l'élément à ajouter
 * @return Liste modifiée
 */
Deque *push_front(Deque *d, void *elem) {
	DequeNode *n = malloc(sizeof(DequeNode));

	n->data = elem;
	n->next = d->sent->next;
	n->next->prev = n;
	d->sent->next = n;
	n->prev = d->sent;

	(d->size)++;

	return d;
}

/**
 * Supprime le premier élément de la liste.
 * @param d Liste à modifier
 * @return Liste modifiée
 */
Deque *pop_front(Deque *d) {
	assert(!dequeEmpty(d));
	DequeNode *dn = d->sent->next;

	d->sent->next = dn->next;
	dn->next->prev = d->sent;
	freeNode(dn);
	(d->size)--;

	return d;
}

/**
 * Supprime le dernier élément de la liste.
 * @param d Liste à modifier
 * @return Liste modifiée
 */
Deque *pop_back(Deque *d) {
	assert(!dequeEmpty(d));
	DequeNode *dn = d->sent->prev;

	d->sent->prev = dn->prev;
	dn->prev->next = d->sent;
	freeNode(dn);
	(d->size)--;

	return d;
}

/**
 * Insère un élément à la position donnée dans la liste.
 * @param d Liste à modifier
 * @param elem Élément à ajouter
 * @param pos Position du nouvel élément dans la liste
 * @return Liste modifiée
 */
Deque *insert_at(Deque *d, void *elem, int pos) {
	assert(pos >= 0 && pos <= d->size);
	DequeNode *dn = malloc(sizeof(DequeNode));
	DequeNode *t = d->sent->next;

	dn->data = elem;
	while (pos--) t = t->next; //TODO à vérifier

	t->prev->next = dn;
	dn->prev = t->prev;
	t->prev = dn;
	dn->next = t;
	(d->size)++;

	return d;
}

/**
 * Supprime l'élément de la liste à la position donnée.
 * @param d Liste à modifier
 * @param pos Position de l'élément à supprimer
 * @return Liste modifiée
 */
Deque *remove_at(Deque *d, int pos) {
	assert(pos >= 0 && pos < d->size);
	DequeNode *t = d->sent->next;

	while (pos--) t = t->next; //TODO à vérifier

	t->next->prev = t->prev;
	t->prev->next = t->next;
	freeNode(t);
	(d->size)--;

	return d;
}

/**
 * Retourne le premier élément de la liste.
 * @param d Liste à lire
 * @return Premier élément
 */
void *front(const Deque *d) {
	assert(!dequeEmpty(d));
	return d->sent->next->data;
}

/**
 * Retourne le dernier élément de la liste.
 * @param d Liste à lire
 * @return Dernier élément
 */
void *back(const Deque *d) {
	assert(!dequeEmpty(d));
	return d->sent->prev->data;
}

/**
 * Retourne l'élément de la liste à la position donnée
 * @param d Liste à lire
 * @param pos Position de l'élément à trouver
 * @return Élément à la position donnée
 */
void *ith(const Deque *d, int pos) {
	assert(pos >= 0 && pos < d->size);
	DequeNode *t = d->sent->next;

	while (pos--) t = t->next;
	return t->data;
}

/**
 * Effectue une fonction sur chacun des éléments de la liste
 * @param d Liste à utiliser
 * @param f Fonction à appeler sur chaque élément
 */
void dequeMap(const Deque *d, void (*f) (void *)) {
	DequeNode *n = d->sent->next;
	while (n != d->sent) {
		f(n->data);
		n = n->next;
	}
}

/**
 * Supprime la liste et tout son contenu en le désallouant
 * @param d Liste à supprimer
 */
void delete_deque(Deque *d) {
	while (!dequeEmpty(d))
		pop_front(d);
	freeNode(d->sent);
	free(d);
}
