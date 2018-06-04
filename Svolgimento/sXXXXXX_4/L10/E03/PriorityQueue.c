#include "PriorityQueue.h"
#include <stdlib.h>

typedef struct pqNode *node;
struct pqNode{
    int item; priority p; node next;
};

struct PriorityQueue{
    node head; int cnt;
};

static void freeR(node);
static void freeR(node x)
{
    if(x==NULL) return;
    freeR(x->next);
    free(x);
}

PQ PQinit(void)
{
    PQ p = malloc(sizeof(*p));
    p->head = NULL; p->cnt = 0;
    return p;
}

void PQput(PQ p,int item,priority pr)
{
    node x = p->head,t = malloc(sizeof(*t));
    t->item = item; t->p = pr; t->next = NULL;

    if(p->head==NULL){
        p->head = t; return;
    }else{
        while(x->next->p<=t->p && x->next!=NULL) x = x->next;
        t->next = x->next; x->next = t;
    }
    p->cnt++;
}

bool PQempty(PQ p)
{
    return p->cnt == 0;
}

int PQget(PQ p)
{
    int tmp;
    node supp = p->head;
    p->head = p->head->next; p->cnt--;
    tmp = supp->item; free(supp);
    return tmp;
}

void PQfree(PQ p)
{
    freeR(p->head); free(p);
}