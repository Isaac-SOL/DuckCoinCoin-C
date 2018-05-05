/*
 * blockchain.c
 *
 *  Created on: 2 févr. 2018
 *      Author: Pierre
 */

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "sha256/sha256_utils.h"
#include "json/json.h"
#include "blockchain.h"
#include "transaction.h"
#include "util.h"

/* ********************* *\
|* Structures de Données *|
\* ********************* */

/**
 * Structure de données représentant un bloc
 */
struct s_block {
	int index;
	char *timestamp;
	char *previousHash;
	char *currentHash;
	TransactionList *transactions;
	char *merkleRoot;
	int nonce;
};

/**
 * Structure de données représentant la Blockchain
 */
struct s_blockchain {
	int difficulty;
	Deque *blocks;
};

/* ************* *\
|* Constructeurs *|
\* ************* */

/**
 * Initialisation d'une Blockchain.
 * @param difficulty difficulté de la blockchain
 */
Blockchain *blockchain(int difficulty) {
	Blockchain *bc = malloc(sizeof(Blockchain));
	if (bc == NULL) {
		printf("Erreur d'allocation memoire pour blockchain.\n");
		exit(1);
	}

	bc->difficulty = difficulty;
	bc->blocks = deque();
	return bc;
}

/**
 * Initialisation d'un block.
 */
Block *block() {

	Block *b = malloc(sizeof(Block));
	if (b == NULL) {
		printf("Erreur d'allocation memoire pour block.\n");
		exit(1);
	}
	
	b->previousHash = malloc((SHA256_BLOCK_SIZE*2 + 1) * sizeof(char));
		if (b->previousHash == NULL) {
			printf("Erreur d'allocation memoire pour block.\n");
			exit(1);
		}
	b->currentHash = malloc((SHA256_BLOCK_SIZE*2 + 1) * sizeof(char));
	if (b->currentHash == NULL) {
		printf("Erreur d'allocation memoire pour block.\n");
		exit(1);
	}
	b->merkleRoot = malloc((SHA256_BLOCK_SIZE*2 + 1) * sizeof(char));
	if (b->merkleRoot == NULL) {
		printf("Erreur d'allocation memoire pour block.\n");
		exit(1);
	}
	b->transactions = deque();
	b->timestamp = getTimeStamp();	//TODO est-ce qu'on fait ça ici ou à l'ajout dans la blockchain?
	b->nonce = 0;

	return b;
}

/* ************ *\
|* Destructeurs *|
\* ************ */

/**
 * Désalloue tout le contenu d'un block. Ne désalloue pas le block en lui-même.
 * @param b Block à supprimer
 */
void freeBlockContent(Block *b) {
    free(b->timestamp);
    free(b->previousHash);
    free(b->currentHash);
    free(b->merkleRoot);
    delete_deque(b->transactions);
    //Note: pas de free(b), sera fait par delete_deque()
}

/**
 * Désalloue totalement une Blockchain ainsi que tout son contenu.
 * @param bc Blockchain à désallouer
 */
void freeBlockchain(Blockchain *bc) {
    for (int i = 0; i < dequeSize(bc->blocks); i++)
        freeBlockContent((Block *) ith(bc->blocks, i));
    delete_deque(bc->blocks);
    free(bc);
}

/* ******************* *\
|* Opérateurs de Block *|
\* ******************* */

/**
 * Revoie le hash du block.
 * @return Hash du block
 */
char *getBlockHash(Block *b) {	//TODO je crois que ça sert à rien
	return b->currentHash;
}

/**
 * Ajoute une transaction à la liste de transactions d'un block.
 * @param b Pointeur vers le block à modifier
 * @param transaction Transaction à ajouter
 */
void addTransactionToBlock(Block *b, char transaction[TRANSACTION_LEN]) {
	addTransaction(b->transactions, transaction);
}

/**
 * Renvoie le hash du block donné sur 32 octets.
 * @param b Pointeur vers le block à lire
 * @param hash Reçoit le hash du bloc en sortie
 */
void calcBlockHash(const Block *b, char hash[SHA256_BLOCK_SIZE*2 + 1]) {
	sha256ofString((BYTE *) blockToString(b), hash);
}

/**
 * Vérifie que le hash reçu corresponde bien à la difficulté.
 * @param hash Hash à vérifier
 * @param difficulty Difficulté à satisfaire
 * @return Booléen, renvoie true si le hash correspond, false sinon.
 */
