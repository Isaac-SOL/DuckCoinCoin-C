/*
 * main.c
 *
 *  Created on: 17 févr. 2018
 *      Author: Pierre
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include "sha256/sha256.h"
#include "blockchain.h"
#include "transaction.h"
#include "randomGen.h"

//Variables externes utilisées par getopt()
extern int optopt;
extern char *optarg;

/**
 * Affiche les instructions brèves d'utilisation.
 * -b : Mode génération de Blockchain, indique le nombre de blocks à générer
 * -d : Mode génération de blockchain, indique la difficulté de la blockchain générée
 * -i : Mode lecture de fichier, indique le fichier JSON d'entrée
 * -c : Indique si l'utilisateur peut cheater après que la blockchain soit construite
 * -o : Indique le fichier JSON dans lequel sera inscrite la blockchain.
 * @param exec Nom de l'exécutable
 */
void correctUse(char *exec) {
	printf("Utilisation: ./%s -b nbBlocks -d difficulté  [-c] [-o outJSONFile]\n", exec);
	printf("OU:          ./%s -i inJSONFile  [-c [-o outJSONFile]]\n", exec);
}

int main(int argc, char *argv[]) {
	char *infile = NULL, *outfile = NULL;
	int option, nbBlocks = 0, cheat = 0, difficulty = 0;
	Blockchain *bc;

	/* Étape zéro:  récupération et vérification des paramètres */

	//On récupère les options une par une, on s'arrête quand il n'y en a plus
	while ((option = getopt(argc, argv, "b:d:i:o:c")) != -1) {
		switch (option) {
			case 'b':
				nbBlocks = (int) atoi(optarg);
				if (nbBlocks <= 0) {
					printf("Erreur: Le nombre de blocks doit etre strictement positif.\n");
					correctUse(argv[0]);
					exit(3);
				}
				break;
			case 'd':
				difficulty = (int) atoi(optarg);
				if (difficulty <= 0) {
					printf("Erreur: La difficulte doit etre strictement positive.\n");
					correctUse(argv[0]);
					exit(3);
				}
				break;
			case 'i':
				infile = optarg;
				break;
			case 'o':
				outfile = optarg;
				break;
			case 'c':
				cheat = 1;
				break;
			case '?':
				if (optopt == 'i' || optopt == 'o' || optopt == 'k')
					printf("Erreur: L'option -%c necessite un argument.\n", optopt);
				else
					printf("Erreur: Option -%c inconnue.\n", optopt);
				correctUse(argv[0]);
				exit(3);
				break;
			default:
				printf("Erreur inconnue dans la lecture de l'option.\n");
				exit(3);
		}
	}

	//Temporaire, pour test
	printf("Status:\n-b : %d\n-d : %d\n-i : %s\n-c : %s\n-o : %s\n", nbBlocks, difficulty, infile ? infile : "false", cheat ? "true" : "false", outfile ? outfile : "false");

	//Vérification de la cohérence des paramètres
	if ((nbBlocks > 0 || difficulty > 0) && infile) {
		printf("Erreur: Syntaxe incorrecte.\n");
		correctUse(argv[0]);
		exit(3);
	}
	if ((!nbBlocks || !difficulty) && !infile) {
		printf("Erreur: Il manque des parametres.\n");
		correctUse(argv[0]);
		exit(3);
	}

	/* Première étape: Construction et affichage de la blockchain */

	if (nbBlocks > 0) { //Cas génération de blockchain
		bc = random_blockchain(difficulty, nbBlocks); //Ajouter les arguments difficulté et nombre de blocks!
	} else { //Cas lecture de fichier JSON
		printf("La lecture de fichiers JSON n'est pas encore supportee.\n");
		exit(0);
	}

	afficherBlockchain(bc);

	/* Deuxième étape: Écriture dans un fichier JSON le cas échéant */

	if (outfile && !infile) {
		printf("La création de fichiers JSON n'est pas encore supportee.\n");
		exit(0);
	}

	/* Troisième étape: Cheat le cas échéant, et mise à jour du fichier JSON le cas échéant */

	if (cheat) {
		printf("\n\n+----------------+\n|CONSOLE DE CHEAT|\n+----------------+\n");
		int mode = 0, opt1, opt2;

		while (mode != 3) {
			printf("\n\nIndiquez le mode d'action, puis les options si necessaires:\n");
			printf("Mode 0: Affichage de la Blockchain\n");
			printf("Mode 1: Suppression d'un block.  Option 1: Index du block a supprimer\n");
			printf("Mode 2: Suppression d'une transaction.  Option 1: Index du block a modifier,  Option 2: Index de la transaction a supprimer\n");
			printf("Mode 3: Quitter le programme\n");
			scanf("%d", &mode);
			switch (mode) {
				case 0: //Affichage de la Blockchain
					fflush(stdin);
					afficherBlockchain(bc);
					break;

				case 1: //Suppression Block
					scanf("%d", &opt1);
					fflush(stdin);
					cheatBlock(bc, opt1);
					messageValidite(verifBlockchain(bc));
					printf(verifMerkleRoot(bc) ? "Erreur dans une Merkle Root!\n" : "Pas d'erreur dans les Merkle Roots.\n");
					//maj JSON
					break;

				case 2: //Suppression Transaction
					scanf("%d", &opt1);
					scanf("%d", &opt2);
					fflush(stdin);
					cheatTransaction(bc, opt1, opt2);
					messageValidite(verifBlockchain(bc));
					printf(verifMerkleRoot(bc) ? "Erreur dans une Merkle Root!\n" : "Pas d'erreur dans les Merkle Roots.\n");
					//maj JSON
					break;

				case 3: //Quitter le programme
					printf("Fermeture de la console de cheat.\n");
					break;

				default:
					printf("Erreur: Mode inconnu.\n");
			}
		}
	}

	return 0;
}
