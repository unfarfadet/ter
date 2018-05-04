#include "gestionnaire.h"
#include "patoh.h"

#include <stdlib.h>
#include <stdio.h>

gestionnaire::gestionnaire(PaToH_Parameters _pargs, int _contraintes):
	pargs(_pargs),
	contraintes(_contraintes)
	{}

void gestionnaire::getVisitOrder(const int choix)
{
	switch(choix)
	{
		case PATOH_VO_CONT : printf("Continu et sequentiel."); break;
		case PATOH_VO_RAND : printf("Aleatoire."); break;
		case PATOH_VO_SCD : printf("Non decroissant en fonction des degres des sommets."); break;
		case PATOH_VO_SMAXNS : printf("Non-decreasing maxmimum net size order."); break;
		case PATOH_VO_SMINNS : printf("Non-decreasing minimum net size order."); break;
		case PATOH_VO_SMINNSSUM : printf("Non-decreasing minimum of net size sum sorted."); break;
		case PATOH_VO_SWEEP : printf("Avec sweep : croissant pour ID de sommets pair, decroissant sinon."); break;
	}
}

void gestionnaire::getAlgoGrossissement(const int choix)
{
	switch(choix)
	{
		/* -------- Matching -------------- */
		
		case PATOH_CRS_HCM : case PATOH_CRS_PHCM : case PATOH_CRS_MANDIS : case PATOH_CRS_AVGDIS : 
		case PATOH_CRS_CANBERA : case PATOH_CRS_ABS : case PATOH_CRS_GCM : case PATOH_CRS_SHCM : 
		printf("Randomized machine-based hierarchical clustering scheme : "); break;
		/* --------- Agglomeratives ---------*/
		case PATOH_CRS_HCC : case PATOH_CRS_HPC : case PATOH_CRS_ABSHCC : case PATOH_CRS_ABSHPC : 
		case PATOH_CRS_CONC : case PATOH_CRS_GCC : case PATOH_CRS_SHCC :
		printf("Randomized agglomerative clustering schemes : "); break;
		/* --------------- Net Base Agglomeratives -------------- */
		case PATOH_CRS_NC: case PATOH_CRS_MNC: 
		printf("Net-based clustering algorithms : "); break;
	}
	switch(choix)
	{
		case PATOH_CRS_HCM : printf("HCM = Heavy connectivity matching."); break;
		case PATOH_CRS_PHCM : printf("PHCM = Probabilist heavy connectivity matching."); break;
		case PATOH_CRS_MANDIS : printf("MANDIS = Manhattan distance."); break;
		case PATOH_CRS_AVGDIS : printf("AVEDIS = Average distance."); break;
		case PATOH_CRS_CANBERA : printf("CANBERRA = Canberra metric."); break;
		case PATOH_CRS_ABS : printf("ABS = Absorption Metric."); break;
		case PATOH_CRS_GCM : printf("GCM = Greedy Cut Matching."); break;
		case PATOH_CRS_SHCM : printf("SHCM = Scaled Heavy Connectivity Matching."); break;
		case PATOH_CRS_HCC : printf("HCC = Heavy Connectivity Clustering."); break;
		case PATOH_CRS_HPC : printf("HPC = Heavy Pin Clustering."); break;
		case PATOH_CRS_ABSHCC : printf("ABSHCC = Absorption Clustering using Nets."); break;
		case PATOH_CRS_ABSHPC : printf("ABSHPC = Absorption Clustering using Pins."); break;
		case PATOH_CRS_CONC : printf("CONC = Connectivity Clustering."); break;
		case PATOH_CRS_GCC : printf("GCC = Greedy Cut Clustering."); break;
		case PATOH_CRS_SHCC : printf("SHCC = Scaled Heavy Connectivy Clustering."); break;
		case PATOH_CRS_NC : printf("NC = Net Clustering."); break;
		case PATOH_CRS_MNC : printf("MNC = Modified Net Clustering."); break;
	}
}

