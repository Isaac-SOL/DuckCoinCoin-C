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
	char *timestamp;
	char *previousHash;
	char *currentHash;
	TransactionList *transactions;
	char *merkleRoot;
	int nonce;
};

/**
 * Structure de données représentant la Blockchain
 */
struct s_blockchain {
	int difficulty;
	Deque *blocks;
};

/**
 * Initialisation d'une Blockchain.
 * @param bc Pointeur vers la blockchain à initialiser
 * @param difficulty difficulté de la blockchain
 */
void blockchain(Blockchain *bc, int difficulty) {
	bc->difficulty = difficulty;
	bc->blocks = deque();
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
 * Initialisation d'un block.
 * @param b Pointeur vers le block à initialiser
 * @param index Index du block dans la blockchain
 * @param previousHash hash du block précédent dans la blockchain. NULL si ce block est le génésis.
 */
void block(Block *b, int index, char *previousHash) {

	if (previousHash == NULL) {
		for (int i = 0; i < SHA256_BLOCK_SIZE; i++)
			b->previousHash[i] = '\0';
	} else {
		b->previousHash = previousHash;
	}
	b->index = index;
	b->currentHash = malloc(SHA256_BLOCK_SIZE * sizeof(char));
	if (b->currentHash == NULL) {
		printf("Erreur d'allocation mémoire pour block.\n");
		exit(1);
	}
	b->merkleRoot = malloc(SHA256_BLOCK_SIZE * sizeof(char));
	if (b->merkleRoot == NULL) {
		printf("Erreur d'allocation mémoire pour block.\n");
		exit(1);
	}
	b->transactions = deque();
	b->timestamp = getTimeStamp();
	b->nonce = 0;

}

char *getBlockHash(Block *b) {
	return b->currentHash;
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
 * Transforme un block en chaîne de caractères.
 * @param b Block à transformer
 * @return Block transformé
 */
char *btoa(Block *b) {
	//TODO à créer
	return "TODO";
}

/**
 * Renvoie le hash du block donné sur 32 octets.
 * @param b Pointeur vers le block à lire
 * @param hash Reçoit le hash du bloc en sortie
 */
void calcBlockHash(const Block *b, char hash[SHA256_BLOCK_SIZE]) {
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
void updateNonce(Block *b, char hash[SHA256_BLOCK_SIZE], int difficulty) { //TOSDO adapter
	calcBlockHash(b, hash);
	while (!verifyHash(hash, difficulty)) {
		(b->nonce)++;
		calcBlockHash(b, hash);
	}
}

/**
 * Ajoute un block à une Blockchain.
 * @param bc Pointeur vers la blockchain à modifier
 * @param b Block à ajouter
 */
void addBlock(Blockchain *bc) {

	Block *b = malloc(sizeof(Block));
	block(b, dequeSize(bc->blocks), dequeEmpty(bc->blocks) ? NULL : getBlockHash(front(bc->blocks)));

	b->index = dequeSize(bc->blocks);

	//Calcul du hash
	//updateNonce(b, ce->blockHash, bc->difficulty); TODO adapter

	//Ajout du bloc à la blockchain
	push_front(bc->blocks, b);

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