int verifyHash(const char hash[SHA256_BLOCK_SIZE*2 + 1], int difficulty) {
	for (int i = 0; i < difficulty; i++)
		if (hash[i] != '0')
			return 0;
	return 1;
}

/**
 * Incrémente la nonce d'un block jusqu'à ce que son hash corresponde à la difficulté.
 * @param b Pointeur vers le block à modifier
 * @param hash Renvoie le hash du bloc une fois la nonce trouvée
 * @param difficulty Difficulté de la blockchain
 */
void calcTrueBlockHash(Block *b, char hash[SHA256_BLOCK_SIZE*2 + 1], int difficulty) {
	b->nonce = 0;
	calcBlockHash(b, hash);
	while (!verifyHash(hash, difficulty)) {
		(b->nonce)++;
		calcBlockHash(b, hash);
	}
}

/**
 * Calcule la Merkle Root des transactions du block et la range dans la variable à cet effet
 */
void calcBlockMerkleRoot(Block *b) {
	calcMerkleRoot(b->transactions, b->merkleRoot);
}

/* ************************ *\
|* Opérateurs de Blockchain *|
\* ************************ */

/**
 * Ajoute le block Génésis à une blockchain vide.
 * @param bc Blockchain
 */
void addGenesis(Blockchain *bc) {
	assert(dequeEmpty(bc->blocks));
	Block *b = block();
	b->index = 0;
	strcpy(b->previousHash, "0");

	char *transaction = malloc(TRANSACTION_LEN * sizeof(char));
	strcpy(transaction, "Genesis");
	addTransactionToBlock(b, transaction);

	calcBlockMerkleRoot(b);
	calcBlockHash(b, b->currentHash);

	push_back(bc->blocks, b);
}

/**
 * Ajoute un block à une Blockchain.
 * @param bc Pointeur vers la blockchain à modifier
 * @param b Block à ajouter
 */
void addBlock(Blockchain *bc, Block *b) {

	//Ajout des informations liées à la Blockchain
	b->index = dequeSize(bc->blocks);
	strcpy(b->previousHash, ((Block *) back(bc->blocks))->currentHash);
	calcBlockMerkleRoot(b);

	//Calcul du hash
	calcTrueBlockHash(b, b->currentHash, bc->difficulty);

	//Ajout du bloc à la blockchain
	push_back(bc->blocks, b);
}

/* *************************** *\
|* Fonctions de Transformation *|
\* *************************** */

/**
 * Transforme un block en chaîne de caractères.
 * @param b Block à transformer
 * @return Block transformé
 */
char *blockToString(const Block *b) {	//TODO sûrement à revoir, mais on peut travailler avec
	char *result = malloc(STR_BLOCK_LEN * sizeof(char));
	if (result == NULL) {
		printf("Erreur d'allocation memoire pour blockToString.\n");
		exit(1);
	}
	sprintf(result, "%d,%s,%s,%d,%s", b->index, transactionsToString(b->transactions), b->previousHash, b->nonce, b->merkleRoot);
	return result;
}

/**
 * Affiche un hash.
 * @param hash Hash à afficher.
 */
void afficherHash(char hash[SHA256_BLOCK_SIZE*2 + 1]) {
	for (int i = 0; i < SHA256_BLOCK_SIZE*2 + 1; i++)
		printf("%c", hash[i]);
}

/**
 * Affiche le contenu d'un block. Peut être utilisé par dequeMap().
 * @param vb Block à afficher.
 */
void afficherBlock(void *vb) {
	Block *b = (Block *) vb;
	printf("  ----  BLOCK %d  ----\n", b->index);
	printf("Cree le %s\n", b->timestamp);
	printf("Transactions:\n");
	dequeMap(b->transactions, afficherTransaction);
	printf("Hash:           ");
	afficherHash(b->currentHash);
	printf("\nHash Precedent: ");
	afficherHash(b->previousHash);
	printf("\nMerkle Root:    ");
	afficherHash(b->merkleRoot);
	printf("\nNonce: %d\n\n", b->nonce);
}

/**
 * Affiche le contenu de la blockchain sur la sortie standard.
 * @param bc Blockchain dont il faut afficher le contenu.
 */
