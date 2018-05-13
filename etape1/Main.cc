#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include <zlib.h>
#include <string.h>

#include "PARSER/ParseUtils.h"
#include "PARSER/Dimacs.h"
#include "PARSER/Parser.h"
#include "PATOH/Partitionnement.h"

using namespace TER;
// export MROOT="/home/glavio/TER/etape1"
//=================================================================================================

static Parser* parser;

// Main:

int main(int argc, char** argv)
{
	
	printf("RAPPEL : <filename> <param> <#parts>*\n\t");
	printf("P : pour avoir les poids des parties (nets))\n");
	printf("\t S : pour avoir les poids des sommets (cells))\n");
	printf("\t B : pour avoir la matrice d'adjacence en binaire\n");
	printf("\t D : pour avoir la matrice d'adjacence en décimal\n");
	if(argc < 2)
		printf("ERREUR !\n"), exit(1);

	int weights = 0;
	int nb_partitions = 0;
	char base = 'Z';
	char* in = NULL;
	char* out = NULL;
	
	gzFile cnf = NULL;
	FILE* dimacs = NULL;
	FILE* primal = NULL;
	FILE* dual = NULL;
	FILE* matrice = NULL;

	
	if(argc > 2)
	{
		for(int i = 2, j = argc ; i < j ; i++)
		{
			switch(toupper(*argv[i]))
			{
				case 'P' : weights+= 1; printf("\n Poids : poids des parties"); break;
				case 'S' : weights+= 2; printf("\n Poids : poids des sommets"); break;
				case 'B' : 
				{
					if(base == 'D') 
						printf("Vous avez déjà mis D\n") ;
					else
						base = 'B', printf("\n Matrice d'adjacence en binaire"); 
				} break;
				case 'D' : 
				{
					if(base == 'B') 
						printf("Vous avez déjà mis B\n") ;
					else
						base = 'D', printf("\n Matrice d'adjacence en décimal"); 
				} break;
				default : nb_partitions = atoi(argv[i]); printf("\n %d partitions\n", nb_partitions);
			}
		}
	}
	if(base == 'Z') 
		base = 'B';
	printf("\n");
	Partitionnement partitions(nb_partitions, argv[1]);
	Parser P(base, weights);
	parser = &P;
	in = (char*) malloc(sizeof(char) * (10 + (int)strlen(argv[1]))); 
	out = (char*) malloc(sizeof(char) * (15 + (int)strlen(argv[1])));
	strcpy(in, "IN/");
	strcat(in, argv[1]);
	
	cnf = (argc == 1) ? gzdopen(0, "rb") : gzopen(in, "rb");
	if (cnf == NULL)
		printf("ERROR! Could not open file: %s\n", argc == 1 ? "<stdin>" : argv[1]), exit(1);
	
	printf("Opening %s file... \n\n", in);
	parse_DIMACS(cnf, P); 
	gzclose(cnf);
	
	strcpy(out, "OUT/dimacs_");
	strcat(out, argv[1]);
	dimacs = fopen(out, "wr");
	if(dimacs == NULL)
		printf("ERROR! Could not open file: %s\n", out), exit(1);
	printf("Brbrbr... DIMACS generating...\n");
	P.toDimacs(dimacs);
	fclose(dimacs);
	printf("Go check %s !\n\n", out);

	
	strcpy(out, "OUT/primal_");
	strcat(out, argv[1]);
	primal = fopen(out, "wr");
	if(primal == NULL) 
		printf("ERROR! Could not open file: %s\n", out), exit(1);
	printf("Brbrbr... PRIMAL generating...");
	switch(weights)
	{
		case 0 : printf("Without weights.\n"); break;
		case 1 : printf("With net weights.\n"); break;
		case 2 : printf("With cell weights.\n"); break;
		case 3 : printf("With both net and cell weights.\n"); break;
	}
	P.toPatohPrimal(primal);
	printf("Go check %s !\n\n", out);
	fclose(primal);
	partitions.toPartitions(out);
	partitions.calculCutset(0.75, 0.25);
	
	//// SI besoin
	strcpy(out, "OUT/matrice_");
	strcat(out, argv[1]);
	matrice = fopen(out, "wr");
	printf("Brbrbr... MATRICE generating... Parametre : %c\n", base);
	P.toMatrice(matrice);
	fclose(matrice);
	printf("Go check %s !\n\n", out);
	free(in);
	free(out);
	
	
	
	return 1;
	
}
