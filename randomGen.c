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
 * @author Nicolas
 * Generation random de blocs refait à 5h après problemes avec mon ordi soyez indulgent ;-;
 */

/**
 * generation transaction random
 * @return transaction aleatoire
 */
Transaction random_trans(){
	srand(time(NULL));
	int i = (rand()%(330-1))+1;
	Transaction t = malloc(TRANSACTION_LEN * sizeof(char));
	sprintf(t, "Source-Destination: %d", i);
	return t;
}

/**
 * generation random(aleat) de blocks
 * @return un bloc genere aleatoirement
 */
Block *random_block(){
	srand(time(NULL));
	int numTrans = (rand() % (MAX_TRANSACTIONS - 1)) + 1;
	Block *b = block();

	for (int i = 0; i < numTrans; i++) {
		Transaction t = random_trans();
		addTransactionToBlock(b, t);
	}

	return b;
}

/**
 * add: verifier block et transaction differentes     + utilisation des fonctions
 * @return bc une block chain aleatoire de taille aleatoire(a modif)
 */
Blockchain *random_blockchain(int difficulty, int nbBlocks){
	Blockchain *bc = blockchain(difficulty);
	addGenesis(bc);

	for (int i = 1; i < nbBlocks; i++) {
		Block *b = random_block();
		addBlock(bc, b);
	}

	return bc;
}


