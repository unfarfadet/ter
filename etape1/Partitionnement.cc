#include "PATOH/Patoh.h"
#include "PATOH/Partitionnement.h"

#include <time.h>

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <vector>
#include <map>
#include <time.h>
#include <cmath>

using namespace TER;
using namespace std;

Partitionnement::Partitionnement(int _nb, char * _name):
	nb_Parts(_nb),
	name(_name)
	{
		char chemin[] = "OUT/clausesPartagees_";
		file_partage = (char*) malloc(sizeof(char) * (strlen(chemin) + strlen(_name)));
		strcpy(file_partage, chemin);
		strcat(file_partage, _name);
	}

Partitionnement::~Partitionnement()
{
	free(file_partage);
}

void Partitionnement::toPartitions(char * filename)
{
	/// pour l'instant on s'occupe que de deux
	PaToH_Parameters args;
	int nbSommets = 0;
	int _n = 0;
	int _nconst = 0;
	int *cwghts = NULL;
	int *nwghts = NULL;
	int *xpins = NULL; 
	int *pins = NULL;
	int *partvec = NULL;
	int cut = 0;
	int *partweights = NULL;
	int nbPins = 0;
	int nbEnsembles = 0;
	int ** ensembles;
	int inc[nb_Parts] = {0,0};
	PaToH_Read_Hypergraph(filename, &nbSommets, &_n, &_nconst, &cwghts, 
						  &nwghts, &xpins, &pins);
	
	//////////////////// Tentative pour cutset
	char c;
	int nb_conflits = 0;
	int ** conflits;
	int sommet = 0;
	int partition_avant = -1;
	bool meme_partition = true;
	int nb_sommets_lus = 1;
	vector<int> sommets_temporaires;
	FILE * listeClausesPartagees = NULL;
	//char chemin[] = "OUT/clausesPartagees_";
	//char * nomFichier = (char*) malloc(sizeof(char) * (strlen(chemin) + strlen(Partitionnement::name)));

	/////////////////// Fin tentative pour cutset
	
	/// PaToH travaille : 1) lit le fichier 2) initialise les paramètres 3) alloue 4) partitionne
	printf("Hypergraph %10s -- #Cells=%6d  #Nets=%6d  #Pins=%8d #Const=%2d\n",
		   filename, nbSommets, _n, xpins[_n], _nconst);
	
	PaToH_Initialize_Parameters(&args, PATOH_CONPART, 
								PATOH_SUGPARAM_DEFAULT);
	args._k = nb_Parts;
	partvec = (int *) malloc(nbSommets*sizeof(int));
	partweights = (int *) malloc(nb_Parts*_nconst*sizeof(int));
	PaToH_Alloc(&args, nbSommets, _n, _nconst, cwghts, nwghts, 
				xpins, pins);

	PaToH_Part(&args, nbSommets, _n, _nconst, 0, cwghts, nwghts, xpins, pins, NULL, partvec, partweights, &cut);

	/// A nous de jouer maintenant
	
	nbEnsembles = nb_Parts * _nconst;
	ensembles = (int **) malloc(nbEnsembles*sizeof(int*));
	
	for(int i = 0 ; i < nbEnsembles; i++)
		ensembles[i] = (int *) malloc(partweights[i] * sizeof(int));
		
	for(int i = 0 ; i < nbSommets ; i++)
	{
		ensembles[partvec[i]][inc[partvec[i]]] = i+1;
		inc[partvec[i]]++;
	}
	
	// ----
	conflits = (int**) malloc(cut * sizeof(int*));
	ifstream entree (filename, ios::in|ios::binary);
	streambuf * backup = cin.rdbuf();
	 
	if(!entree) { printf("non \n"); }
	else 
	{
		do { entree.get(c);} while(c != '\n');
		
	//	entree.get(c); // on se place au début
		cin.rdbuf(entree.rdbuf());
		cin >> sommet;
		partition_avant = partvec[sommet-1];
		sommets_temporaires.push_back(sommet);
	//	printf("\t premiere partition = %d\n", partition_avant);
		do
		{
			while(cin.read(&c,1) && (c < '0' || c > '9'))
			{
				if(c == '\n')
				{ 
					if(!meme_partition)
					{ 
						conflits[nb_conflits] = (int*) malloc(sizeof(int) * (nb_sommets_lus));
						for(int i = 0 ; i < nb_sommets_lus; i++)
							conflits[nb_conflits][i] = sommets_temporaires[i];
						//printf("Nombre conflit n°%d avec %d sommets\n", nb_conflits, nb_sommets_lus);
						//for(int i = 0 ; i < nb_sommets_lus; i++)
						//	printf(" %d ", conflits[nb_conflits][i]);
						nb_conflits++;
						meme_partition = true;
					}
					sommets_temporaires.clear();
				//	printf("\n");
					nb_sommets_lus = 0;
				} 
			}
			if((c >= '0') && (c <= '9'))
			{
				cin.putback(c);
				cin >> sommet ;
				if(nb_sommets_lus == 0)
					partition_avant = partvec[sommet-1];
				else if(partvec[sommet-1] != partition_avant)
					meme_partition = false;
			//	printf("\tsommet %d a partition %d\n", sommet, partvec[sommet-1]);
				sommets_temporaires.push_back(sommet);
				nb_sommets_lus++;
			}
		} while(!cin.eof());
	
		entree.close();
	}
	// ----
	cin.rdbuf(backup);
	
	/// Ecriture fichier
	
	listeClausesPartagees = fopen(Partitionnement::file_partage, "wr");
	fprintf(listeClausesPartagees, "%d", nb_conflits);
	for(int i = 0; i < nbEnsembles; i++)
		fprintf(listeClausesPartagees, " %d", partweights[i]);
	fprintf(listeClausesPartagees, "\n");
	for(int i = 0 ; i < nbEnsembles ; i++, fprintf(listeClausesPartagees, "0 \n"))
		for(int j = 0 ; j < partweights[i] ; j++)
			fprintf(listeClausesPartagees,"%d ", ensembles[i][j]);

	for(int i = 0 ; i < nb_conflits ; i++, fprintf(listeClausesPartagees, "0 \n"))
	{
		for(int j = 0, taille_temp = sizeof(conflits[i]) / sizeof(int); j <= taille_temp ; j++)
			fprintf(listeClausesPartagees, "%d ", conflits[i][j]);
	}
	fclose(listeClausesPartagees);
	
	free(cwghts);      free(nwghts);
	free(xpins);       free(pins);
	free(partweights); 
	
	for(int i = 0 ; i < nbEnsembles ; i++)
		free(ensembles[i]);
	free(ensembles);
	free(partvec);
	for(int i = 0 ; i < nb_conflits ; i++)
		free(conflits[i]);
	free(conflits);
	PaToH_Free();
}