void gestionnaire::choixAlgoPartitionnementInitial(const int choix)
{
	switch(choix)
	{
		case PATOH_IPA_GHGP: printf("GHGP = Greedy Hypergraph Growing Partition."); break;
		case PATOH_IPA_AGGMATCH: printf("AGG_MATCH = Agllomerativ Match and Bin Packing."); break;
		case PATOH_IPA_BF: printf("BINPACK = Bin packing avec heuristique Best Fit Decreasing Bin Packing."); break;
		case PATOH_IPA_BINPACK: printf("BF = Breadth-First Partitionning."); break;
		case PATOH_IPA_RANDOM1: printf("RANDOM1 = sommets assignes aleatoirement."); break;
		case PATOH_IPA_RANDOM2: printf("RANDOM2 = sommets assignes a partition avec plus petit poids."); break;
		case PATOH_IPA_RANDOM3: printf("RANDOM3 = ordre de visite aleatoire coupe en deux."); break;
		case PATOH_IPA_GHG_MAXPIN: printf("GHG_MAXPIN = Greedy Hypergraph Growing with Max Pin."); break;
		case PATOH_IPA_GHG_MAXNET: printf("GHG_MAXNET = Greedy Hypergraph Growing with Max Net."); break;
		case PATOH_IPA_GHG_MAXPOSGAIN: printf("GHG_MAXPOSGAIN = Greedy Hypergraph Growing with Max only-Pos FM Gain."); break;
		case PATOH_IPA_COMP_GHGP: printf("COMP_GHG = Component bin-pack and Greedy Hypergraph Growing Partition."); break;
		case PATOH_IPA_GREEDY_COMP_GHGP: printf("GREEDY_COMP_GHGP = Greedy Component bin-pack and Greedy Hypergraph Growing Partition."); break;
		case PATOH_IPA_ALL: printf("ALL = utilise algo ci dessus pour chaque instance et / ou execution de cette phase."); break;
	}
}

void gestionnaire::choixAlgoAffinement(const int choix)
{
	switch(choix)
	{
		case PATOH_REFALG_NONE: printf("NONE = aucun affinement."); break;
		case PATOH_REFALG_T_BFM: printf("T_BFM = Boundary FM with tight balance."); break;
		case PATOH_REFALG_T_FM: printf("T_FM = FM with tight balance."); break;
		case PATOH_REFALG_D_BFM: printf("D_BFM = BFM with dynamic locking."); break;
		case PATOH_REFALG_D_FM: printf("D_FM = FM with dynamic locking."); break;
		case PATOH_REFALG_BKL: printf("BKL = Boundary Kerninghan-Lin."); break;
		case PATOH_REFALG_KL: printf("KL = Kerninghan-Lin."); break;
		case PATOH_REFALG_MLG_BFM: printf("MLG_BFM = BFM with Krishnamurthy's multilevel gain improvment."); break;
		case PATOH_REFALG_MLG_FM: printf("MLG_FM = FM with Krishnamurty's multilevel gain."); break;
		case PATOH_REFALG_BFMKL: printf("BFLKL = une passe BFM suivie par une passe BKL."); break;
		case PATOH_REFALG_FMKL: printf("FMKL = une passe FM suivie par une passe KL."); break;
	}
}

void gestionnaire::choixBalance(const int choix)
{
	switch(choix)
	{
		case PATOH_BALANCE_STRICT: printf(" sera forcee a etre strictement egale a (imbalance) * log(#partitions) a chaque bisection."); break;
		case PATOH_BALANCE_ADAPTIVE: printf(" sera ajustement dynamiquement a chaque recursion, avec pour objectif d'avoir (imbalance) a la fin."); break;
		case PATOH_BALANCE_RELAXED: printf(" est tranquille : chaque bisection aura le desequilibrage (imbalance) maximal.");
	}
}

