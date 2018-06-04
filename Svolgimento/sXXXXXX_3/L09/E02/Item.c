#include "Item.h"

#include <stdlib.h>
#include <string.h>

/*--------------------------------------------------------------------------------------------------------------------*/
struct Entrant{
    char *name;
    int ID,score;
};

/*--------------------------------------------------------------------------------------------------------------------*/

entrant_t ENTRbuild(FILE *fp)
{
    if(fp==NULL)
        return ENTRnew(stdin,DEFAULT_ENTRY_SCORE);

    return ENTRnew(fp,-1);
}

entrant_t ENTRnew(FILE *fp,int score)
{
    entrant_t new = malloc(sizeof(*new));
    char buf[30];

    if(fscanf(fp,"%d %s",&new->ID,buf)==EOF) return NULL;
    new->name = strdup(buf);

    if(score==-1) fscanf(fp,"%d",&new->score);

    return new;
}

void ENTRfree(entrant_t x)
{ free(x->name); free(x); }

/*--------------------------------------------------------------------------------------------------------------------*/

bool KEYequal(entrant_t x,int ID)
{ return (x->ID==ID); }

bool ENTRgreater(entrant_t a, entrant_t b)
{ return (a->score > b->score); }

/*--------------------------------------------------------------------------------------------------------------------*/

void ENTRresults(entrant_t winner,entrant_t loser)
{
    int tmp = twfive_of(loser->score);
    printf("winner is\n"); ENTRdisplay(stdout,winner);
    printf("new score->%d",winner->score += tmp);
    printf("loser is\n");  ENTRdisplay(stdout,loser);
    printf("new score->%d",loser->score -= tmp);
}

bool ENTRdefeated(entrant_t x)
{ return (x->score <= 0); }

void ENTRzeroscore(entrant_t x)
{ if(x!=NULL) (x->score = 0); }

/*--------------------------------------------------------------------------------------------------------------------*/

void ENTRdisplay(FILE *fp,entrant_t x)
{ fprintf(fp,"%d %s %d\n",x->ID,x->name,x->score); }

/*--------------------------------------------------------------------------------------------------------------------*/

void ENTRswap(entrant_t *a,entrant_t *b)
{ entrant_t *c = a; a = b; b = c; }

