/*
 * transaction.c
 *
 *  Created on: 3 févr. 2018
 *      Author: Pierre
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "transaction.h"
#include "deque.h"
#include "sha256/sha256_utils.h"
#include "util.h"

/**
 * Ajout d'une transaction à un TransactionBlock.
 * @param tl Pointeur vers le TransactionBlock à modifier
 * @param transaction Transaction à ajouter
 */
void addTransaction(TransactionList *tl, Transaction transaction) {
	assert(!transactionListIsFull(tl));
	push_front(tl, transaction);
}

/**
 * Vérifie si le TransactionBlock est plein.
 * @param tl Pointeur vers le TransactionBlock à vérifier
 * @return Booléen, renvoie true si le TransactionBlock est plein, false sinon
 */
int transactionListIsFull(const TransactionList *tl) {
	return dequeSize(tl) == MAX_TRANSACTIONS;
}

/**
 * Transforme une liste de transactions en chaîne de caractères.
 * @param tl Liste à transformer
 * @return La liste transformée
 */
char *transactionsToString(const TransactionList *tl) {
	char *result = malloc(STR_TRANSACTIONLIST_LEN * sizeof(char));	//Fait dynamiquement pour garder le contenu à la sortie de la foncion
	if (result == NULL) {
		printf("Erreur d'allocation mémoire pour transactionsToString.\n");
		exit(1);
	}

	result[0] = '\0';
	for (int i = 0; i < dequeSize(tl); i++) {
		strcat(result, ith(tl, i));	//TODO à transformer avec une fontion map() comme vu en SDD?
	}

	return result;
}

/**
 * Affiche le contenu d'une transaction. Peut être utilisé par dequeMap().
 * @param vt La transaction à afficher
 */
void afficherTransaction(void *vt) {
	char *t = (char *) vt;
	printf("    %s\n", t);
}

/**
 * Calcul de la merkle root d'un TransactionBlock.
 * @param tl Pointeur vers le TransactionBlock à lire
 * @param root Renvoie la merkleRoot du TransactionBlock
 */
void calcMerkleRoot(const TransactionList *tl, char root[SHA256_BLOCK_SIZE*2 + 1]) {
	TransactionList *newList, *nextList;

	newList = deque();
	nextList = deque();

	//Hash de chaque transaction
	for (int i = 0; i < dequeSize(tl); i++) {
		char hash[SHA256_BLOCK_SIZE*2 + 1];
		char *dhash = malloc((SHA256_BLOCK_SIZE*2 + 1) * sizeof(char));
		sha256ofString((BYTE *) ith(tl, i), hash);
		memcpy(dhash, hash, (SHA256_BLOCK_SIZE*2 + 1) * sizeof(char));
		push_back(newList, dhash);
	}

	//Concaténation et hash successivement
	while (dequeSize(newList) != 1) {
		while (!dequeEmpty(newList)) {
			//Création du bloc contenant la concaténation
			char *cat = malloc((SHA256_BLOCK_SIZE*4 + 3) * sizeof(char));
			char *nextValue;
			nextValue = front(newList);
			pop_front(newList);
			memcpy(cat, nextValue, (SHA256_BLOCK_SIZE*2 + 1) * sizeof(char));
			if (!dequeEmpty(newList)) {
				nextValue = front(newList);
				pop_front(newList);
				memcpy(cat + (SHA256_BLOCK_SIZE*2 + 1), nextValue, SHA256_BLOCK_SIZE*2 + 1 * sizeof(char));
			} else memcpy(cat + (SHA256_BLOCK_SIZE*2 + 1), cat, SHA256_BLOCK_SIZE*2 + 1 * sizeof(char));
			cat[SHA256_BLOCK_SIZE*4 + 2] = '\0'; //NULL-terminated pour utiliser la fonction sha256OfString()

			//Cacul du hash et ajout à la liste
			char hash[SHA256_BLOCK_SIZE*2 + 1];
			char *dhash = malloc((SHA256_BLOCK_SIZE*2 + 1) * sizeof(char));
			sha256ofString((BYTE *) cat, hash);
			memcpy(dhash, hash, (SHA256_BLOCK_SIZE*2 + 1) * sizeof(char));
			push_back(nextList, dhash);
		}
		delete_deque(newList);
		newList = nextList;
		nextList = deque();
	}

	//Enregistrement du résultat et libération de la mémoire
	memcpy(root, front(newList), (SHA256_BLOCK_SIZE*2 + 1) * sizeof(char));
	delete_deque(newList);
	delete_deque(nextList);
}	//Bon c'est pas joli-joli mais ça devrait fonctionner
