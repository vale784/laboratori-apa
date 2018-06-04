#include "Item.h"

#include <string.h>
#include <malloc.h>

struct Item{
    char *s;
    int n;
};


Item ITEMsetvoid(void)
{
    Item x = malloc(sizeof(*x));
    x->n = -1; x->s = NULL;
    return x;
}

Item ITEMnew(FILE *fp)
{
    Item x = malloc(sizeof(*x));
    char buf[100];
    if(fscanf(fp,"%s %d",buf,&x->n)==EOF)
        return NULL;
    x->s = strdup(buf);
    return x;
}

void ITEMfree(Item x)
{ free(x->s); free(x);}

void ITEMprint(Item x,FILE *out)
{ fprintf(out,"%s %d",x->s,x->n) }

bool ITEMless(Item a,Item b)
{ return (strcmp(a->s,b->s)<0); }

bool ITEMmajor(Item a,Item b)
{ return (strcmp(a->s,b->s)<0); }

char* ITEMgetKey(Item x)
{ return x->s; }

