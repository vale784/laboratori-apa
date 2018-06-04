#include "exArray.h"
#include <stdlib.h>

struct ExArray{
    Ex *allEx;
    int nEx;
};

ExArr EXAbuild(const char *path)
{
    if(path==NULL) {
        printf("I don't support this function yet\n");
        return NULL;
    }

    int i;
    ExArr x = malloc(sizeof(*x));
    FILE *fp = fopen(path,"r");
    if(fp==NULL) return NULL;

    fscanf(fp,"%d",&x->nEx);
    x->allEx = malloc(x->nEx* sizeof(*x->allEx));

    for(i=0 ;i < x->nEx; i++)
        x->allEx[i] = EXbuild(fp);

    return x;
}

void EXAfree(ExArr x)
{
    free(x->allEx); free(x);
}

int EXAsearch(ExArr x, const char *s,int n,EXkey_cmp k)
{
    int i;

    for(i=0;i<x->nEx;i++)
        switch(k) {
            case s_name:
                if (EXkey_compare(x->allEx[i], s, 0, s_name)) {
                    EXdisplay(x->allEx[i], stdout);
                    return 0;
                }
            case s_index:
                if (EXkey_compare(x->allEx[i], s, 0, s_name)) {
                    return i;
                }
        }
    printf("No exercise found\n");
    return 0;
}

char* EXAgetName(ExArr x, int i)
{ return x->allEx[i].exName; }