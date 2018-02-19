/*
 * blockchain.h
 *
 *  Created on: 2 févr. 2018
 *      Author: Pierre
 */

#ifndef BLOCKCHAIN_H_
#define BLOCKCHAIN_H_

#include "sha256/sha256.h"
#include "transaction.h"

typedef struct s_blockchain Blockchain;
typedef struct s_block Block;

/**
 * Initialisation d'une Blockchain.
 * @param bc Pointeur vers la blockchain à initialiser
 */
void blockchain(Blockchain *bc);

/**
 * Initialisation d'un block.
 * @param b Pointeur vers le block à initialiser
 */
void block(Block *b);

/**
 * Définir la difficulté d'une Blockchain.
 * @param bc Pointeur vers la blockchain à modifier
 * @param diff Nouvelle difficulté
 */
void difficulty(Blockchain *bc, int diff);

/**
 * Ajoute une transaction à la liste de transactions d'un block.
 * @param b Pointeur vers le block à modifier
 * @param transaction Transaction à ajouter
 */
void addTransactionToBlock(Block *b, char transaction[TRANSACTION_LEN]);

/**
 * Renvoie le hash du block donné sur 32 octets.
 * @param b Pointeur vers le block à modifier
 * @param hash Reçoit le hash du bloc en sortie
 */
void getBlockHash(const Block *b, char hash[SHA256_BLOCK_SIZE]);

/**
 * Vérifie que le hash reçu corresponde bien à la difficulté.
 * @param hash Hash à vérifier
 * @param difficulty Difficulté à satisfaire
 * @return Booléen, renvoie true si le hash correspond, false sinon.
 */
int verifyHash(const char hash[SHA256_BLOCK_SIZE], int difficulty);

/**
 * Incrémente la nonce d'un block jusqu'à ce que son hash corresponde à la difficulté.
 * @param b Pointeur vers le block à modifier
 * @param hash Renvoie le hash du bloc une fois la nonce trouvée
 * @param difficulty Difficulté de la blockchain
 */
void updateNonce(Block *b, char hash[SHA256_BLOCK_SIZE], int difficulty);

/**
 * Ajoute un block à une Blockchain.
 * @param bc Pointeur vers la blockchain à modifier
 * @param b Block à ajouter
 */
void addBlock(Blockchain *bc, Block *b);

#endif /* BLOCKCHAIN_H_ */
