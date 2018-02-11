/*
 * block.c
 *
 *  Created on: 2 févr. 2018
 *      Author: Pierre
 */

//A IGNORER pour le moment, leur contenu est géré par les fichiers blockchain, je les garde au cas où

#include <stdlib.h>
#include <sha256/sha256.h>
#include <sha256/sha256_utils.h>

#include "block.h"

/*
 * Structure de données représentant un bloc
 */
struct s_block {
	int index;
	char timestamp[30];
	char previousHash[32];
	int transactionCount;
	char merkleRoot[32];
	char hash[32];
	int nonce;
};


void init_block(Block b) {
	b->index = 0;
	//b->timestamp = ???

}