int Partitionnement::findClause(vector<int> liste, int num)
{
	bool trouve = false;
	int inc, retour, taille_temp;
	inc = 4;
	retour = -1 ;
	taille_temp = liste.size();
	while(!trouve && inc < taille_temp)
	{
		if(liste[inc] == num)
			trouve = true, retour = inc;
		else
			inc++;
	}
	return retour;
}

void Partitionnement::calculCutset(double P1, double P2)
{
	ifstream entree (Partitionnement::file_partage, ios::in|ios::binary);
	if(!entree)
		perror("Fichier inexistant");
	
	int cutsize, taille1, taille2;
	cin.rdbuf(entree.rdbuf());
	cin >> cutsize >> taille1 >> taille2;
	
	const int NB_SOMMETS = taille1 + taille2;
	const int DEBUT_CLAUSE = 4;
	
	int nb_sommets_restants, int_lu, temp, num_conflit, nb_litt_une, nb_litt_deux, elu, derniere_partition, partition_inverse, taille_temp;
	double pourcentage_max, pourcentage_temp;
	
	vector<int> partition_une;
	vector<int> partition_deux;
	vector<int> cutset;
	vector<vector<int>> matrice;
	vector<int> litt_temp;
	map<int, int> correspondance;
	vector<int> litt_a_supprimer;

	int occurence[cutsize][nb_Parts];
	double pourcentage[NB_SOMMETS][3];
	
	srand(time(NULL));
	int_lu  = temp = num_conflit = nb_litt_une = nb_litt_deux = elu = taille_temp =  0;	
	derniere_partition = partition_inverse = -1;
	pourcentage_max =  pourcentage_temp = 0.0;

	for(int i = 0; i < NB_SOMMETS ; i++)
		pourcentage[i][0] = pourcentage[i][1] = pourcentage[i][2] = 0.0;

	printf("Brbrbr... Calcul du cutset...\n....Cutsize = %d \n", cutsize);
	/// Récupération des données
	while(cin >> int_lu && int_lu != 0) { partition_une.push_back(int_lu); }
	while(cin >> int_lu && int_lu != 0) { partition_deux.push_back(int_lu); }
	printf("....Première partition ( %d ) = ", taille1);
	for(int i = 0 ; i < taille1 ;i++)
		printf("%d ", partition_une[i]);
	printf("\n....Deuxieme partition ( %d ) = ", taille2);
	for(int j = 0; j < taille2 ;j++)
		printf("%d ", partition_deux[j]);
	printf("\n");
	
	for(int i = 0 ; i < taille1 ; i++, temp++)
	{
		correspondance[partition_une[i]] = temp;
		matrice.push_back(vector<int>(4,0));
		matrice[temp][1]= partition_une[i];
		matrice[temp][0] = 0;
	}
	for(int i = 0 ; i < taille2 ; i++, temp++)
	{
		correspondance[partition_deux[i]] = temp;
		matrice.push_back(vector<int>(4,0));
		matrice[temp][1] = partition_deux[i];
		matrice[temp][0] = 1;
	}
	/// map donne pour un sommet donné (son nom) son indice dans la matrice
	while(!cin.eof())
	{
		cin >> int_lu;
		if(int_lu == 0)
		{
			occurence[num_conflit][0] = nb_litt_une;
			occurence[num_conflit][1] = nb_litt_deux;
			
			temp = (nb_litt_une > nb_litt_deux) ? 1 : 0;
			if(occurence[num_conflit][temp] == 1)
			{
				for(int i = 0, taille_temp = litt_temp.size() ; i < taille_temp; i++)
				{
					if(matrice[correspondance[litt_temp[i]]][0] == temp)
						matrice[correspondance[litt_temp[i]]][3] += 1;
				}
			}
			num_conflit++;
			nb_litt_une = nb_litt_deux = 0;
			litt_temp.clear();
			
		}
		else
		{
			temp = correspondance[int_lu];
			matrice[temp][2] += 1;
			if(matrice[temp][0] == 0)
				nb_litt_une++;
			else
				nb_litt_deux++;
			litt_temp.push_back(int_lu);
			matrice[temp].push_back(num_conflit+1);
		}
	}
	/*
	for(int i = 1 ; i <= 20 ; i++)
		printf("%d %d \n", i, correspondance[i]);
	
	printf("\n\n");
	*/
	for(int i = 0 ; i < taille1 + taille2 ;i++, printf("\n"))
		for(int j = 0 ; j < (signed) matrice[i].size(); j++)
			printf("%d ", matrice[i][j]);
	printf("\n\n"); 
	
	nb_sommets_restants = matrice.size();
	int nombre_passage = 0;
	while(nb_sommets_restants > 3)
	{
		
		nombre_passage++;
		pourcentage_max = 0.0;
		printf("\n\nPassage n°%d\n", nombre_passage);
		printf("Taille de la map = %d\n", correspondance.size());
		printf(".... Calcul pourcentage ....\n");
		printf("Derniere_partition %d\n", derniere_partition);
		for(int i = 0, j = 0, taille = matrice.size() ; i < taille ; i++)
		{	
			if(derniere_partition != matrice[i][0])
			{
				pourcentage[i][0] = (matrice[i][2] * 100) / cutsize;
				pourcentage[i][1] = (matrice[i][3] * 100) / cutsize;
				pourcentage[i][2] = P1 * pourcentage[i][0] + P2 * pourcentage[i][1];
				pourcentage_temp = pourcentage[i][2];
				//printf("Sommet %d a une partition differente %d, pourcentage %.2f\n", matrice[i][1], matrice[i][0], pourcentage_temp);
				if(pourcentage_temp == 0)
					litt_a_supprimer.push_back(matrice[i][1]);
					
				else if(pourcentage_max < pourcentage_temp && derniere_partition != matrice[i][0])
				{
					//printf("Nouveau pourcentage max\n");
					//printf("le sommet %d, ayant pour indice %d, a comme pourcentage total =  %.2f\n", matrice[i][1], i, pourcentage_temp);
					pourcentage_max = pourcentage_temp;
					elu = matrice[i][1]; 
				}
				else if(pourcentage_max == pourcentage_temp && derniere_partition != matrice[i][0])
				{
					j = rand() % 10 + 1;
					if(j > 5)
						elu = matrice[i][1];
				}				
			}
		}
		derniere_partition = matrice[correspondance[elu]][0];
		partition_inverse = abs(derniere_partition - 1);
		
		for(int i = 0, j = 0, t = litt_a_supprimer.size() ; i < t; i++)
		{
			printf("... Suppression sommet %d ...\n", litt_a_supprimer[i]);
			temp = litt_a_supprimer[i];
			j = correspondance[temp];
			matrice.erase(matrice.begin() + j);
			correspondance.erase(temp);
			
			for(int h = 0, matrice_size = matrice.size(); h < matrice_size ; h++)
				if(correspondance[matrice[h][1]] > j)
					correspondance[matrice[h][1]] -= 1;
			nb_sommets_restants--;
		}
		litt_a_supprimer.clear();
		
		/// MAJ MATRICE
		
		if(nb_sommets_restants > 3)
		{
			printf("Elu des dieux : %d, de la partition %d, ayant comme pourcentage %.2f \n", elu, derniere_partition, pourcentage_max);
		
			///// On parcoure la ligne correspondant à l'élu, cad toutes les clauses auxquels il appartient
			for(int i =  DEBUT_CLAUSE, retour = -1, indice = correspondance[elu], conflit = 0, t = matrice[indice].size(); i < t; i++)
			{
				conflit = matrice[indice][i]; /// Num clause
				printf("Conflit selection %d...", conflit);
				/// Si après suppression de l'élu il n'y a plus d'autres littéraux de même partition dans clause partagee
				if( (occurence[conflit-1][derniere_partition] -= 1) == 0)
				{
					/// On supprime alors la clause complete
					printf("Suppression clause complete\n");
					for(int j = 0, t = matrice.size() ; j < t ; j++)
					{
						if(j != indice)
						{
							retour = Partitionnement::findClause(matrice[j], conflit);
							if(retour != -1)
							{
								matrice[j].erase(matrice[j].begin() + retour);
								matrice[j][2] -= 1;
								if(matrice[j][2] == 0)
									litt_a_supprimer.push_back(matrice[j][1]);
								if(occurence[conflit-1][partition_inverse] == 1)
									matrice[j][3] -= 1;
								occurence[conflit-1][partition_inverse] = 0;
							}
						}
					}
				}
				else
				{
					printf("Maj littéraux appartenant à clause\n");
					/// MAJ des littéraux qui étaient inclus dans clause
					for(int j = 0, t = matrice.size(); j < t ; j++)
					{
						if(j != indice)
						{
							retour = Partitionnement::findClause(matrice[j], conflit);
							if(retour != -1)
							{
								if(occurence[conflit-1][partition_inverse] == 1 && matrice[j][0] == derniere_partition)
									matrice[j][3] += 1;
							} 
						}
					}
				}
			}
			/// SUPPRESSION DE L'ELU DANS LA MATRICE ET  CORRESPONDANCE
			temp = correspondance[elu];
			//printf("...Suppression de l'élu %d.... avec pour indice %d....\n", elu, correspondance[elu]);
			//printf(" Vérification suppression : %d, taille correspondace %d\n", matrice[temp][1], correspondance.size());
			matrice.erase(matrice.begin() + temp);
			correspondance.erase(elu);
			//printf(" Vérification suppression : taille matrice %d taille correspondace %d\n", matrice.size(), correspondance.size());
			for(int h = 0, matrice_size = matrice.size() ; h < matrice_size ; h++)
				if(correspondance[matrice[h][1]] > temp)
					correspondance[matrice[h][1]] -= 1;
			
			for(int i = 0, j = 0, t = litt_a_supprimer.size() ; i < t; i++)
			{
				printf("... Suppression sommet %d ...\n", litt_a_supprimer[i]);
				temp = litt_a_supprimer[i];
				j = correspondance[temp];
				matrice.erase(matrice.begin() + j);
				correspondance.erase(temp);
				
				for(int h = 0, matrice_size = matrice.size(); h < matrice_size ; h++)
					if(correspondance[matrice[h][1]] > j)
						correspondance[matrice[h][1]] -= 1;
				nb_sommets_restants--;
			}
			litt_a_supprimer.clear();
		
			printf(" Vérification suppression : taille matrice %d taille correspondace %d\n", matrice.size(), correspondance.size());
			for(int i = 0 ; i < matrice.size(); i++, printf("\n"))
			{
				printf("correspondance sommet %d indice %d, ", matrice[i][1], correspondance[matrice[i][1]]);
				for(int j = 0 ; j < matrice[i].size();j++)
					printf("%d ", matrice[i][j]);
			}
		
			/// ARRIVEE DE L'ELU DANS CUTSET, MAJ DES PARTITIONS
			
			cutset.push_back(elu);
			temp = 0;
			if(derniere_partition == 0)
			{
				while(partition_une[temp] != elu) temp++;
				partition_une.erase(partition_une.begin() + temp);
				taille1--;
			}
			else
			{
				while(partition_deux[temp] != elu) temp++;
				partition_deux.erase(partition_deux.begin() + temp);
				taille2--;
			}
		}
		nb_sommets_restants = matrice.size();
	}
	/// AFFICHAGE 
	printf("\nDéliberation jury sur les derniers sommets...\n");
	for(int i = 0 ; i < matrice.size(); i++, printf("\n"))
	{
		printf("correspondance sommet %d indice %d, ", matrice[i][1], correspondance[matrice[i][1]]);
		for(int j = 0 ; j < matrice[i].size();j++)
			printf("%d ", matrice[i][j]);
	}
	switch(nb_sommets_restants)
	{
		case 3 :
		{
			elu = matrice[0][1]; 
			derniere_partition = matrice[0][0];
			if(matrice[1][0] == matrice[0][0])
				elu = matrice[2][1], derniere_partition = matrice[2][0];
			else if (matrice[1][0] != matrice[2][0])
				elu = matrice[1][1], derniere_partition = matrice[1][0];
		} break;
		case 2 :
		{
			derniere_partition = (taille1>taille2) ? 0 : 1;
			if(matrice[0][0] == derniere_partition)
				elu = matrice[0][1];
			else
				elu = matrice[1][1];
		} break;
	}
	
	cutset.push_back(elu);
	temp = 0;
	if(derniere_partition == 0)
	{
		while(partition_une[temp] != elu) temp++;
		partition_une.erase(partition_une.begin() + temp);
		taille1--;
	}
	else
	{
		while(partition_deux[temp] != elu) temp++;
		partition_deux.erase(partition_deux.begin() + temp);
		taille2--;
	}
	
	printf("....Première partition ( %d ) = ", taille1);
	for(int i = 0, v = partition_une.size() ; i < v ;i++)
		printf("%d ", partition_une[i]);
	printf("\n....Deuxieme partition ( %d ) = ", taille2);
	for(int j = 0, w = partition_deux.size(); j < w ;j++)
		printf("%d ", partition_deux[j]);
	printf("\n.... Cutset ( %d ) = ", cutset.size());
	for(int h = 0, x = cutset.size(); h < x; h++)
		printf("%d ", cutset[h]);
	printf("\n");
	/// FREE
	litt_a_supprimer.clear();
	correspondance.clear();
	litt_temp.clear();
	
	for(int i = 0, j = matrice[i].size() ; i < j; i++)
		matrice[i].clear();
	matrice.clear();
	
	cutset.clear();
	partition_deux.clear();
	partition_une.clear();
	entree.close();
}
