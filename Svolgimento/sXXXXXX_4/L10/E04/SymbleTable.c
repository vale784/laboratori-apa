#include "SymbleTable.h"
#include <stdlib.h>

struct SybmleTable{
    Item *st;
    int M,nr,nc;
};

static int hash(ST,Item);
static int hash(ST s,Item x)
{
    return (x.y * s->nc) + x.x;
}

ST STinit(int M,int nr,int nc)
{
    int i;
    ST st  = malloc(sizeof(*st));
    st->st = malloc(M * sizeof(int));
    for(i=0;i<M;i++)
        st->st[i] = ITEMsetVoid();
    st->M  = M; st->nr=nr; st->nc=nc;
    return st;
}

int STinsert(ST s,Item x)
{
    int i = hash(s,x);
    s->st[i] = x;
    return i;
}

int STgetIndex(ST st, Item x)
{
    int i = hash(st,x);
    if(st->st[i].type==ITEMsetVoid().type)
        return -1; else return i;
}

Item STretrieve(ST st,int i)
{
    return st->st[i];
}

void   STeditType(ST s,int i,int newTipe)
{

}

void STfree(ST st)
{
    free(st->st); free(st);
}