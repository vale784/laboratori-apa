#include "athletes.h"
#include <stdlib.h>
#include <string.h>


Atl ATbuild(FILE *fp)
{
    Atl newAT;
    char name[maxLength],surname[maxLength],cate[maxLength],date[maxLength];

    if(fp == stdin)
        printf("Type New Athlete in format: \n"
               "A<code> <name> <surname> <category> <date of birth> <weekly training hours>\n");

    fscanf(fp,"A%d %s %s %s %s %d\n",&newAT.code,name,surname,cate,date,&newAT.hours);

    (newAT.name)    = strdup(name);  (newAT.surname) = strdup(surname);
    (newAT.cate)    = strdup(cate);  (newAT.date)    = strdup(date);

    return newAT;
}

void ATdisplay(Atl x,FILE *fp)
{
    fprintf(fp,"A%0.6d %s %s %s %s %d\n",x.code,x.name,
            x.surname,x.cate,x.date,x.hours);
}

void ATdelete(Atl x)
{
    free(x.name); free(x.surname);
    free(x.cate); free(x.date);
}

bool ATkey_compare(Atl x1,const char *s,int n,ATkey_cmp k)
{
    int l1,l2;
    size_t l;
    if(k==s_code)
        return (x1.code == n);
    if(k==s_surname) {
        l1 = strlen(x1.surname); l2 = strlen(s);
        l = (size_t) ((l1 < l2)? l2:l1);
        return (strncmp(x1.surname,s,l)==0);
    }

    return false;
}