#include <malloc.h>
#include <mem.h>
#include <stdbool.h>
#include "SymbleTable.h"

/*--------------------------------------------------------------------------------------------------------------------*/

struct SymbleTable{
    Item *st;
    int M;
};

static int  hash(char*,int);
static bool full(ST,int);

/*--------------------------------------------------------------------------------------------------------------------*/

static int hash(char *s, int M)
{
    int h = 0, b = 26;
    for(;*s!='\0';s++)
        h = (b*h + (*s-((int)'A'))+1) % M;
    return h;
}

static bool full(ST st,int i)
{
    return (st->st[i].name != (ITEMsetvoid()).name);
}

/*--------------------------------------------------------------------------------------------------------------------*/

ST STinit(int N)
{
    int i;
    ST wr  = malloc(sizeof(*wr));
    wr->st = malloc(N*sizeof(Item));
    for(i=0;i<N;i++)
        wr->st[i] = ITEMsetvoid();
    wr->M  = 0;
    return wr;
}

int STinsert(ST st,Item a)
{
    int i = hash(strupr(a.name),st->M);
    while(full(st,i)) i = (i+1) % st->M;
    st->st[i].name  = a.name;
    st->st[i].netId = a.netId;
    return i;
}

int STgetIndex(ST st,Item s)
{
    int tmp, i = hash(strupr(s.name),st->M);
    tmp = i;
    while((full(st,i))&&(i!=-1))
        i = (i==tmp-1)? (-1):((i+1) % st->M);

    return i;
}

Item STretrieve(ST st,int i)
{
    return st->st[i];
}

Item STsearch(ST st,char *s)
{
    Item supp;
    supp.name  = s;
    supp.netId = NULL;
    return STretrieve(st,STgetIndex(st,supp));
}

void STfree(ST st)
{
    int i;
    for(i=0;i<st->M;i++)
        if(full(st,i)) {
            free(st->st[i].name);
            free(st->st[i].netId);
        }
    free(st->st); free(st);
}