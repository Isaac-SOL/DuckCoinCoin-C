/**
 * randomGen.c
 *
 * original: 07/04/2018
 * remade: 19/04/2018
 * @author Nicolas
 *
 *
 * generation de hash / blockchain / transaction aleatoires
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "randomGen.h"

#include "sha256/sha256_utils.h"
#include "blockchain.h"
#include "transaction.h"
#include "deque.h"
#include "util.h"


/**
 * @author Nicolas generation random de blocs refait à 5h après problemes avec mon ordi soyez indulgent ;-;
 */

/**
 * generation transaction random
 * @return transaction aleatoire
 */
Transaction random_trans(){
	srand(time(NULL));
	int i = (rand()%(330-1))+1;
	Transaction t; //Il faut allouer la mémoire avant le sprintf!
	sprintf(t, "Source-Destination: %d", i);
	return t;
}
/*//pas sur de devoir faire ca
TransactionList *random_tl(){
	srand(time(NULL));
	int sizet = (rand()%(330-1))+1; // taille TransactionList
	int temp;
	TransactionList *tl = deque();
	for (int i = 1; i<sizet, ++i){
		temp = sizet-i;
		Transaction *t;
		sprintf(t, "Source-Destination: %d", temp);
		push_back(tl, t);
	}
}
*/

/**
 * generation random(aleat) de blocks
 * @return un bloc genere aleatoirement
 */
Block *random_block(){
	Block *b = block();
	//char hash;  -- "Inutilisé" selon le compilateur
	Transaction t = random_trans();
	addTransactionToBlock(b, t);
	return b;
}

/**
 * add: verifier block et transaction differentes     + utilisation des fonctions
 * @return bc une block chain aleatoire de taille aleatoire(a modif)
 */
Blockchain *random_blockchain(){
	srand(time(NULL));
	int r = (rand()%(420-1))+1;
	Blockchain *bc = blockchain(r);
	for (int i = 0; i < r; i++) {
		if (i == 0)
			addGenesis(bc);
		Block *b = random_block();
		addBlock(bc, b);
	}
	return bc;
}


