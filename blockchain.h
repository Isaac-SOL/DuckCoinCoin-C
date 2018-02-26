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

#define TIMESTAMP_LEN 30

typedef struct s_blockchain Blockchain;
typedef struct s_block Block;

/**
 * Initialisation d'une Blockchain.
 * @param bc Pointeur vers la blockchain à initialiser
 * @param difficulty difficulté de la blockchain
 */
void blockchain(Blockchain *bc, int difficulty);

/**
 * Initialisation d'un block.
 * @param b Pointeur vers le block à initialiser
 * @param index Index du block dans la blockchain
 * @param previousHash hash du block précédent dans la blockchain. NULL si ce block est le génésis.
 */
void block(Block *b, int index, char *previousHash);


/**
 * Renvoie la date à l'appel de la fonction.
 * @return date actuelle dans une chaîne de caractères
 */
char *getTimeStamp(); //TODO juger de l'utilité de rendre cette fonction publique

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
void calcBlockHash(const Block *b, char hash[SHA256_BLOCK_SIZE]);

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
void addBlock(Blockchain *bc);

#endif /* BLOCKCHAIN_H_ */
