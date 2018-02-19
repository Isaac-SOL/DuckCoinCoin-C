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
#include "blockchain.h"
#include "transaction.h"

/**
 * Structure de données représentant un bloc
 */
struct s_block {
	int index;
	char timestamp[30];
	char previousHash[SHA256_BLOCK_SIZE];
	TransactionBlock transactions;
	char merkleRoot[SHA256_BLOCK_SIZE];
	int nonce;
};

/**
 * Structures de données rajoutant des informations à un block
 * À utiliser dans la Blockchain
 */
typedef struct s_chainElem {
	Block *block;
	char blockHash[SHA256_BLOCK_SIZE];
	struct s_chainElem *prev;
} ChainElement;

/**
 * Structure de données représentant la Blockchain
 */
struct s_blockchain {
	int difficulty;
	int blockCount;
	ChainElement *last;
};

/**
 * Initialisation d'une Blockchain.
 * @param bc Pointeur vers la blockchain à initialiser
 */
void blockchain(Blockchain *bc) {
	bc->blockCount = 0;
	bc->last = NULL;
}

/**
 * Initialisation d'un block.
 * @param b Pointeur vers le block à initialiser
 */
void block(Block *b) {

	b->nonce = 0;

}

/**
 * Définir la difficulté d'une Blockchain.
 * @param bc Pointeur vers la blockchain à modifier
 * @param diff Nouvelle difficulté
 */
void difficulty(Blockchain *bc, int diff) {
	bc->difficulty = diff;
}

/**
 * Ajoute une transaction à la liste de transactions d'un block.
 * @param b Pointeur vers le block à modifier
 * @param transaction Transaction à ajouter
 */
void addTransactionToBlock(Block *b, char transaction[TRANSACTION_LEN]) {
	addTransaction(&(b->transactions), transaction);
}

/**
 * Renvoie le hash du block donné sur 32 octets.
 * @param b Pointeur vers le block à lire
 * @param hash Reçoit le hash du bloc en sortie
 */
void getBlockHash(const Block *b, char hash[SHA256_BLOCK_SIZE]) {
	SHA256_CTX ctx;
	sha256_init(&ctx);
	sha256_update(&ctx, (BYTE *) b, sizeof(Block));
	sha256_final(&ctx, (BYTE *) hash);
}

/**
 * Vérifie que le hash reçu corresponde bien à la difficulté.
 * @param hash Hash à vérifier
 * @param difficulty Difficulté à satisfaire
 * @return Booléen, renvoie true si le hash correspond, false sinon.
 */
int verifyHash(const char hash[SHA256_BLOCK_SIZE], int difficulty) {
	for (int i = 0; i < difficulty; i++)
		if (hash[i] != '0')
			return 0;
	return 1;
}

/**
 * Incrémente la nonce d'un block jusqu'à ce que son hash corresponde à la difficulté.
 * @param b Pointeur vers le block à modifier
 * @param hash Renvoie le hash du bloc une fois la nonce trouvée
 * @param difficulty Difficulté de la blockchain
 */
void updateNonce(Block *b, char hash[SHA256_BLOCK_SIZE], int difficulty) {
	getBlockHash(b, hash);
	while (!verifyHash(hash, difficulty)) {
		(b->nonce)++;
		getBlockHash(b, hash);
	}
}

/**
 * Ajoute un block à une Blockchain.
 * @param bc Pointeur vers la blockchain à modifier
 * @param b Block à ajouter
 */
void addBlock(Blockchain *bc, Block *b) {

	ChainElement *ce = malloc(sizeof(ChainElement));
	ce->block = b;

	//Finalisation des informations du bloc
	if (bc->last == NULL) {
		//Nettoyage du hash car il n'y a pas de bloc précédent
		for (int i = 0; i < SHA256_BLOCK_SIZE; i++)
			b->previousHash[i] = '\0';
	} else {
		//Informations pour les blocs suivants
		memcpy(b->previousHash, bc->last->blockHash, SHA256_BLOCK_SIZE);
	}
	b->index = bc->blockCount;
	//b->timestamp = ??? (TODO)

	//Calcul du hash
	updateNonce(b, ce->blockHash, bc->difficulty);

	//Ajout du bloc à la blockchain
	ce->prev = bc->last;
	bc->last = ce;
	(bc->blockCount)++;

}
