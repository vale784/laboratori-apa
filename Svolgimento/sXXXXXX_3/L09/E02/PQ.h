#ifndef L09E02_PQ_H
#define L09E02_PQ_H

#include "Item.h"

#define DEFAULT_INIT_ENTRANTS 15

#define LEFT(A)   ((A)*2+1)
#define RIGHT(A)  ((A)*2+2)
#define PARENT(A) (((A)-1)/2)

typedef struct Heap *Heap;

Heap PQinit(int);
Heap PQadd(Heap,entrant_t);
void PQfree(Heap);
void PQread(FILE *,Heap);

void PQdelete(Heap);
void PQdisplay(FILE *,Heap);

void PQheapbuild(Heap);
void PQheapify(Heap,int);
int  greater(Heap,int,int,int);

entrant_t PQsearch(Heap,int);

void PQresults(Heap,int);


#endif //L09E02_PQ_H
