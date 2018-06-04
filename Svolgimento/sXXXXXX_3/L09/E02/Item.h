#ifndef L09E02_ITEM_H
#define L09E02_ITEM_H

#include <stdio.h>
#include <stdbool.h>

#define twfive_of(A) (((A)<=4)? (A):(((A)/4)+1))

#define DEFAULT_ENTRY_SCORE 10

typedef struct Entrant *entrant_t;

//create/destruction of item
entrant_t ENTRbuild(FILE *);
entrant_t ENTRnew(FILE *,int);
void      ENTRfree(entrant_t);

//comparison function
bool KEYequal(entrant_t,int);
bool ENTRgreater(entrant_t,entrant_t);

//add and remove points
void ENTRresults(entrant_t,entrant_t);

//check if to delete
bool ENTRdefeated(entrant_t);

//display
void ENTRdisplay(FILE *,entrant_t);

//support functions
void ENTRswap(entrant_t *,entrant_t *);

void ENTRzeroscore(entrant_t);


#endif //L09E02_ITEM_H
