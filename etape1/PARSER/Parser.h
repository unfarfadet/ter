/****************************************************************************************[Solver.h]
Copyright (c) 2003-2006, Niklas Een, Niklas Sorensson
Copyright (c) 2007-2010, Niklas Sorensson

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************/

#ifndef TER_Parser_h
#define TER_Parser_h

#include "template/Vec.h"
#include "template/Alg.h"
#include "SolverTypes.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

namespace TER {

class Parser {
public:

    // Constructor/Destructor:
    //
    Parser(char _base, int _weights);
    virtual ~Parser();

    // Problem specification:
    //
    Var     newVar    (); /// Add a new variable with parameters specifying variable mode.
    bool    addClause_(vec<Lit>& ps);                     /// Add a clause to the solver without making superflous internal copy. Will change the passed vector 'ps'.
    void    toDimacs     (FILE* f, const vec<Lit>& assumps);            // Write CNF to file in DIMACS-format.
    void    toDimacs     (FILE* f, Clause& c, vec<Var>& map, Var& max);
    void    toDimacs     (FILE* f); // Convenience versions of 'toDimacs()':
    
    void 	toPatoh(FILE* f, Clause& c, vec<Var>& map, Var& max);
    
    void 	toPatohPrimal(FILE* f);
    void 	toPatohDual(FILE* f);
    void	toMatrice(FILE* f);
    
    // Read state:
    lbool   value      (Var x) const;       // The current value of a variable.
    lbool   value      (Lit p) const;       // The current value of a literal.
    int     nAssigns   ()      const;       // The current number of assigned literals.
    int     nClauses   ()      const;       // The current number of original clauses.
    int     nVars      ()      const;       // The current number of variables.

protected:

    // Helper structures:
    //
    struct VarData { CRef reason; int level; };
    static inline VarData mkVarData(CRef cr, int l){ VarData d = {cr, l}; return d; }
    
    vec<CRef>           clauses;          // List of problem clauses.
    vec<lbool>          assigns;          // The current assignments.
    vec<VarData>        vardata;          // Stores reason and level for each variable.
    ClauseAllocator     ca;
                                                    // Return the next decision variable.
    // Operations on clauses:
    //
    void     attachClause     (CRef cr);               // Attach a clause to watcher lists.
    
private:
	int 	numClause;
	char	base;
	int 	weights;
	void	toPatohPrimal(FILE* f, const vec<Lit>& assumps);
	void	toPatohDual(FILE* f, const vec<Lit>& assumps);
	void 	toMatrice(FILE* f, const vec<Lit>& assumps);
	void	setAdjacenceBinaire(int ** matrice, int * ligne, int taille);
	void	setAdjacenceDecimal(int ** matrice, int * ligne, int taille, int numClause);
	void 	getLigne(int * ligne, Clause& c, vec<Var>& map, Var& max);
	    
};


//=================================================================================================

inline lbool    Parser::value         	(Var x) const   { return assigns[x]; }
inline lbool    Parser::value         	(Lit p) const   { return assigns[var(p)]; }
inline int      Parser::nVars         	()      const   { return vardata.size(); }
inline void     Parser::toDimacs     	(FILE* f){ vec<Lit> as; toDimacs(f, as); }
inline void     Parser::toPatohPrimal   (FILE* f){ vec<Lit> as; toPatohPrimal(f, as); }
inline void     Parser::toMatrice 		(FILE* f){vec<Lit> as; toMatrice(f, as); }
inline void     Parser::toPatohDual  	(FILE* f){vec<Lit> as; toPatohDual(f, as); }

}

#endif
