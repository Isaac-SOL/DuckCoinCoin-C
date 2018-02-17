/*
 * transaction.c
 *
 *  Created on: 3 févr. 2018
 *      Author: Pierre
 */

#include "transaction.h"
#include "string.h"
#include "sha256/sha256.h"

struct s_transactionBlock {
	int transactionCount;
	char **data;
};

/*
 * Initialisation d'une liste de transactions.
 */
void transactionBlock(TransactionBlock *tb) {
	tb->transactionCount = 0;
}

/**
 * Renvoie le nombre de transactions.
 * @param tb Block de transactions à lire
 * @return Nombre de transactions
 */
int getTransactionCount(TransactionBlock *tb) {
	return tb->transactionCount;
}

/**
 * Renvoie la transaction présente à l'index donné.
 * @param tb Block de transactions à lire
 * @param i Index de la transaction
 * @return Transaction à l'index i
 */
char *getTransactionAt(TransactionBlock *tb, int i) {
	return tb->data[i];
}

/*
 * Ajout d'une transaction dans la liste
 */
void addTransaction(TransactionBlock *tb, char t[TRANSACTION_LEN]) {
	tb->data[tb->transactionCount] = t;
}

/*
 * Calcul de la merkle root de la liste de transactions
 */
void merkleRoot(TransactionBlock *tb, char hash[SHA256_BLOCK_SIZE]) {

	char merkleTree[MAX_TRANSACTIONS][SHA256_BLOCK_SIZE];
	char nextMerkleTree[MAX_TRANSACTIONS][SHA256_BLOCK_SIZE];
	for (int i = 0; i < tb->transactionCount; i++) {
		SHA256_CTX ctx;
		sha256_init(&ctx);
		sha256_update(&ctx, (BYTE *) tb->data[i], TRANSACTION_LEN);
		sha256_final(&ctx, (BYTE *) merkleTree[i]);
	}

	int len = tb->transactionCount;
	while (len > 1) {
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

	memcpy(hash, merkleTree[0], SHA256_BLOCK_SIZE);
}
