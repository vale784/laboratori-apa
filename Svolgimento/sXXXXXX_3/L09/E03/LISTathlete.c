#include "LISTathlete.h"
#include <stdlib.h>


struct athlete_node{
    AT  athlete;
    ATlink next;
};

struct athletes_w{
    ATlink  head;
    int nAT;
};

AW AWbuild(FILE *fp)
{
    AW awNew = malloc(sizeof(*awNew));
    ATlink x;
    int cnt=1;

    awNew->head = x = newNode(fp);

    while((x = x->next = newNode(fp))!=NULL) cnt++;
    awNew->nAT = cnt;

    return awNew;
}

ATlink newNode(FILE *fp)
{
    ATlink x = malloc(sizeof(*x));
    if((x->athlete = ATnew(fp))==NULL){
        free(x); return NULL;
    }
    return x;
}

/*--------------------------------------------------------------------------------------------------------------------*/

ATlink getHead(AW x)
{
    return x->head;
}

ATlink getNext(ATlink x)
{
    return x->next;
}

AT getAthlete(ATlink x)
{
    return x->athlete;
}

int AWsize(AW x)
{
    return x->nAT;
}