/*
 * transaction.c
 *
 *  Created on: 3 févr. 2018
 *      Author: Pierre
 */

#include <time.h>
#include <stdlib.h>
#include <assert.h>

#include "transaction.h"
#include "deque.h"
#include "string.h"
#include "sha256/sha256.h"

/**
 * Ajout d'une transaction à un TransactionBlock.
 * @param tl Pointeur vers le TransactionBlock à modifier
 * @param transaction Transaction à ajouter
 */
void addTransaction(TransactionList *tl, Transaction transaction) {
	assert(!isFull(tl));
	push_front(tl, transaction);
}

/**
 * Vérifie si le TransactionBlock est plein.
 * @param tl Pointeur vers le TransactionBlock à vérifier
 * @return Booléen, renvoie true si le TransactionBlock est plein, false sinon
 */
int isFull(const TransactionList *tl) {
	return dequeSize(tl) == MAX_TRANSACTIONS;
}

/**
 * Transforme une liste de transactions en chaîne de caractères.
 * @param tl Liste à transformer
 * @return La liste transformée
 */
char *ttoa(const TransactionList *tl) {
	//TODO à créer
	return "TODO";
}

/**
 * Calcul de la merkle root d'un TransactionBlock.
 * @param tl Pointeur vers le TransactionBlock à lire
 * @param root Renvoie la merkleRoot du TransactionBlock
 */
void merkleRoot(const TransactionList *tl, char root[SHA256_BLOCK_SIZE]) { //TODO il faut tout changer, cet algorithme n'est plus du tout valide

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


// Chasse (Nicolas) Generation transactions random
//fonction recuperee sur les forums stackexchange pour avoir un random string
char *rand_string(char *str, int size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}


TransactionList *random_tb(){
	char *str = malloc(TRANSACTION_LEN * sizeof(char));
	TransactionList *tb = deque();

	for (int i=0; i<MAX_TRANSACTIONS; i++){
			srand(time(NULL));
			addTransaction(tb, rand_string(str, TRANSACTION_LEN));
			free(tb);  //besoin du free ?
	}
	return tb;
} //TODO A modifier pour faire des vraies transactions aléatoires
