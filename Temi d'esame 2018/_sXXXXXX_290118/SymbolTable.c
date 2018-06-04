#include "SymbolTable.h"
#include <stdlib.h>
#include <string.h>

struct SymbolTable{
    char **tab;
    int N,sz;
};

static int hash(char*,int);
static int hash(char *s,int N)
{
    int h=0,b=26;
    strupr(s);
    for(;*s != '\0';s++)
        h = (b*h + (*s - (int)'A') + 1) % N;
    return h;
}

static int full(ST,int);
static int full(ST s,int i)
{
    return s->tab[i]!=NULL;
}

ST STinit(int N)
{
    int i;
    ST s   = malloc(sizeof(ST));
    s->tab = malloc(N*sizeof(int*));
    for(i=0;i<N;i++)
        s->tab[i] = NULL;
    s->sz = 0;
    s->N  = N;
    return s;
}

void STinsert(ST st,char *s)
{
    int i = hash(s,st->N);
    while(full(st,i)) i = (i+1)%st->N;
    st->tab[i] = s;
    st->sz++;
}

int STgetIndex(ST st,char *s)
{
    int i = hash(s,st->N);
    while(strcmp(s,st->tab[i])!=0)
        i = (i+1)%st->N;
    return i;
}

char* STretrieve(ST st,int i)
{
    return st->tab[i];
}