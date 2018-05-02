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

//Variables externes utilisées par getopt()
extern int optopt;
extern char *optarg;

void correctUse(char *exec) {
	printf("Utilisation: ./%s -b nbBlocks -d difficulté  [-o outJSONFile] [-c]\n", exec);
	printf("OU:          ./%s -i inJSONFile  [-o outJSONFile] [-c]\n", exec);
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
					printf("Erreur: Le nombre de blocks doit être strictement positif.\n");
					correctUse(argv[0]);
				}
				exit(3);
				break;
			case 'd':
				difficulty = (int) atoi(optarg);
				if (difficulty <= 0) {
					printf("Erreur: La difficulté doit être strictement positive.\n");
					correctUse(argv[0]);
				}
				exit(3);
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
					printf("Erreur: L'option -%c nécessite un argument.\n", optopt);
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

	//Vérification de la cohérence des paramètres
	if ((nbBlocks > 0 || difficulty > 0) && infile) {
		printf("Erreur: Syntaxe incorrecte.\n");
		correctUse(argv[0]);
		exit(3);
	}
	if ((!nbBlocks || !difficulty) && !infile) {
		printf("Erreur: Il manque des paramètres.\n");
		correctUse(argv[0]);
		exit(3);
	}

	/* Première étape: Construction et affichage de la blockchain */

	if (nbBlocks > 0) { //Cas génération de blockchain
		bc = blockchain(difficulty);
		addGenesis(bc);

		for (int i = 1; i < nbBlocks; i++) {
			//addRandomBlock(bc)...
		}

	} else { //Cas lecture de fichier JSON
		printf("La lecture de fichiers JSON n'est pas encore supportée.\n");
		exit(0);
	}

	afficherBlockchain(bc);

	/* Deuxième étape: Écriture dans un fichier JSON le cas échéant */

	if (outfile) {
		printf("La création de fichiers JSON n'est pas encore supportée.\n");
		exit(0);
	}

	/* Troisième étape: Cheat le cas échéant, et mise à jour du fichier JSON le cas échéant */

	if (cheat) {
		printf("Le cheat de block n'est pas encore supporté.\n");
	}

	return 0;
}
