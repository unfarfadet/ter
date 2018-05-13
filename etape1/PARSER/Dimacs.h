#ifndef TER_Dimacs_h
#define TER_Dimacs_h

#include <stdio.h>

#include "ParseUtils.h"
#include "SolverTypes.h"

namespace TER {

//=================================================================================================
// DIMACS Parser:

template<class B, class Parser>
static void readClause(B& in, Parser& P, vec<Lit>& lits) {
    int     parsed_lit, var;
    lits.clear();
    for (;;){
        parsed_lit = parseInt(in);
        if (parsed_lit == 0) break;
        var = abs(parsed_lit)-1;
        while (var >= P.nVars()) P.newVar();
        lits.push(mkLit(var));
    }
}

template<class B, class Parser>
static void parse_DIMACS_main(B& in, Parser& P) {
    vec<Lit> lits;
    int vars    = 0;
    int clauses = 0;
    int cnt     = 0;
    for (;;){
        skipWhitespace(in);
        if (*in == EOF) break;
        else if (*in == 'p'){
            if (eagerMatch(in, "p cnf")){
                vars    = parseInt(in);
                clauses = parseInt(in);
            }else{
                printf("PARSE ERROR! Unexpected char: %c\n", *in), exit(3);
            }
        } else if (*in == 'c' || *in == 'p')
            skipLine(in);
        else if(*in == '%') break;
        else
        {
            cnt++;
            readClause(in, P, lits);
            P.addClause_(lits); }
    }
    if (vars != P.nVars())
        fprintf(stderr, "WARNING! DIMACS header mismatch: wrong number of variables.\n");
    if (cnt  != clauses)
        fprintf(stderr, "WARNING! DIMACS header mismatch: wrong number of clauses.\n");
}

// Inserts problem into parser.
//
template<class Parser>
static void parse_DIMACS(gzFile input_stream, Parser& P) {
    StreamBuffer in(input_stream);
    parse_DIMACS_main(in, P); }

//=================================================================================================
}

#endif
