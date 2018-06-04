#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include "Item.h"

typedef struct stationVisited *sLink;
struct stationVisited{
    char stationId[MAX_STAT_ID_LENGTH]; int timesVisited; sLink next;
};

struct User{
    char cardId[MAX_USR_ID_LENGTH+1]; sLink head; bool activeRent; int time, statVisited;
};

Item  ITEMsetVoid(void)
{
    Item tmp = malloc(sizeof(*tmp));
    tmp->head   = NULL; tmp->activeRent = false;
    strcpy(tmp->cardId," "); tmp->time = -1;
}

int  KEYcompare(Key a,Key b)
{
    return strcmp(a,b);
}

Key KEYget(Item x)
{
    return x->cardId;
}

bool  ITEMless(Item a,Item b)
{
    return (strcmp(a->cardId,b->cardId)<0);
}

void  ITEMshow(Item x)
{
    printf("user %s has %s an active Rent, user were in %d stations\n",
           x->cardId,(x->activeRent==true)? "":"not",x->statVisited);
}

Item ITEMnew(char *cardId,char *stationId,int time)
{
    Item x = malloc(sizeof(*x)); x->activeRent = true;
    strcpy(x->cardId,cardId); x->head = malloc(sizeof(*x->head));
    strcpy(x->head->stationId,stationId); x->head->timesVisited = 1;
    x->statVisited = 1; x->time = time;
    return x;
}

bool ITEMactiveRent(Item x)
{
    return x->activeRent;
}

void ITEMstartRent(Item x,char *stationId,int time)
{
    sLink t = stationSearch(x->head,stationId);
    x->activeRent = true; x->time = time;

    if(t==NULL){
        x->statVisited++;
        stationInsert(x->head,newStation(stationId));
    }else{
        t->timesVisited++;
    }
}

sLink stationSearch(sLink head, char *stationId)
{
    sLink x;

    for(x=head;x!=NULL;x=x->next)
        if(strcmp(x->stationId,stationId)==0)
            return x;
    return NULL;
}

void stationInsert(sLink head,sLink new)
{
    sLink t;
    for(t=head;t->next!=NULL;t->next);
    t->next = new; new->next = NULL;
}

sLink newStation(char *stationId)
{
    sLink x = malloc(sizeof(*x));
    strcpy(x->stationId,stationId);
    x->timesVisited = 1;
    return x;
}

void ITEMstopRent(Item x,int time)
{
    x->activeRent = false;
    printf("%s rental's time is %d",x->cardId,(time - x->time));
}