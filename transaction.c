/*
 * transaction.c
 *
 *  Created on: 3 févr. 2018
 *      Author: Pierre
 */

#include <time.h>
#include <stdlib.h>

#include "transaction.h"
#include "string.h"
#include "sha256/sha256.h"

/**
 * Liste de transactions.
 */
struct s_transactionList {
	int count;
	char data[MAX_TRANSACTIONS][TRANSACTION_LEN];
};

/**
 * Initialisation d'un TransactionBlock.
 * @param tb Pointeur vers le TransactionBlock à initialiser
 */
void transactionBlock(TransactionList *tb) {
	tb->count = 0;
}

/**
 * Ajout d'une transaction à un TransactionBlock.
 * @param tb Pointeur vers le TransactionBlock à modifier
 * @param transaction Transaction à ajouter
 */
void addTransaction(TransactionList *tb, const char transaction[TRANSACTION_LEN]) {
	memcpy(tb->data + tb->count, transaction, TRANSACTION_LEN);
}

/**
 * Vérifie si le TransactionBlock est plein
 * @param tb Pointeur vers le TransactionBlock à vérifier
 * @return Booléen, renvoie true si le TransactionBlock est plein, false sinon
 */
int isFull(const TransactionList *tb) {
	return tb->count == MAX_TRANSACTIONS;
}

/**
 * Renvoie le nombre de transactions dans un TransactionBlock.
 * @param tb Pointeur vers le TransactionBlock à lire
 * @return Nombre de transactions
 */
int getTransactionCount(const TransactionList *tb) {
	return tb->count;
}	//TODO voir si ça sert vraiment à quelque chose

/**
 * Renvoie la transaction présente à l'index donné.
 * @param tb Pointeur vers le TransactionBlock à lire
 * @param i Index de la transaction
 * @return Transaction à l'index i
 */
const char *getTransactionAt(const TransactionList *tb, int i) {
	return tb->data[i];
}	//TODO voir si ça sert vraiment à quelque chose

/**
 * Calcul de la merkle root d'un TransactionBlock.
 * @param tb Pointeur vers le TransactionBlock à lire
 * @param root Renvoie la merkleRoot du TransactionBlock
 */
void merkleRoot(const TransactionList *tb, char root[SHA256_BLOCK_SIZE]) {

	char merkleTree[MAX_TRANSACTIONS][SHA256_BLOCK_SIZE];
	char nextMerkleTree[MAX_TRANSACTIONS][SHA256_BLOCK_SIZE];
	for (int i = 0; i < tb->count; i++) {
		SHA256_CTX ctx;
		sha256_init(&ctx);
		sha256_update(&ctx, (BYTE *) tb->data[i], TRANSACTION_LEN);
		sha256_final(&ctx, (BYTE *) merkleTree[i]);
	}

	int len = tb->count;
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
	TransactionList *tb = malloc(sizeof(TransactionList));

	for (int i=0; i<MAX_TRANSACTIONS; i++){
			srand(time(NULL));
			transactionBlock(tb);
			addTransaction(tb, rand_string(str, TRANSACTION_LEN));
			free(tb);  //besoin du free ?
	}
	return tb;
} //A modifier [wip]