void afficherBlockchain(Blockchain *bc) {
	printf("+----------+\n|BLOCKCHAIN| Blocks: %d, Difficulte: %d\n+----------+\n\n", dequeSize(bc->blocks), bc->difficulty);
	dequeMap(bc->blocks, afficherBlock);
}

/* ************************* *\
|* Fonctions de Vérification *|
\* ************************* */

/**
 * Comparaison de deux hash : 0 = aucune différence, 1 sinon
 * @param chaine1 premier hash à comparer
 * @param chaine2 second hash à comparer
 * @return 0 si aucune différence, 1 sinon
 */
int compByte(char *chaine1, char *chaine2) {
	for (int i = 0; i < SHA256_BLOCK_SIZE*2 + 1; i++)
		if (chaine1[i] != chaine2[i])
			return 1;
	return 0;
}

/**
 * vérification 1 : la chaîne commece bien par le bloc génésis et que le
 * chaînage des hash est valide, et que le hash du bloc est bien celui annoncé.
 * retour : 0 = OK
 *			1 = Le premier block n'est pas génésis
 *			2 = Problème dans le chaînage des hash
 *			3 = problème hash du bloc
 */
int verifBlockchain(Blockchain *b) {
	Deque *d = b->blocks;
	Block *block = ith(d, dequeSize(d)-1);
	char *prevHash = block->currentHash;
	char hash[SHA256_BLOCK_SIZE*2 + 1];

	/* Parcourt tous les blocks sauf le génésis qui sera testé après le for
	 * Note: La fonction serait plus optimisée si on pouvait profiter des pointeurs entre les blocs.
	 * Éventuellement penser à créer une fonction iterator pour nos deque */
	for (int i = dequeSize(d)-1; i > -1; i--) {
		block = ith(d, i);
		/* test hash des blocks */
		calcBlockHash(block, hash);
		if (compByte(hash, block->currentHash) != 0) {
			return 3;
		}
		/* test chaînage des hash */
		if (compByte(getBlockHash(block), prevHash) != 0) {
			return 2;
		}
		if ( i!= 0) prevHash = block->previousHash;
	}

	/* Vérification que le premier block est le block génésis */
	if (strcmp(block->previousHash, "0") != 0 || block->nonce != 0 || dequeSize(block->transactions) != 1 || strcmp((char *) front(block->transactions), "Genesis") != 0)
		return 1;
	return 0;
}

/**
 * Vérification 2 : pour chaque block le hash Merkle Root correspond bien aux transactions de ce block : 0 = ok, 1 sinon
 */
int verifMerkleRoot(Blockchain *b) {
	int i;
	Deque *d = b->blocks;
	Block *block = front(d);
	char root[SHA256_BLOCK_SIZE*2 + 1];

	for (i = 0; i < dequeSize(d); i++) {
		block = ith(d, i);
		calcMerkleRoot(block->transactions, root);
		printf("\n");
		if (compByte(block->merkleRoot, root) != 0) {
			return 1;
		}
	}

	return 0;
}

/**
 * Affiche un message en fonction du code de validité d'une Blockchain.
 * Note: transformer les codes de retour pour pouvoir localiser les erreurs?
 * @param code Code de retour à analyser
 */
void messageValidite(int code) {
    switch (code) {
        case 0:
            printf("La blockchain est valide.\n");
            break;

        case 1:
            printf("La blockchain est invalide.\nLe premier Block n'est pas le Genesis.\n");
            break;

        case 2:
            printf("La blockchain est invalide.\nIl y a une erreur de chainage des hashes.\n");
            break;

        case 3:
            printf("La blockchain est invalide.\nUn des hash de Block est incorrect.\n");
            break;

        default:
            printf("La blockchain a une erreur surprenante.\n");
    }
}

/* ****************** *\
|* Fonctions de Cheat *|
\* ****************** */

/**
 * Cheater de block (suppression d'un block dans la blockchain) , avec Vérification 1 en sortie,
 * exception pour le block génésis (position 0) le block suivant prendra le rôle du block génésis
 * @param b Blockchain à modifier
 * @param num Index du block à supprimer
 */
