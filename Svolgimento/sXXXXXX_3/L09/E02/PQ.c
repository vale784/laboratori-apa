#include "PQ.h"

#include <stdlib.h>

struct Heap{
    entrant_t *arr;
    int size,maxN;
};

/*--------------------------------------------------------------------------------------------------------------------*/

Heap PQinit(int maxN)
{
    Heap new  = malloc(sizeof(*new));
    new->arr  = malloc(maxN* sizeof(*new->arr));
    new->size = 0;  new->maxN = maxN;
    return new;
}

void PQfree(Heap h)
{
    int i;
    for(i=0;i<h->size;i++)
        ENTRfree(h->arr[i]);
    free(h->arr); free(h);
}

Heap PQadd(Heap h,entrant_t x)
{
    int i;
    Heap supp;

    if((h->size + 1) <= (h->maxN))
        h->arr[h->size++] = x;
    else{
        supp       = PQinit(2*h->maxN);
        supp->size = h->size + 1;

        for(i=0;i < h->size;i++)
            supp->arr[i] = h->arr[i];

        supp->arr[h->size] = x;
        PQfree(h);
        return supp;
    }
    return h;
}

/*--------------------------------------------------------------------------------------------------------------------*/

void PQdisplay(FILE *fp,Heap h)
{
    int i;
    for(i=0;i < h->size;i++)
        ENTRdisplay(fp,h->arr[i]);
}

/*--------------------------------------------------------------------------------------------------------------------*/

void PQheapbuild(Heap h)
{
    int i;
    for(i=(h->size)/2-1;i>=0;i--)
        PQheapify(h,i);
}

void PQheapify(Heap h,int i)
{
    int tmp = greater(h,LEFT(i),RIGHT(i),i);
    if(tmp!=i) {
        ENTRswap(&h->arr[tmp],&h->arr[i]);
        PQheapify(h,tmp);
    }
}

int greater(Heap h, int l, int r, int i)
{
    if(l >= h->size)    return i; //r is always major of l
    if(r >= h->size)
        if(l < h->size) return ((ENTRgreater(h->arr[i],h->arr[l]))? (i):(l));

    if(ENTRgreater(h->arr[l],h->arr[i]))
        if(ENTRgreater(h->arr[l],h->arr[r]))
            return l; else return r;
    else if(ENTRgreater(h->arr[i],h->arr[r]))
        return i; else return r;
}

/*--------------------------------------------------------------------------------------------------------------------*/

void PQread(FILE *fp, Heap h)
{
    int i;
    entrant_t tmp;
    for(i=0;((tmp=ENTRbuild(fp))!=NULL);i++)
        h = PQadd(h,tmp);
}


void PQdelete(Heap h)
{
    int i;

    for(i=h->size;ENTRdefeated(h->arr[i]);i--)
        ENTRfree(h->arr[i]);

    h->size = i;
}

entrant_t PQsearch(Heap h,int ID)
{
    int i;

    for(i=0;i<h->size;i++)
        if(KEYequal(h->arr[i],ID))
            return h->arr[i];

    return NULL;
}

void PQresults(Heap h,int a)
//a is binary: 0: the last one wins, 1: the one before the last one
{
    int b = (a==0)? 1:0;
    ENTRresults(h->arr[h->size-1-a],h->arr[h->size-1-b]);
}