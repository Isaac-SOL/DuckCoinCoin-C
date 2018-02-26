/*
 * blockchain.c
 *
 *  Created on: 2 févr. 2018
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
 * Structure de données représentant un bloc
 */
struct s_block {
	int index;
	char timestamp[TIMESTAMP_LEN];
	char previousHash[SHA256_BLOCK_SIZE];
	TransactionList *transactions;
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
	setTimeStamp(b);
	b->nonce = 0;

}

/**
 * Renvoie la date à l'appel de la fonction.
 * @return date actuelle dans une chaîne de caractères
 */
char *getTimeStamp() {
	time_t ltime;
	time(&ltime);
	return ctime(&ltime);
}

/**
 * Initialise le timestamp d'un Block.
 * @param b Block duquel il faut initialiser le timestamp
 */
void setTimeStamp(Block *b) {
    char *ch;
    ch = getTimeStamp();
    strcpy(b->timestamp, ch);
    free(ch);
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
	addTransaction(b->transactions, transaction);
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
 * Incrémente la nonce d'un block jusqu'à ce que son hash corresponde à la difficultÃ©.
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
void addBlock(Blockchain * bc, Block * b) {

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

/**
 * @author Chasse (Nicolas) generation random de blocs [wip]
 */

/**
 * generation random(aleat) de blocks
 * @return un bloc genere aleatoirement
 */
Block *random_block(){
	srand(time(NULL));
	Block *b= malloc(sizeof(Block));
	block(b);
	b->index = rand();
	b->transactions = random_tb();
	merkleRoot(b->transactions, b->merkleRoot);

	return b;
} // pas du tout sur de l'utilisation de la merkle root et

/**
 * add: verifier qu'un bloc n'est pas identique a un autre ?
 * @return bc une block chain aleatoire de taille aleatoire(a modif)
 */
Blockchain random_bc(){
	srand(time(NULL));
	int r = (rand()%(500-1))+1; //define une taille p-e ?
	Blockchain *bc = malloc(sizeof(Blockchain));
	for (int i = 0; i<r; i++){
		Block *b = random_block();
		addBlock(bc, b);
	}
	return *bc;
}
