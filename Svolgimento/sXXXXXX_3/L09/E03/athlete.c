#include <stdio.h>
#include <stdlib.h>
#include "athlete.h"

struct athlete{
    int maxNA,maxAA,maxAI,maxSUM;
};



/*--------------------------------------------------------------------------------------------------------------------*/

AT ATnew(FILE *fp)
{
    AT atNew = malloc(sizeof(*atNew));
    if(fscanf(fp,"%d %d %d %d",&atNew->maxNA,&atNew->maxAA,&atNew->maxAI,&atNew->maxSUM)==EOF) {
        free(atNew); return NULL;
    }
    return atNew;
}

/*--------------------------------------------------------------------------------------------------------------------*/

void ATdisplay(AT x)
{
    printf("%d %d %d %d",x->maxNA,x->maxAA,x->maxAI,x->maxSUM)
}


/*--------------------------------------------------------------------------------------------------------------------*/

int getNA(AT x)
{
    return x->maxNA;
}

int getAA(AT x)
{
    return x->maxAA;
}

int getAI(AT x)
{
    return x->maxAI;
}

int getSum(AT x)
{
    return x->maxSUM;
}

/*--------------------------------------------------------------------------------------------------------------------*/