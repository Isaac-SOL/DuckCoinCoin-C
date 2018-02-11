/*
 * blockchain.c
 *
 *  Created on: 2 févr. 2018
 *      Author: Pierre
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sha256/sha256.h"
#include "sha256/sha256_utils.h"
#include "blockchain.h"
#include "transaction.h"

/*
 * Structure de données représentant un bloc
 */
struct s_block {
	int index;
	char timestamp[30];
	char previousHash[SHA256_BLOCK_SIZE];
	int transactionCount;
	char transactions[MAX_TRANSACTIONS][TRANSACTION_LEN];
	char merkleRoot[SHA256_BLOCK_SIZE];
	char thisHash[SHA256_BLOCK_SIZE];
	int nonce;
};

/*
 * Structure de données représentant la Blockchain
 */
struct s_blockchain {
	int difficulty;
	int blockCount;
	Block *blocks;
	char lastBlockHash[SHA256_BLOCK_SIZE];
};

/*
 * Initialisation d'une Blockchain.
 */
void blockchain(Blockchain bc) {
	bc->blockCount = 0;
	bc->blocks = NULL;
}

/*
 * Initialisation d'un bloc.
 */
void block(Block b) {

	b->nonce = 0;
}

/* Définir la difficulté d'une Blockchain
 * int diff :	Nouvelle difficulté
 */
void difficulty(Blockchain bc, int diff) {
	bc->difficulty = diff;
}

/* Renvoie le hash du block sur 32 octets
 * char *hash :	Reçoit le hash du bloc en sortie
 */
void getBlockHash(Block b, char hash[SHA256_BLOCK_SIZE]) {
	SHA256_CTX ctx;
	sha256_init(&ctx);
	sha256_update(&ctx, (BYTE *) b, sizeof(struct s_block));
	sha256_final(&ctx, (BYTE *) hash);
}

/* Vérifie que le hash reçu corresponde bien à la difficulté
 * char *hash :		Hash à vérifier
 * int difficulty :	Difficulté de la blockchain
 */
int verifyHash(char hash[SHA256_BLOCK_SIZE], int difficulty) {
	for (int i = 0; i < difficulty; i++)
		if (hash != '0')
			return 0;
	return 1;
}

/* Incrémente la nonce du bloc jusqu'à ce que son hash corresponde à la difficulté
 * char *hash :		Renvoie le hash du bloc une fois la nonce trouvée
 * int difficulty :	Difficulté de la blockchain
 */
void updateNonce(Block b, char hash[SHA256_BLOCK_SIZE], int difficulty) {
	getBlockHash(b, hash);
	while (!verifyHash(hash, difficulty)) {
		(b->nonce)++;
		getBlockHash(b, hash);
	}
}

void copyTransactions(TransactionBlock tb, Block b) {
	b->transactionCount = tb->transactionCount;
	for (int i = 0; i < tb->transactionCount; i++)
		memcpy(b->transactions[i], tb->data[i], TRANSACTION_LEN);
}

/*
 * Ajoute un bloc à la Blockchain.
 */
void addBlock(Blockchain bc, Block b) {

	//Finalisation des informations du bloc
	if (bc->blocks == NULL) {
		//Informations pour le bloc génésis
		bc->blocks = (Block *) malloc(sizeof(Block));
		if(bc->blocks == NULL) {
			printf("Erreur d'allocation mémoire pour un Bloc.\n");
			exit(1);
		}
		//Nettoyage du hash car il n'y a pas de bloc précédent
		for (int i = 0; i < SHA256_BLOCK_SIZE; i++)
			b->previousHash[i] = '\0';
	} else {
		//Informations pour les blocs suivants
		memcpy(b->previousHash, bc->lastBlockHash, SHA256_BLOCK_SIZE);
	}
	b->index = bc->blockCount;
	//b->timestamp = ???

	//Calcul du hash
	updateNonce(b, bc->lastBlockHash, bc->difficulty);

	//Ajout du bloc à la blockchain
	bc->blocks = (Block *) realloc(bc->blocks, (bc->blockCount + 1) * sizeof(Block));
	bc->blocks[bc->blockCount] = b;
	(bc->blockCount)++;

}
