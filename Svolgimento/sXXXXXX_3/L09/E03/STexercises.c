#include "STexercises.h"
#include <stdlib.h>
#include <mem.h>

/*--------------------------------------------------------------------------------------------------------------------*/

struct ST_ex{
    EX *a;
    int N,M;
};

/*--------------------------------------------------------------------------------------------------------------------*/

void STinsert(ST st,EX x)
{
    int i = 0,start = hashing(KEYget(x),st->M),index = start;

    while(full(st,index))//probing
        index = (start + C1_PROBING*(++i) + C2_PROBING*i*i)%st->M;

    st->a[index] = x; st->N++;
}

bool full(ST st,int i)
{
    return (st->a[i]!=NULL);
}

static ST STinit(int M)
{
    int i;
    ST st = malloc(sizeof(*st));
    st->a = malloc(M* sizeof(*st->a));

    for(i=0;i<M;i++)
        st->a[i]=NULL;
}

static inline Cate_e cate(int i,int C1,int C2)
{
    return ((i<C1)? na:((i<C2)? aa:ai));
}

ST STbuild(FILE *fp,int M)
{
    ST st = STinit(M);
    EX x;
    int NA,AA,AI,sum,i;

    fscanf(fp,"%d %d %d",&NA,&AA,&AI);
    sum = NA + AA + AI;

    for(i=0;(i<sum)&&((x=EXnew(fp,cate(i,NA,AA)))!=NULL);i++)
        STinsert(st,x);

    return st;
}

/*--------------------------------------------------------------------------------------------------------------------*/

int getIndex(ST st,EX x)
{
    int i=0, start = hashing(KEYget(x),st->M), index = start;

    while(!KEYeq(KEYget(x),KEYget(st->a[index])))
        index = (start + C1_PROBING*(++i) + C2_PROBING*i*i)%st->M;

    return index;
}

EX getExercise(ST st,int i)
{
    return (st->a[i]);
}

/*--------------------------------------------------------------------------------------------------------------------*/

int *getCompatibleEX(ST st,int maxNA_diff,int maxAA_diff,int maxAI_diff,int *n)
{
    int i,cnt,tmp=0,arr[st->N],*toRet;

    memset(arr,0,st->N * sizeof(int));

    for(i=0,cnt=0;i<st->M;i++)
        if(full(st,i)) {
            switch(getCate(st->a[i])){
                case na:
                    tmp = maxNA_diff; break;
                case aa:
                    tmp = maxAA_diff; break;
                case ai:
                    tmp = maxAI_diff; break;
            }
            if(getDiff(st->a[i])<=tmp)
                arr[cnt++] = i;
        }

    toRet = malloc((++cnt)*sizeof(int));
    memcpy(toRet,arr,cnt * sizeof(int));
    *n = cnt;

    return toRet;
}

float STgetScore(ST st,int i)
{
    return getScore(st->a[i]);
}

/*--------------------------------------------------------------------------------------------------------------------*/

