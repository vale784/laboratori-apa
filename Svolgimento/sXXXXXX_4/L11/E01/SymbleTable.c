#include "SymbleTable.h"
#include <stdlib.h>
#include <string.h>


struct SymbleTable{
    char **tab; int size;
};

static int hash(char*,int);
static int hash(char *s,int M)
{
    int h=0,b=26;
    s = strupr(s);
    for(;*s!=0;s++)
        h = (b * h +(*s - (int) 'A') + 1 ) % M;
    return h;
}

static int full(ST,int);
static int full(ST s,int i)
{
    return s->tab[i]!=NULL;
}

ST     STinit(int sz)
{
    int i;
    ST s    = malloc(sizeof(*s));
    s->tab  = malloc(sz * sizeof(char *));
    for(i=0;i<sz;i++)
        s->tab[i] = NULL;
    s->size = sz;
    return s;
}

void   STinsert(ST tab,char *s)
{
    int i = hash(s,tab->size);
    while(full(tab,i)) i++;
    tab->tab[i] = s;
}

int    STsearch(ST tab,char *s)
{
    int i = hash(s,tab->size);
    while(strcmp(tab->tab[i],s)!=0)
        i++;
    return i;
}

char*  STretrieve(ST tab,int i)
{
    return tab->tab[i];
}

void   STfree(ST tab)
{
    int i;
    for(i=0;i<tab->size;i++)
        free(tab->tab[i]);
    free(tab->tab); free(tab);
}