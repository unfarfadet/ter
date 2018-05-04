#ifndef GESTIONNAIRE
#define GESTIONNAIRE

#include "patoh.h"

class gestionnaire
{
	public:
		gestionnaire(PaToH_Parameters _pargs, int _contraintes);
		void affichage_param();
	private:
		PaToH_Parameters pargs;
	    int contraintes;
	    
	    void getVisitOrder(const int choix);
		void getAlgoGrossissement(const int choix);
		void choixAlgoPartitionnementInitial(const int choix);
		void choixAlgoAffinement(const int choix);
		void choixBalance(const int choix);
};
#endif
