/*
 * blockchain.c
 *
 *  Created on: 2 fÃ©vr. 2018
 *      Author: Pierre
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "sha256/sha256.h"
#include "blockchain.h"
#include "transaction.h"

/**
 * Structure de donnÃ©es reprÃ©sentant un bloc
 */
struct s_block {
	int index;
	char timestamp[TIMESTAMP_LEN];
	char previousHash[SHA256_BLOCK_SIZE];
	TransactionBlock transactions;
	char merkleRoot[SHA256_BLOCK_SIZE];
	int nonce;
};

/**
 * Structures de donnÃ©es rajoutant des informations Ã  un block
 * Ã€ utiliser dans la Blockchain
 */
typedef struct s_chainElem {
	Block *block;
	char blockHash[SHA256_BLOCK_SIZE];
	struct s_chainElem *prev;
} ChainElement;

/**
 * Structure de donnÃ©es reprÃ©sentant la Blockchain
 */
struct s_blockchain {
	int difficulty;
	int blockCount;
	ChainElement *last;
};

/**
 * Initialisation d'une Blockchain.
 * @param bc Pointeur vers la blockchain Ã  initialiser
 */
void blockchain(Blockchain *bc) {
	bc->blockCount = 0;
	bc->last = NULL;
}

/**
 * Initialisation d'un block.
 * @param b Pointeur vers le block Ã  initialiser
 */
void block(Block *b) {
	setTimeStamp(b);
	b->nonce = 0;

}

/**
 * Return date a l appel de la fonction.
 */
char * getTimeStamp() {
	time_t ltime;
	time(&ltime);
	return ctime(&ltime);
}

/**
 * Initialise le timestamp au block b.
 */
void setTimeStamp(Block *b) {
    char *ch;
    ch = getTimeStamp();
    strcpy(b->timestamp, ch);
    free(ch);
}

/**
 * DÃ©finir la difficultÃ© d'une Blockchain.
 * @param bc Pointeur vers la blockchain Ã  modifier
 * @param diff Nouvelle difficultÃ©
 */
void difficulty(Blockchain *bc, int diff) {
	bc->difficulty = diff;
}

/**
 * Ajoute une transaction Ã  la liste de transactions d'un block.
 * @param b Pointeur vers le block Ã  modifier
 * @param transaction Transaction Ã  ajouter
 */
void addTransactionToBlock(Block *b, char transaction[TRANSACTION_LEN]) {
	addTransaction(&(b->transactions), transaction);
}

/**
 * Renvoie le hash du block donnÃ© sur 32 octets.
 * @param b Pointeur vers le block Ã  lire
 * @param hash ReÃ§oit le hash du bloc en sortie
 */
void getBlockHash(const Block *b, char hash[SHA256_BLOCK_SIZE]) {
	SHA256_CTX ctx;
	sha256_init(&ctx);
	sha256_update(&ctx, (BYTE *) b, sizeof(Block));
	sha256_final(&ctx, (BYTE *) hash);
}

/**
 * VÃ©rifie que le hash reÃ§u corresponde bien Ã  la difficultÃ©.
 * @param hash Hash Ã  vÃ©rifier
 * @param difficulty DifficultÃ© Ã  satisfaire
 * @return BoolÃ©en, renvoie true si le hash correspond, false sinon.
 */
int verifyHash(const char hash[SHA256_BLOCK_SIZE], int difficulty) {
	for (int i = 0; i < difficulty; i++)
		if (hash[i] != '0')
			return 0;
	return 1;
}

/**
 * IncrÃ©mente la nonce d'un block jusqu'Ã  ce que son hash corresponde Ã  la difficultÃ©.
 * @param b Pointeur vers le block Ã  modifier
 * @param hash Renvoie le hash du bloc une fois la nonce trouvÃ©e
 * @param difficulty DifficultÃ© de la blockchain
 */
void updateNonce(Block *b, char hash[SHA256_BLOCK_SIZE], int difficulty) {
	getBlockHash(b, hash);
	while (!verifyHash(hash, difficulty)) {
		(b->nonce)++;
		getBlockHash(b, hash);
	}
}

/**
 * Ajoute un block Ã  une Blockchain.
 * @param bc Pointeur vers la blockchain Ã  modifier
 * @param b Block Ã  ajouter
 */
void addBlock(Blockchain *bc, Block *b) {

	ChainElement *ce = malloc(sizeof(ChainElement));
	ce->block = b;

	//Finalisation des informations du bloc
	if (bc->last == NULL) {
		//Nettoyage du hash car il n'y a pas de bloc prÃ©cÃ©dent
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

	//Ajout du bloc Ã  la blockchain
	ce->prev = bc->last;
	bc->last = ce;
	(bc->blockCount)++;

}
