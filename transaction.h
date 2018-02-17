/*
 * transaction.h
 *
 *  Created on: 3 févr. 2018
 *      Author: Pierre
 */

#ifndef TRANSACTION_H_
#define TRANSACTION_H_

#include "sha256/sha256.h"

#define MAX_TRANSACTIONS 16	//DOIT être une puissance de 2
#define TRANSACTION_LEN 30

typedef struct s_transactionBlock TransactionBlock;

/*
 * Initialisation d'une liste de transactions.
 */
void transactionBlock(TransactionBlock *tb);

/**
 * Renvoie le nombre de transactions.
 * @param tb Block de transactions à lire
 * @return Nombre de transactions
 */
int getTransactionCount(TransactionBlock *tb);

/**
 * Renvoie la transaction présente à l'index donné.
 * @param tb Block de transactions à lire
 * @param i Index de la transaction
 * @return Transaction à l'index i
 */
char *getTransactionAt(TransactionBlock *tb, int i);

/*
 * Ajout d'une transaction dans la liste
 */
void addTransaction(TransactionBlock *tb, char t[TRANSACTION_LEN]);

/*
 * Calcul de la merkle root de la liste de transactions
 */
void merkleRoot(TransactionBlock *tb, char hash[SHA256_BLOCK_SIZE]);

#endif /* TRANSACTION_H_ */