void cheatBlock(Blockchain *b, int num) {
	double temps;
	clock_t start;
	start = clock();

	if (num < dequeSize(b->blocks) && num > 0) { //Vérification que l'index est valide
		printf("Suppression du Block #%d\n", num);
		int i;

		b->blocks = remove_at(b->blocks, num); //Suppression

		Block *prevBlock, *nextBlock;
		i = num;
		while(i < dequeSize(b->blocks)) { //Modification en chaîne des blocks suivants pour l'intégrité
			printf("Re-minage du block #%d...\n", i);
			nextBlock = ith(b->blocks, i);
			prevBlock = ith(b->blocks, i-1);
			nextBlock->index = i;
			nextBlock->previousHash = prevBlock->currentHash;
			calcTrueBlockHash(nextBlock, nextBlock->currentHash, b->difficulty);
			i++;
		}

		temps = (double) (clock() - start) / (double) CLOCKS_PER_SEC;
		printf("Temps d'execution : %.2f secondes\n\n", temps);

	}
	else printf("Le Block #%d n'existe pas. Aucune modification effectuee.\n", num);
}

/**
 * Cheater de Transaction (suppression d'une transaction dans un block de la blockchain) , avec Vérification 2 en sortie,
 * exception pour le block génésis (position 0)
 * @param b Blockchain à modifier
 * @param numB Numéro du block à modifier
 * @param numT Numéro de la transaction à supprimer
 */
void cheatTransaction(Blockchain *b, int numB, int numT) {
	double temps;
	clock_t start;
	start = clock();

	if (numB < dequeSize(b->blocks) && numB > 0) { //Vérification de l'index
		Block *t = ith(b->blocks, numB);

		int i;
		if (numT < dequeSize(t->transactions) && numT >= 0 && dequeSize(t->transactions) > 1) { //Vérification de l'index de transaction
			printf("Suppression de la transaction #%d dans le Block #%d\n", numT, numB);
			t->transactions = remove_at(t->transactions, numT); //Suppression

			i = numB;
			Block *prevBlock, *nextBlock;
			while (i < dequeSize(b->blocks)) { //Modification en chaîne des blocks suivants pour l'intégrité
				printf("Re-minage du block #%d...\n", i);
				nextBlock = ith(b->blocks, i);
				prevBlock = ith(b->blocks, i-1);
				calcBlockMerkleRoot(nextBlock);
				nextBlock->previousHash = prevBlock->currentHash;
				calcTrueBlockHash(nextBlock, nextBlock->currentHash, b->difficulty);
				i++;
			}

			temps = (double) (clock() - start) / (double) CLOCKS_PER_SEC;
			printf("Temps d'execution : %.2f secondes\n\n", temps);

		}
		else printf("La Transaction #%d n'existe pas ou est la seule dans le Block #%d.\nAucune modification effectuee.\n", numT, numB);
	}
	else printf("Le Block #%d n'existe pas. Aucune modification effectuee.\n", numB);
}

/* **** *\
|* JSON *|
\* **** */

/**
 * Transforme une json_value en TransactionList si elle est compatible.
 * @param tl TransactionList vide
 * @param value json_value représentant une TransactionList
 */
void TransactionsFromJSON(TransactionList *tl, json_value *value) {

	if (value->type != json_array) {
		printf("Erreur : Transactions non array\n");
		return;
	}

	for (int i = 0; i < value->u.array.length; i++) {
		Transaction t = malloc(TRANSACTION_LEN * sizeof(char));
		strcpy(t, value->u.array.values[i]->u.string.ptr);
		addTransaction(tl, t);
	}
}

/**
 * Transforme une json_value en Block si elle est compatible.
 * @param b Block vide
 * @param value json_value représentant un Block
 */
void BlockFromJSON(Block *b, json_value *value) {

	if (value->type != json_object) {
		printf("Erreur : Block non object\n");
		return;
	}

	for (int i = 0; i < value->u.object.length; i++) {

		if (!strcmp(value->u.object.values[i].name, "index")) {
			b->index = value->u.object.values[i].value->u.integer;

		} else if (!strcmp(value->u.object.values[i].name, "timestamp")) {
			strcpy(b->timestamp, value->u.object.values[i].value->u.string.ptr);

		} else if (!strcmp(value->u.object.values[i].name, "nonce")) {
			b->nonce = value->u.object.values[i].value->u.integer;

		} else if (!strcmp(value->u.object.values[i].name, "hash")) {
			memcpy(b->currentHash, value->u.object.values[i].value->u.string.ptr, SHA256_BLOCK_SIZE*2 + 1);

		} else if (!strcmp(value->u.object.values[i].name, "prevHash")) {
			memcpy(b->previousHash, value->u.object.values[i].value->u.string.ptr, SHA256_BLOCK_SIZE*2 + 1);

		} else if (!strcmp(value->u.object.values[i].name, "merkleRoot")) {
			memcpy(b->merkleRoot, value->u.object.values[i].value->u.string.ptr, SHA256_BLOCK_SIZE*2 + 1);

		} else if (!strcmp(value->u.object.values[i].name, "transactions")) {
			TransactionsFromJSON(b->transactions, value->u.object.values[i].value);
		}

	}

}

