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

#include "sha256/sha256_utils.h"
#include "blockchain.h"
#include "transaction.h"
#include "util.h"

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
 * @param difficulty difficulté de la blockchain
 */
Blockchain *blockchain(int difficulty) {
	Blockchain *bc = malloc(sizeof(Blockchain));
	bc->difficulty = difficulty;
	bc->blocks = deque();
	return bc;
}

/**
 * Initialisation d'un block.
 */
Block *block() {

	Block *b = malloc(sizeof(Block));
	
	b->previousHash = malloc(SHA256_BLOCK_SIZE * sizeof(char));
		if (b->previousHash == NULL) {
			printf("Erreur d'allocation mémoire pour block.\n");
			exit(1);
		}
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
	b->timestamp = getTimeStamp();	//TODO est-ce qu'on fait ça ici ou à l'ajout dans la blockchain?
	b->nonce = 0;

	return b;
}

/**
 * Revoie le hash du block.
 * @return Hash du block
 */
char *getBlockHash(Block *b) {	//TODO je crois que ça sert à rien
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
char *blockToString(const Block *b) {	//TODO sûrement à revoir, mais on peut travailler avec
	char *result = malloc(STR_BLOCK_LEN * sizeof(char));
	if (result == NULL) {
		printf("Erreur d'allocation mémoire pour blockToString.\n");
		exit(1);
	}
	sprintf(result, "%d,%s,%s,%d,%s", b->index, transactionsToString(b->transactions), b->previousHash, b->nonce, b->merkleRoot);
	return result;
}

/**
 * Renvoie le hash du block donné sur 32 octets.
 * @param b Pointeur vers le block à lire
 * @param hash Reçoit le hash du bloc en sortie
 */
void calcBlockHash(const Block *b, char hash[SHA256_BLOCK_SIZE]) {
	sha256ofString((BYTE *) blockToString(b), hash);
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
void calcTrueBlockHash(Block *b, char hash[SHA256_BLOCK_SIZE], int difficulty) {
	calcBlockHash(b, hash);
	while (!verifyHash(hash, difficulty)) {
		(b->nonce)++;
		calcBlockHash(b, hash);
	}
}

/**
 * Calcule la Merkle Root des transactions du block et la range dans la variable à cet effet
 */
void calcBlockMerkleRoot(Block *b) {
	calcMerkleRoot(b->transactions, b->merkleRoot);
}

/**
 * Ajoute un block à une Blockchain.
 * @param bc Pointeur vers la blockchain à modifier
 * @param b Block à ajouter
 */
void addBlock(Blockchain *bc, Block *b) {

	//Ajout des informations liées à la Blockchain
	b->index = dequeSize(bc->blocks);
	strcpy(b->previousHash, ((Block *) front(bc->blocks))->currentHash);
	calcBlockMerkleRoot(b);

	//Calcul du hash
	calcTrueBlockHash(b, b->currentHash, bc->difficulty);

	//Ajout du bloc à la blockchain
	push_front(bc->blocks, b);
}

///TODO Il faut mettre ça dans un fichier à part genre randomGeneration.c

/**
 * @author Chasse (Nicolas) generation random de blocs [wip]
 */

/**
 * generation random(aleat) de blocks
 * @return un bloc genere aleatoirement
 */
/*Block *random_block(int difficulty){
	srand(time(NULL));
	Block *b = block(rand(), difficulty);
	b->transactions = randomTransactionList();
	merkleRoot(b->transactions, b->merkleRoot);

	return b;
}*/ // pas du tout sur de l'utilisation de la merkle root et

/**
 * add: verifier qu'un bloc n'est pas identique a un autre ?
 * @return bc une block chain aleatoire de taille aleatoire(a modif)
 */
/*Blockchain random_bc(){
	srand(time(NULL));
	int r = (rand()%(500-1))+1; //define une taille p-e ?
	Blockchain *bc = malloc(sizeof(Blockchain));
	for (int i = 0; i<r; i++){
		Block *b = random_block();
		addBlock(bc, b);
	}
	return *bc;
}*/
