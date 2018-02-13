/*
 * transaction.h
 *
 *  Created on: 3 févr. 2018
 *      Author: Pierre
 */

#ifndef TRANSACTION_H_
#define TRANSACTION_H_

#define MAX_TRANSACTIONS 16	//DOIT être une puissance de 2
#define TRANSACTION_LEN 30

typedef struct s_transactionBlock TransactionBlock;

/*
 * Initialisation d'une liste de transactions.
 */
void transactionBlock(TransactionBlock *tb);

/*
 * Ajout d'une transaction dans la liste
 */
void addTransaction(TransactionBlock *tb, char t[TRANSACTION_LEN]);

/*
 * Calcul de la merkle root de la liste de transactions
 */
void merkleRoot(TransactionBlock *tb, char hash[SHA256_BLOCK_SIZE]);

#endif /* TRANSACTION_H_ */
