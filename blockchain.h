/*
 * blockchain.h
 *
 *  Created on: 2 févr. 2018
 *      Author: Pierre
 */

#ifndef BLOCKCHAIN_H_
#define BLOCKCHAIN_H_

#include "sha256/sha256.h"
#include "json/json.h"
#include "transaction.h"

#define TIMESTAMP_LEN 30
#define STR_BLOCK_LEN 420

/* ********************* *\
|* Structures de Données *|
\* ********************* */

typedef struct s_blockchain Blockchain;
typedef struct s_block Block;

/* ************* *\
|* Constructeurs *|
\* ************* */

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

/* ************ *\
|* Destructeurs *|
\* ************ */

/**
 * Désalloue tout le contenu d'un block. Ne désalloue pas le block en lui-même.
 * @param b Block à supprimer
 */
void freeBlockContent(Block *b);

/**
 * Désalloue totalement une Blockchain ainsi que tout son contenu.
 * @param bc Blockchain à désallouer
 */
void freeBlockchain(Blockchain *bc);

/* ******************* *\
|* Opérateurs de Block *|
\* ******************* */

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

/* ************************ *\
|* Opérateurs de Blockchain *|
\* ************************ */

/**
 * Ajoute le block Génésis à une blockchain vide.
 * @param bc Blockchain
 */
void addGenesis(Blockchain *bc);

/**
 * Ajoute un block à une Blockchain.
 * @param bc Pointeur vers la blockchain à modifier
 * @param b Block à ajouter
 */
void addBlock(Blockchain *bc, Block *b);

/* *************************** *\
|* Fonctions de Transformation *|
\* *************************** */

/**
 * Transforme un block en chaîne de caractères.
 * @param b Block à transformer
 * @return Block transformé
 */
char *blockToString(const Block *b);

/**
 * Affiche le contenu de la blockchain sur la sortie standard.
 * @param bc Blockchain dont il faut afficher le contenu.
 */
void afficherBlockchain(Blockchain *bc);

/* ************************* *\
|* Fonctions de Vérification *|
\* ************************* */

/**
 * Comparaison de deux hash : 0 = aucune différence, 1 sinon
 * @param chaine1 premier hash à comparer
 * @param chaine2 second hash à comparer
 * @return 0 si aucune différence, 1 sinon
 */
int compByte(char *chaine1, char *chaine2);

/**
 * vérification 1 : la chaîne commece bien par le bloc génésis et que le
 * chaînage des hash est valide, et que le hash du bloc est bien celui annoncé.
 * retour : 0 = OK
 *			1 = Le premier block n'est pas génésis
 *			2 = Problème dans le chaînage des hash
 *			3 = problème hash du bloc
 */
int verifBlockchain(Blockchain *b);

/**
 * Vérification 2 : pour chaque block le hash Merkle Root correspond bien aux transactions de ce block : 0 = ok, 1 sinon
 */
int verifMerkleRoot(Blockchain *b);

/**
 * Affiche un message en fonction du code de validité d'une Blockchain.
 * Note: transformer les codes de retour pour pouvoir localiser les erreurs?
 * @param code Code de retour à analyser
 */
void messageValidite(int code);

/* ****************** *\
|* Fonctions de Cheat *|
\* ****************** */

/**
 * Cheater de block (suppression d'un block dans la blockchain) , avec Vérification 1 en sortie,
 * exception pour le block génésis (position 0) le block suivant prendra le rôle du block génésis
 * @param b Blockchain à modifier
 * @param num Index du block à supprimer
 */
void cheatBlock(Blockchain *b, int num);

/**
 * Cheater de Transaction (suppression d'une transaction dans un block de la blockchain) , avec Vérification 2 en sortie,
 * exception pour le block génésis (position 0)
 * @param b Blockchain à modifier
 * @param numB Numéro du block à modifier
 * @param numT Numéro de la transaction à supprimer
 */
void cheatTransaction(Blockchain *b, int numB, int numT);

/* **** *\
|* JSON *|
\* **** */

/**
 * Crée une Blockchain à partr de la json_value donnée si elle est compatible.
 * @param value json_value représentant une Blockchain
 * @return La blockchain créée
 */
Blockchain *BCfromJSON(json_value *value);

/**
 * Écrit le contenu de la Blockchain donnée au format JSON dans le fichier donné.
 * @param bc Blockchain à transformer
 * @param f Fichier dans lequel écrire
 */
void BCtoJSON(Blockchain *bc, FILE *f);

#endif /* BLOCKCHAIN_H_ */
