/*
 * transaction.h
 *
 *  Created on: 3 févr. 2018
 *      Author: Pierre
 */

#ifndef TRANSACTION_H_
#define TRANSACTION_H_

#include "sha256/sha256.h"
#include "deque.h"

#define MAX_TRANSACTIONS 10
#define TRANSACTION_LEN 30

typedef Deque TransactionList;
typedef char * Transaction;

/**
 * Ajout d'une transaction à un TransactionBlock.
 * @param tb Pointeur vers le TransactionBlock à modifier
 * @param transaction Transaction à ajouter
 */
void addTransaction(TransactionList *tl, Transaction transaction);

/**
 * Vérifie si le TransactionBlock est plein
 * @param tb Pointeur vers le TransactionBlock à vérifier
 * @return Booléen, renvoie true si le TransactionBlock est plein, false sinon
 */
int isFull(const TransactionList *tl);

/**
 * Calcul de la merkle root d'un TransactionBlock.
 * @param tb Pointeur vers le TransactionBlock à lire
 * @param root Renvoie la merkleRoot du TransactionBlock
 */
void merkleRoot(const TransactionList *tl, char hash[SHA256_BLOCK_SIZE]);


/**
 * creation transaction block random
 * @return tb un bloc de transaction random
 */
TransactionList *random_tb();

#endif /* TRANSACTION_H_ */
