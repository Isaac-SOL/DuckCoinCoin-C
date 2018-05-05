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
#include "json/json.h"
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
		FILE *f = fopen(infile, "r");
		if (f == NULL) {
			printf("Erreur dans l'ouverture du fichier %s.\n", infile);
			perror(NULL);
			exit(2);
		}
		fseek(f, 0, SEEK_END);
		int size = ftell(f);
		char *infileContent = malloc (size * sizeof(char));
		fseek(f, 0, SEEK_SET);
		printf("Lecture...\n");
		fread(infileContent, sizeof(char), size, f);
		printf("Parsing...\n");
		bc = BCfromJSON(json_parse(infileContent, size));
		fclose(f);
		free(infileContent);
	}

	afficherBlockchain(bc);

	/* Deuxième étape: Écriture dans un fichier JSON le cas échéant */

	if (outfile && !infile) {
		FILE *f = fopen(outfile, "w");
		if (f == NULL) {
			printf("Erreur dans l'ouverture du fichier %s.\n", outfile);
			perror(NULL);
			exit(2);
		}
		BCtoJSON(bc, f);
		fclose(f);
	}

	/* Troisième étape: Cheat le cas échéant, et mise à jour du fichier JSON le cas échéant */

	if (cheat) {
		printf("\n\n+----------------+\n|CONSOLE DE CHEAT|\n+----------------+\n");
		int mode = 0, opt1, opt2;

		while (mode != 5) {
			printf("\n\nIndiquez le mode d'action, puis les options si necessaires:\n");
			printf("Mode 0: Affichage de la Blockchain\n");
			printf("Mode 1: Suppression d'un block.  Option 1: Index du block a supprimer\n");
			printf("Mode 2: Suppression d'une transaction.  Option 1: Index du block a modifier,  Option 2: Index de la transaction a supprimer\n");
			printf("Mode 3: Verification de l'integrite de la Blockchain\n");
			if (outfile) printf("Mode 4: Enregistrer dans le fichier JSON\n");
			printf("Mode 5: Quitter le programme\n");
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
					//maj JSON
					break;

				case 2: //Suppression Transaction
					scanf("%d", &opt1);
					scanf("%d", &opt2);
					fflush(stdin);
					cheatTransaction(bc, opt1, opt2);
					//maj JSON
					break;

				case 3: //Vérification de la Blockchain
					messageValidite(verifBlockchain(bc));
					printf(verifMerkleRoot(bc) ? "Erreur dans une Merkle Root!\n" : "Pas d'erreur dans les Merkle Roots.\n");
					break;

				case 4: //Écrire dans le fichier de sortie
					if (outfile) {
						printf("Ecriture dans le fichier %s.", outfile);
						FILE *f = fopen(outfile, "w");
						if (f == NULL) {
							printf("Erreur dans l'ouverture du fichier %s.\n", outfile);
							perror(NULL);
							exit(2);
						}
						BCtoJSON(bc, f);
						fclose(f);
					} else {
						printf("Aucun fichier de sortie n'a ete donne en argument.\nUtilisez l'option -i pour specifier un fichier de sortie.\n");
					}
					break;

				case 5: //Quitter le programme
					printf("Fermeture de la console de cheat.\n");
					break;

				default:
					printf("Erreur: Mode inconnu.\n");
			}
		}
	}
	freeBlockchain(bc);

	return 0;
}
