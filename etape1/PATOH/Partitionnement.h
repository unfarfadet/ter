#ifndef TER_Partitionnement_h
#define TER_Partitionnement_h

#include "PATOH/Patoh.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
using namespace std;

namespace TER {

	class Partitionnement {
		public:
			Partitionnement(int _nb, char * _name);
			virtual ~Partitionnement();
			void toPartitions(char * filename);
			void calculCutset(double P1, double P2);
		private:
			int nb_Parts;
			char * name;
			char * file_partage;
			int findClause(vector<int> liste, int num);
	};
}

#endif
