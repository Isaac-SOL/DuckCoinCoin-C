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

/*
 * Initialisation d'une Blockchain.
 */
void blockchain(Blockchain *bc);

/*
 * Initialisation d'un bloc.
 */
void block(Block *b);

/* Définir la difficulté d'une Blockchain
 * int diff :	Nouvelle difficulté
 */
void difficulty(Blockchain *bc, int diff);

/* Renvoie le hash du block sur 32 octets
 * char *hash :	Reçoit le hash du bloc en sortie
 */
void getBlockHash(Block *b, char hash[SHA256_BLOCK_SIZE]);

/* Vérifie que le hash reçu corresponde bien à la difficulté
 * char *hash :		Hash à vérifier
 * int difficulty :	Difficulté de la blockchain
 */
int verifyHash(char hash[SHA256_BLOCK_SIZE], int difficulty);

/* Incrémente la nonce du bloc jusqu'à ce que son hash corresponde à la difficulté
 * char *hash :		Renvoie le hash du bloc une fois la nonce trouvée
 * int difficulty :	Difficulté de la blockchain
 */
void updateNonce(Block *b, char hash[SHA256_BLOCK_SIZE], int difficulty);

/* Copie les transactions depuis une liste vers un block
 * TransactionBlock *tb :	Liste des transactions à copier
 * Block *b :				Block dans lequel copier
 */
void copyTransactions(TransactionBlock *tb, Block *b);

/*
 * Ajoute un bloc à la Blockchain.
 */
void addBlock(Blockchain *bc, Block *b);

#endif /* BLOCKCHAIN_H_ */
