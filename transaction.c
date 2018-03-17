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
 * Calcul de la merkle root d'un TransactionBlock.
 * @param tl Pointeur vers le TransactionBlock à lire
 * @param root Renvoie la merkleRoot du TransactionBlock
 */
void calcMerkleRoot(const TransactionList *tl, char root[SHA256_BLOCK_SIZE]) { //TODO il faut tout changer, cet algorithme n'est plus du tout valide

	char merkleTree[MAX_TRANSACTIONS][SHA256_BLOCK_SIZE];
	char nextMerkleTree[MAX_TRANSACTIONS][SHA256_BLOCK_SIZE];
	for (int i = 0; i < dequeSize(tl); i++) {
		SHA256_CTX ctx;
		sha256_init(&ctx);
		sha256_update(&ctx, (BYTE *) ith(tl, i), TRANSACTION_LEN);
		sha256_final(&ctx, (BYTE *) merkleTree[i]);
	}

	int len = dequeSize(tl);
	while (len > 1) {	//TODO optimiser cette partie (pas de memcpy nécessaire dans tous les cas)
		for (int i = 0; i < len; i += 2) {
			char concat[SHA256_BLOCK_SIZE * 2];
			if (i - len == 1) {
				memcpy(concat, merkleTree[i], SHA256_BLOCK_SIZE);
				memcpy(concat + SHA256_BLOCK_SIZE, merkleTree[i], SHA256_BLOCK_SIZE);
			} else {
				memcpy(concat, merkleTree[i], SHA256_BLOCK_SIZE * 2);
			}
			SHA256_CTX ctx;
			sha256_init(&ctx);
			sha256_update(&ctx, (BYTE *) concat, SHA256_BLOCK_SIZE * 2);
			sha256_final(&ctx, (BYTE *) nextMerkleTree[i/2]);
		}

		len = len/2 + len%2;
		memcpy(merkleTree, nextMerkleTree, sizeof(merkleTree));
	}

	memcpy(root, merkleTree[0], SHA256_BLOCK_SIZE);
}	//TODO tout ceci m'a l'air bien dégueulasse

///TODO Mettra ça dans un fichier à part, genre randomGeneration.c

/**
 * Génère une transaction aléatoire
 * @return Pointeur vers la transaction aléatoire
 */
/*char *randomTransaction() {
	char *result = malloc(TRANSACTION_LEN * sizeof(char));

	srand(time(NULL));
	sprintf(result, "%s -> %s: %d DCC.", randomElement(MEMBERS, 15*sizeof(char), 5),
										 randomElement(MEMBERS, 15*sizeof(char), 5),
										 rand()%100);

	return result;
}
*/
// Chasse (Nicolas) Generation transactions random
/**
 * Génère une liste de transactions aléatoires.
 * @return Une liste de transactions aléatoires.
 */
/*TransactionList *randomTransactionList() {
	TransactionList *tl = deque();
	srand(time(NULL));
	int max = rand() % (MAX_TRANSACTIONS+1);

	for (int i = 0; i < max; i++)
			addTransaction(tl, randomTransaction());

	return tl;
} //TODO A modifier pour faire des vraies transactions aléatoires
*/
