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
#define STR_BLOCK_LEN 420

typedef struct s_blockchain Blockchain;
typedef struct s_block Block;

/**
 * Initialisation d'une Blockchain.
 * @param difficulty difficulté de la blockchain
 */
Blockchain *blockchain(int difficulty);

/**
 * Initialisation d'un block.
 * @param index Index du block dans la blockchain
 * @param previousHash hash du block précédent dans la blockchain. NULL si ce block est le génésis.
 */
Block *block();

/**
 * Revoie le hash du block.
 * @return Hash du block
 */
char *getBlockHash(Block *b);	//TODO je crois que ça sert à rien

/**
 * Ajoute une transaction à la liste de transactions d'un block.
 * @param b Pointeur vers le block à modifier
 * @param transaction Transaction à ajouter
 */
void addTransactionToBlock(Block *b, char transaction[TRANSACTION_LEN]);

/**
 * Transforme un block en chaîne de caractères.
 * @param b Block à transformer
 * @return Block transformé
 */
char *blockToString(const Block *b);

/**
 * Renvoie le hash du block donné sur 32 octets.
 * @param b Pointeur vers le block à modifier
 * @param hash Reçoit le hash du bloc en sortie
 */
void calcBlockHash(const Block *b, char hash[SHA256_BLOCK_SIZE*2 + 1]);

/**
 * Vérifie que le hash reçu corresponde bien à la difficulté.
 * @param hash Hash à vérifier
 * @param difficulty Difficulté à satisfaire
 * @return Booléen, renvoie true si le hash correspond, false sinon.
 */
int verifyHash(const char hash[SHA256_BLOCK_SIZE*2 + 1], int difficulty);

/**
 * Incrémente la nonce d'un block jusqu'à ce que son hash corresponde à la difficulté.
 * @param b Pointeur vers le block à modifier
 * @param hash Renvoie le hash du bloc une fois la nonce trouvée
 * @param difficulty Difficulté de la blockchain
 */
void calcTrueBlockHash(Block *b, char hash[SHA256_BLOCK_SIZE*2 + 1], int difficulty);

/**
 * Calcule la Merkle Root des transactions du block et la range dans la variable à cet effet
 */
void calcBlockMerkleRoot(Block *b);

/**
 * Ajoute un block à une Blockchain.
 * @param bc Pointeur vers la blockchain à modifier
 * @param b Block à ajouter
 */
void addBlock(Blockchain *bc, Block *b);

#endif /* BLOCKCHAIN_H_ */