/**
 * Transforme une json_value en liste de Blocks si elle est compatible.
 * @param blocks Liste de Blocks vide
 * @param value json_value représentant une liste de Blocks
 */
void BlocksFromJSON(Deque *blocks, json_value *value) {

	if (value->type != json_array) {
		printf("Erreur : Blocks non array\n");
		return;
	}

	for (int i = 0; i < value->u.array.length; i++) {
		Block *b = block();
		BlockFromJSON(b, value->u.array.values[i]);
		push_back(blocks, b);
	}
}

/**
 * Crée une Blockchain à partr de la json_value donnée si elle est compatible.
 * @param value json_value représentant une Blockchain
 * @return La blockchain créée
 */
Blockchain *BCfromJSON(json_value *value) {
	Blockchain *bc = blockchain(1);

	if (value->type != json_object) {
		printf("Erreur : Blockchain non object\n");
		return NULL;
	}

	for (int i = 0; i < value->u.object.length; i++) {
		if (!strcmp(value->u.object.values[i].name, "difficulte")) {
			bc->difficulty = value->u.object.values[i].value->u.integer;
		} else if (!strcmp(value->u.object.values[i].name, "blocks")) {
			BlocksFromJSON(bc->blocks, value->u.object.values[i].value);
		}
	}

	json_value_free(value);

	return bc;
}

/**
 * Écrit le contenu du la TransactionList donnée au format JSON dans le fichier donné.
 * @param tl TransactionList à transformer
 * @param f Fichier dans lequel écrire
 */
void TransactionsToJSON(TransactionList *tl, FILE *f) {
	fprintf(f, "[\n");
	for (int i = 0; i < dequeSize(tl) - 1; i++)
		fprintf(f, "        \"%s\",\n", ith(tl, i));
	fprintf(f, "        \"%s\"\n", back(tl));
	fprintf(f, "      ]");
}

/**
 * Écrit le contenu du Block donné au format JSON dans le fichier donné.
 * @param b Block à transformer
 * @param f Fichier dans lequel écrire
 */
void BlockToJSON(Block *b, FILE *f) {
	fprintf(f, "    {\n");
	fprintf(f, "      \"index\": %d,\n", b->index);
	fprintf(f, "      \"nonce\": %d,\n", b->nonce);
	fprintf(f, "      \"timestamp\": \"%s\",\n", b->timestamp);
	fprintf(f, "      \"hash\": \"%s\",\n", b->currentHash);
	fprintf(f, "      \"prevHash\": \"%s\",\n", b->previousHash);
	fprintf(f, "      \"merkleRoot\": \"%s\",\n", b->merkleRoot);
	fprintf(f, "      \"transactions\": ");
	TransactionsToJSON(b->transactions, f);
	fprintf(f, "\n    }");
}

/**
 * Écrit le contenu de la liste de Blocks donnée au format JSON dans le fichier donné.
 * @param blocks liste de Blocks à transformer
 * @param f Fichier dans lequel écrire
 */
void BlocksToJSON(Deque *blocks, FILE *f) {
	fprintf(f, "[\n");
	for (int i = 0; i < dequeSize(blocks) - 1; i++) {
		BlockToJSON(ith(blocks, i), f);
		fprintf(f, ",\n");
	}
	BlockToJSON(back(blocks), f);
	fprintf(f, "\n  ]");
}

/**
 * Écrit le contenu de la Blockchain donnée au format JSON dans le fichier donné.
 * @param bc Blockchain à transformer
 * @param f Fichier dans lequel écrire
 */
void BCtoJSON(Blockchain *bc, FILE *f) {
	fprintf(f, "{\n");
	fprintf(f, "  \"difficulte\": %d,\n", bc->difficulty);
	fprintf(f, "  \"blocks\": ");
	BlocksToJSON(bc->blocks, f);
	fprintf(f, "\n}\n");
}
