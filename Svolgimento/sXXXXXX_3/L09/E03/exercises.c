#include "exercises.h"
#include <stdlib.h>
#include <string.h>

/*it extracts the char in the void pointer A*/
#define charIn(A)  (*((char*)(A)))
/*it extracts the char pointer from the void pointer A*/
#define pointIn(A) ((char *)(A))

/*--------------------------------------------------------------------------------------------------------------------*/

struct Exercise{
    char  *name;
    float  expScore;
    int    diff; //0-6
    Cate_e type;
};

/*--------------------------------------------------------------------------------------------------------------------*/

EX EXnew(FILE *fp,Cate_e c)
{
    EX new = malloc(sizeof(*new));
    char buf[25];

    if(fscanf(fp,"%s %d %f",buf,&new->diff,&new->expScore)==EOF){
        EXfree(new); return NULL;
    }
    new->name = strdup(buf); new->type = c;

    return new;
}

void EXfree(EX del)
{
    free(del->name);
    free(del);
}

void EXdisplay(EX x)
{
    printf("%s %d %f\n",x->name,x->diff,x->expScore);
}

/*--------------------------------------------------------------------------------------------------------------------*/

void *KEYget(EX x)
{
    return (void *)(&x->name);
}

int getDiff(EX x)
{
    return x->diff;
}

Cate_e getCate(EX x)
{
    return x->type;
}

float getScore(EX x)
{
    return x->expScore;
}

bool KEYeq(void *a,void *b)
{
    return (strcmp(pointIn(a),pointIn(b))==0);
}

/*--------------------------------------------------------------------------------------------------------------------*/

int hashing(void *a,int M)
{
    int h=0, base = STD_BASE_HASH;
    for(;(charIn(a)!='\0');a++)
        h = (h*base + charIn(a)) % M;
    return h;
}

/*--------------------------------------------------------------------------------------------------------------------*/

