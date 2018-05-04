#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>

#include "patoh.h"
#include "gestionnaire.h"

#define	ERR		-1
#define NBPARAM	 4

#define usRandom(s)     srand(s)
#define uRandom(l)		(rand() % l)
#define fatalerror(m)	puts(errormsg[m]), exit(m)

const char * errormsg[] = {
	"No error", //0
	"\nLe fichier n'existe pas.\n", //1
	"\nVous n'avez pas indiqué un bon type de fichier.\n", //2
	" n'a pas marche.\n", // 3
	"Il n'y a pas le bon nombre de paramètre.\nAttendu : <nom_fichier> <#partitions> <locacite> \n"
};

int main(int argc, char *argv[])
{
	if(argc != NBPARAM)
		fatalerror(4);
	/// argv[1] = filename, argv[2] = #partition, argv[3] = outputdetail
	PaToH_Parameters pargs;
	char * filename = argv[1];
	struct stat sb;
	if(stat(filename, &sb) == ERR)
		fatalerror(1);
	else if((sb.st_mode & S_IFMT) != S_IFREG)
		fatalerror(2);
	int bonFonctionnement;
	int _c; 			// Nombre de cellules / sommets de l'hypergraphe
	int _n; 			// Nombre de parties (nets) de l'hypergraphe
	int _nconst; 		// Nombre de contraintes
	int * cwghts; 		// Tableau de taille _c * _nconst qui contiendra les poids de chaque sommet
	int * nwghts;		// Tableau de taille _n qui contiendra le coût de chaque partie ou NULL si aucun
	int * xpins;		// Tableau de taille _n + 1 qui contiendra l'index de début des sommets connectées aux parties
	int * pins;			// Tableau qui contiendra les listes des épingles des parties. 
	int * partvec;		// Tableau de taille _c qui contiendra le numéro de la partition à laquelle appartient chaque sommet 
	int cut;			// Cutsize de la solution d'après la métrique requise par cuttype
	int * partweights;	// Taille de taille _k * _ncosnt qui retourne le poids total de la partition pour chaque partition
	float * targetweights;	// Tableau de taille _k * _nconst qui contiendra les objectifs concernant les poids des partitions
	float * skewedtweights;	// ??
	
	usRandom(time(NULL));
	
	printf("===============================================================================\n");
	printf("Lecture de l'hypergraphe en cours...\n");
	bonFonctionnement = PaToH_Read_Hypergraph(filename, &_c, &_n, &_nconst, &cwghts, &nwghts, &xpins, &pins);
	if(bonFonctionnement != 0)
		puts("\n PaToH_Read_Hypergraph"), fatalerror(3);
	printf("Hypergraphe %10s \n\t #Sommets (cells) = %d \n\t #Parties (nets) = %d \n\t #Epingles (pins) = %d \n\t #Const = %d\n",
			filename, _c, _n, xpins[_n], _nconst);
	
	PaToH_Initialize_Parameters(&pargs, PATOH_CONPART, PATOH_SUGPARAM_DEFAULT);
	pargs._k = atoi(argv[2]); // Nombre de partitions voulues.
	pargs.outputdetail = atoi(argv[3]);
	gestionnaire parDefaut(pargs, _nconst);
	
	printf("\n===============================================================================\n");
	partvec = (int*) malloc(_c*sizeof(int));
	partweights = (int*) malloc(_nconst * pargs._k * sizeof(int));
	targetweights = (float*) malloc(_nconst * pargs._k * sizeof(float));
	skewedtweights = (float*) malloc(_nconst * pargs._k * sizeof(float));
	
	for(int i = 0 ; i < pargs._k ; i++) 
	{
		targetweights[i] = 1.0 / (float) pargs._k;
		skewedtweights[i] = pargs._k - i;
	}
	
	PaToH_Alloc(&pargs, _c, _n, _nconst, cwghts, nwghts, xpins, pins);
	
	parDefaut.affichage_param();
	
	printf("\n");
	/// Libération mémoire
	
		free(partvec);
		free(partweights);
		free(targetweights);
		free(skewedtweights);
		free(cwghts);
		free(nwghts);
		free(pins);
		free(xpins);
		PaToH_Free(); // Libère les tableaux allouée par Path_Alloc : cwghts, nwghts, xpins, pins.
	
	return 1;
}

