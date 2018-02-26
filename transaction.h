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

typedef struct s_transactionList TransactionList;

/**
 * Initialisation d'un TransactionBlock.
 * @param tb Pointeur vers le TransactionBlock à initialiser
 */
void transactionBlock(TransactionList *tb);

/**
 * Ajout d'une transaction à un TransactionBlock.
 * @param tb Pointeur vers le TransactionBlock à modifier
 * @param transaction Transaction à ajouter
 */
void addTransaction(TransactionList *tb, const char transaction[TRANSACTION_LEN]);

/**
 * Vérifie si le TransactionBlock est plein
 * @param tb Pointeur vers le TransactionBlock à vérifier
 * @return Booléen, renvoie true si le TransactionBlock est plein, false sinon
 */
int isFull(const TransactionList *tb);

/**
 * Renvoie le nombre de transactions dans un TransactionBlock.
 * @param tb Pointeur vers le TransactionBlock à lire
 * @return Nombre de transactions
 */
int getTransactionCount(const TransactionList *tb);

/**
 * Renvoie la transaction présente à l'index donné.
 * @param tb Pointeur vers le TransactionBlock à lire
 * @param i Index de la transaction
 * @return Transaction à l'index i
 */
const char *getTransactionAt(const TransactionList *tb, int i);

/**
 * Calcul de la merkle root d'un TransactionBlock.
 * @param tb Pointeur vers le TransactionBlock à lire
 * @param root Renvoie la merkleRoot du TransactionBlock
 */
void merkleRoot(const TransactionList *tb, char hash[SHA256_BLOCK_SIZE]);


/**
 * creation transaction block random
 * @return tb un bloc de transaction random
 */
TransactionList *random_tb();

#endif /* TRANSACTION_H_ */
