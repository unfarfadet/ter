#include <math.h>

#include "template/Sort.h"
#include "PARSER/Parser.h"

using namespace TER;

Parser::Parser(char _base, int _weights):
	base(_base),
	weights(_weights)
	{numClause = 1;}


Parser::~Parser()
{
}


//=================================================================================================
// Minor methods:


Var Parser::newVar()
{
    int v = nVars();
    assigns  .push(l_Undef);
    vardata  .push(mkVarData(CRef_Undef, 0));
    return v;
}


bool Parser::addClause_(vec<Lit>& ps)
{
    // Check if clause is satisfied and remove false/duplicate literals:
    sort(ps);
    Lit p; int i, j;
    for (i = j = 0, p = lit_Undef; i < ps.size(); i++)
        if (value(ps[i]) == l_True || ps[i] == ~p)
            return true;
        else if (value(ps[i]) != l_False && ps[i] != p)
            ps[j++] = p = ps[i];
    ps.shrink(i - j);

	CRef cr = ca.alloc(ps, false);
	clauses.push(cr);
    return true;
}
//=================================================================================================
// Writing CNF to DIMACS:
// 
// FIXME: this needs to be rewritten completely.

static Var mapVar(Var x, vec<Var>& map, Var& max)
{
    if (map.size() <= x || map[x] == -1){
        map.growTo(x+1, -1);
        map[x] = max++;
    }
    return map[x];
}

void Parser::toDimacs(FILE* f, Clause& c, vec<Var>& map, Var& max)
{
    for (int i = 0; i < c.size(); i++)
        if (value(c[i]) != l_False)
            fprintf(f, "%d ", mapVar(var(c[i]), map, max)+1);
    fprintf(f, "0\n");
}

void Parser::toDimacs(FILE* f, const vec<Lit>& assumps)
{
    vec<Var> map; 
    Var max = 0;

    for (int i = 0; i < clauses.size(); i++)
    {
		Clause& c = ca[clauses[i]];
		for (int j = 0; j < c.size(); j++)
			if (value(c[j]) != l_False)
				mapVar(var(c[j]), map, max);
    }

    fprintf(f, "p cnf %d %d\n", max, clauses.size());
    for (int i = 0; i < clauses.size(); i++)
        toDimacs(f, ca[clauses[i]], map, max);
}

void Parser::toPatoh(FILE* f, Clause& c, vec<Var>& map, Var& max)
{
    for (int i = 0; i < c.size(); i++)
    {   
		if (value(c[i]) != l_False)
            fprintf(f, "%d ", mapVar(var(c[i]), map, max)+1);
    }
    fprintf(f,"\n");
}

void Parser::toPatohPrimal(FILE* f, const vec<Lit>& assumps)
{
	vec<Var> map; 
    Var max = 0;
    int nHyperedges = 0;
    for (int i = 0; i < clauses.size(); i++)
    {
		Clause& c = ca[clauses[i]];
		for (int j = 0; j < c.size(); j++)
			if (value(c[j]) != l_False)
			{	
				mapVar(var(c[j]), map, max); 
				nHyperedges++;
			}
    }
    if(weights == 1 || weights ==3)
    {
		fprintf(f, "1 %d %d %d 2\n", max, clauses.size(), nHyperedges);
		for(int i = 0 ; i < clauses.size(); i++)
		{
			fprintf(f, "%d ", numClause++);
			toPatoh(f, ca[clauses[i]], map, max);
		}	
	}
	else
	{
		fprintf(f, "1 %d %d %d\n", max, clauses.size(), nHyperedges);
		for (int i = 0; i < clauses.size(); i++)
			toPatoh(f, ca[clauses[i]], map, max);
	}
}

void Parser::getLigne(int * ligne, Clause& c, vec<Var>& map, Var& max)
{
	for(int i = 0; i < c.size(); i++)
	{
		if(value(c[i]) != l_False)
			ligne[i] = mapVar(var(c[i]), map, max) + 1;
	}
}

void Parser::setAdjacenceBinaire(int ** matrice, int * ligne, int taille)
{
	int x, y;
	for(int i = 0, j = 1 ; i < (taille-1) ; i++, j++)
	{
		x = ligne[i];
		y = ligne[j];
		matrice[x-1][y-1] = 1;
	}
}

void Parser::setAdjacenceDecimal(int ** matrice, int * ligne, int taille, int numClause)
{
	int x;
	//printf("(clause = %d) : ", numClause);
	for(int i = 0, j = 0; i < taille; i++, j = 0)
	{
		x = ligne[i];
		//printf( "var(%d) : ", x);
		while(matrice[x-1][j] != 0) j++;
		matrice[x-1][j] = numClause+1;
	}
}

void Parser::toMatrice(FILE* f, const vec<Lit>& assumps)
{
	vec<Var> map; 
    Var max = 0;
    int nHyperedges = 0, taille = ca[clauses[0]].size();
    
    for (int i = 0; i < clauses.size(); i++)
    {
		Clause& c = ca[clauses[i]];
		for (int j = 0; j < c.size(); j++)
			if (value(c[j]) != l_False)
			{	
				mapVar(var(c[j]), map, max); 
				nHyperedges++;
			}
    }
    int taille_temp = (base == 'B') ? max : clauses.size();
    int * clause = (int*) malloc(sizeof(int) * taille);
    int ** matrice = (int**) malloc(max * sizeof(int*));
	for(int i = 0 ; i < max; i++)
		matrice[i] = (int*) malloc(taille_temp * sizeof(int));

	for(int i = 0; i < max; i++)
		for(int j = 0 ; j < taille_temp ; j++)
			matrice[i][j] = (base == 'B' && i==j) ? -1 : 0;
	
    fprintf(f, "c matrice adjacence %c\n", base);
	fprintf(f, "1 %d %d %d\n", max, clauses.size(), nHyperedges);
	
	for (int i = 0, newTaille = taille ; i < clauses.size(); i++, newTaille = ca[clauses[i]].size())
	{
		if(taille != newTaille)
		{
			taille = newTaille;
			clause = (int*) realloc(clause, sizeof(int) * taille);
		}
		getLigne(clause, ca[clauses[i]], map, max);
		if(base == 'B')
			setAdjacenceBinaire(matrice, clause, taille);
		else if(base == 'D')
			setAdjacenceDecimal(matrice, clause, taille, i);
	}
	for(int i = 0; i < max; i++, fprintf(f,"\n"))
		for(int j = 0; j < taille_temp; j++)
			if(matrice[i][j] == -1)
				fprintf(f, "x ");
			else if(base == 'D' && matrice[i][j] != 0)
				fprintf(f, "%d ", matrice[i][j]);	
			else if(base == 'B')
				fprintf(f, "%d ", matrice[i][j]);	

	free(clause);
	for(int i = 0 ; i < max ; i++)
		free(matrice[i]);
	free(matrice);
}

