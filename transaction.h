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
#define TRANSACTION_LEN 40
#define STR_TRANSACTIONLIST_LEN 330

typedef Deque TransactionList;
typedef char * Transaction;

/**
 * Ajout d'une transaction à un TransactionBlock.
 * @param tl Pointeur vers le TransactionBlock à modifier
 * @param transaction Transaction à ajouter
 */
void addTransaction(TransactionList *tl, Transaction transaction);

/**
 * Vérifie si le TransactionBlock est plein
 * @param tl Pointeur vers le TransactionBlock à vérifier
 * @return Booléen, renvoie true si le TransactionBlock est plein, false sinon
 */
int transactionListIsFull(const TransactionList *tl);

/**
 * Transforme une liste de transactions en chaîne de caractères.
 * @param tl Liste à transformer
 * @return La liste transformée
 */
char *transactionsToString(const TransactionList *tl);

/**
 * Affiche le contenu d'une transaction. Peut être utilisé par dequeMap().
 * @param vt La transaction à afficher
 */
void afficherTransaction(void *vt);

/**
 * Calcul de la merkle root d'un TransactionBlock.
 * @param tl Pointeur vers le TransactionBlock à lire
 * @param root Renvoie la merkleRoot du TransactionBlock
 */
void calcMerkleRoot(const TransactionList *tl, char hash[SHA256_BLOCK_SIZE*2 + 1]);

#endif /* TRANSACTION_H_ */