void gestionnaire::affichage_param()
{
	printf("PARAMETRES UTILISES");
	
	printf("\n\n ==== Parametres generaux ==== ");
	printf("\n\t 1. Metrique utilisee : "), printf((pargs.cuttype == 1) ? "Connectivity" : "Cutnet");
	printf("\n\t 2. Nombre de partitions : %d", pargs._k);
	printf("\n\t 3. Niveau de locacite : %d", pargs.outputdetail);
	printf("\n\t 4. Graine pour le générateur de nombre aleatoire :");
		if(pargs.seed == -1) printf("Random"); 
		else printf("%d", pargs.seed);
	if(pargs.doinitperm == 0)
		printf("\n\t 5. Listes d'epingles / parties ne sont pas melangees avant le traitement.");
	else
		printf("\n\t 5. Listes d'epingles / parties sont melangees avant le traitement.");

	
	printf("\n\n ==== Net Discard Parameters ==== \n");
	printf("Les parties dont la taille est superieure aux valeurs suivantes sont jetees apres chaque etape de bisection :");
	printf("\n\t 6. %d", pargs.bisec_fixednetsizetrsh);
	printf("\n\t 7.  %.2f * %d (#Contraintes) ", pargs.bisec_netsizetrsh, contraintes);
	printf("\n\t 8. %d * %d (#Partitions) ", pargs.bisec_partmultnetsizetrsh, pargs._k);
	
	printf("\n\n ==== V-Cycle Parameters ==== ");
	printf("\n\t 9. Maximum de grands V-cycles : %d", pargs.bigVcycle);
	printf("\n\t 10. Le maximum de petits V-cycles : %d", pargs.smallVcycle);
	if(pargs.usesamematchinginVcycles == 0)
		printf("\n\t 11. PaToH selectionne automatiquement un algo de grossissement pour chaque V-cycle");
	else
		printf("\n\t 11. PaToH utilisera le meme algo de grossissement durant les V-cycles");
	
	printf("\n\n ==== Use Heap/Buckets Parameters ==== ");
	if(pargs.usebucket == -1)
		printf("\n\t 12. PaToH determine quand utiliser le tas et quand utiliser les buckets");
	else if(pargs.usebucket == 0)	
		printf("\n\t 12. PaToH utilise toujours la structure de donnees tas.");
	else printf("\n\t 12. PaToH utilise toujours la structure de donnees buckets.");
	printf("\n\t 13. Si l'hypergraphe a plus de sommets que %d, alors tas ne sera pas utilise.", pargs.maxcellinheap);
	printf("\n\t 14.15 Si le nombre de sommets pour le niveau i est superieur a \" tailleBucket * (14. / 15.) = taille Bucket * (%d / %d) \" alors tas sera utilise.", pargs.heapchk_mul, pargs.heapchk_div);

	printf("\n\n ==== Memory Allocation Parameters ==== ");
	printf("\n\t 16. %d memoire sera allouee pour contenir ttes les structures de sommets / parties de l'hypergraphe d'origine.", pargs.MemMul_CellNet);
	printf("\n\t 17. %d memoire sera allouee afin de contenir les tableaux d'epingles de l'hypergraphe d'origine.", pargs.MemMul_Pins);
	printf("\n\t 18. %d memoire sera allouee afin de contenir les tableaux d'epingles et des parties de l'hypergraphe d'origine.", pargs.MemMul_General); 
	
	printf("\n\n ==== Coarsening Parameters ==== ");
	printf("\n\t 19. Ordre de visite : "), getVisitOrder(pargs.crs_VisitOrder);
	printf("\n\t 20. Choix de l'algorithme de grossissements :"), getAlgoGrossissement(pargs.crs_alg);
	printf("\n\t 21. Limite le nombre de sommets dans l'hypergraphe a plus gros grains : %d.", pargs.crs_coarsento);
	printf("\n\t 22. Le nombre de sommets dans l'hypergraphe a plus gros grains defini par : %d (#Partitions) * %d.", pargs._k, pargs.crs_coarsentokmult);
	printf("\n\t 23. Arrete le grossissement lorsque le nombre de sommets atteint %d % du nombre initial.", pargs.crs_coarsenper);
	printf("\n\t 24. Limite la construction de gros sommets : le poids maximum alors sera au plus de %.2f * Wavg.", pargs.crs_maxallowedcellwmult);
	printf("\n\t 25. Niveau de demarrage de detection des parties identiques durant le grossissement = %d", pargs.crs_idenafter);
	if(pargs.crs_idenafter < 0) printf(", PaToH genere automatique ce parametre.");
	else printf(".");
	printf("\n\t 26. Les parties dont la taille est egale ou inferieur a %d seront verifiees pour la detection de parties identiques.", pargs.crs_iden_netsizetrh);
	printf("\n\t 27. L'algorithme de grossissement changera apres le niveau %d.", pargs.crs_useafter);
	printf("\n\t 28. Apres le niveau %d, l'algorithme de grossissement sera :", pargs.crs_useafter), getAlgoGrossissement(pargs.crs_useafteralg);
	
	printf("\n\n ==== Both init part & refinement Parameters ==== ");
	printf("\n\t 29. %d d'instances de partitionnement a construire durant la deuxieme phase.", pargs.nofinstances);
 	
	printf("\n\n ==== Initial Partitioning Parameters ==== ");
	printf("\n\t 30. Algorithme pour le partitionnement initial :"), choixAlgoPartitionnementInitial(pargs.initp_alg);
	printf("\n\t 31. %d executions de cet algorithme seront executees pour chaque instance.", pargs.initp_runno);
	if(pargs.initp_ghg_trybalance != 0)
		printf("\n\t 32. PaToH lors de l'algorithme GHGP essayera de trouver de meilleurs parittions equilibrees.");
	else	printf("\n\t 32. PaToH lors de l'algorithme GHGP n'essayera pas de trouver de meilleurs parittions equilibrees.");
	printf("\n\t 33. Algorithme pour l'affinement qui sera utilise ensuite : "), choixAlgoAffinement(pargs.initp_refalg);
	
	printf("\n\n ==== Refinement Parameters ==== ");
	printf("\n\t 34. Algorithme pour l'affinement : "),  choixAlgoAffinement(pargs.ref_alg);
	printf("\n\t 35. Apres le niveau %d, l'algorithme d'affinement sera change.", pargs.ref_useafter);
	printf("\n\t 36. Apres le niveau ci-dessus, PaToH utilisera l'algo suivant : "), choixAlgoAffinement(pargs.ref_useafteralg);
	printf("\n\t 37. Limite le nombre de passes pour chaque niveau d'affinement a %d.", pargs.ref_passcnt);
	printf("\n\t 38. Limite le nombre de deplacements consecutifs avec gains negatifs a %d.", pargs.ref_maxnegmove);
	printf("\n\t 39. Limite le nombre de deplacements consecutifs avec gains negatifs a (#sommets)  * %.4f", pargs.ref_maxnegmovemult);
	printf("\n\t 40. Limite le nombre maximum de déplacement d'un sommet dans une passe a : %d.", pargs.ref_dynamiclockcnt);
	printf("\n\t 41. ?? %.2f", pargs.ref_slow_uncoarsening);
	
	printf("\n\n ==== Imbalance Parameters ==== ");
	printf("\n\t 42. La contrainte d'equilibrage "), choixBalance(pargs.balance);
	printf("\n\t 43. %.2f sera le ratio de desiquilibrage pour la partition initiale, soit l'hypergraphe a plus gros grains.", pargs.init_imbal);
	printf("\n\t 44. %.2f sera le ratio de desequilibrage pour la partition finale.", pargs.final_imbal);
	printf("\n\t 45. Le ratio maximal %.2f * (imbalance) sera force au niveau de chaque niveau d'affinement.", pargs.fast_initbal_mult);
	printf("\n\t 46. Pour la partition initiale, les instances qui ont %.2f * cutsize plus que la partition avec le cutsize minimal sont jetees.", pargs.init_sol_discard_mult);
	printf("\n\t 47. Pour la partition finale, les instances qui ont (%.2f en fonction du niveau augmente) * cutsize plus que la partition avec le cutsize minimal sont jetees.", pargs.final_sol_discard_mult);
	
	printf("\n\n ==== Utility Parameters for Users Parameters ==== ");
	printf("\n\t 48. %s", pargs.allargs);
	printf("\n\t 49. %s", pargs.inputfilename);
	printf("\n\t 50. %d", pargs.noofrun);
	printf("\n\t 51. %d", pargs.writepartinfo);
}

