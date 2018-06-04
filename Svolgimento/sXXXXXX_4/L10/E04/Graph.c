#include "Graph.h"
#include <stdlib.h>
#include <stdbool.h>
#include <mem.h>
#include <stdio.h>

typedef struct adjNode *adjLink;

struct adjNode{
    int node;
    adjLink next;
};

struct Graph{
    adjLink *adjL;
    int      nE,nV,nU;
};

typedef struct BestSolution *sol;
struct BestSolution{
    ED e; sol next;
};

ST st;

static adjLink * LISTinit(int);
static adjLink * LISTinit(int v)
{
    adjLink *x = malloc(v*sizeof(*x));
    int i;
    for(i=0;i<v;i++) x[i] = NULL;
    return x;
}

static void GRinsertList(GR,ED);
static void GRinsertList(GR g,ED e)
{
    adjLink t = g->adjL[e.a],x = g->adjL[e.b];
    g->nE++;
    if(t!=NULL){
        for(;t->next!=NULL;t = t->next)
            if(t->node==e.b) return;
        t->next = malloc(sizeof(*t)); t = t->next;
    }else t = malloc(sizeof(*t));

    if(x!=NULL){
        for(;x->next!=NULL;x = x->next)
            if(x->node==e.a) return;
        x->next = malloc(sizeof(*x)); x = x->next;
    }else x = malloc(sizeof(*x));

    t->node = e.b; t->next = NULL;
    x->node = e.a; x->next = NULL;
}

static void dfs_searchPath(GR,ED,sol,int,int,int*,int*,bool);
static void dfs_searchPath(GR g,ED e,sol S,int exit,int time,int *pre,int *cnt,bool found)
{
    if(STretrieve(st,e.b).type == exit){
        found = true; g->nU--; STeditType(st,e.b,'X'); S->next = NULL;return;
    }
    adjLink t; ED x;

    pre[e.b] = time++;
    for(t=g->adjL[e.b];t != NULL;t = t->next){
        dfs_searchPath(g,(x = EDcreateInt(e.b,t->node)),S,exit,time,pre,cnt,found);
        if(found){
            (*cnt)++; S->e = x; S->next = malloc(sizeof(*S)); return;
        }
    }
}

static void LfreeR(adjLink);
static void LfreeR(adjLink x)
{
    if(x==NULL) return;
    LfreeR(x->next);
    free(x);
}

static void LISTfree(adjLink*,int);
static void LISTfree(adjLink *arr,int nV)
{
    int i;
    for(i=0;i<nV;i++)
        LfreeR(arr[i]);
    free(arr);
}


GR GRinit(int V,int nr,int nc)
{
    GR g      = malloc(sizeof(*g));
    g->adjL   = LISTinit(V);
    st        = STinit(V,nr,nc);
    g->nV     = V;
    g->nE     = 0;
    return g;
}

ED EDcreate(Item a,Item b)
{
    int i,j;
    ED  c;
    if((i = STgetIndex(st,a))== -1)
        i = STinsert(st,a);
    if((j = STgetIndex(st,b))== -1)
        j = STinsert(st,b);
    c.a = i; c.b = j;
    return c;
}

ED EDcreateInt(int a,int b)
{
    ED  c;
    c.a = a; c.b = b;
    return c;
}

void GRinsert(GR g,ED e)
{
    GRinsertList(g,e);
}

void GRdfs_searchPath(GR g,Item I,int Exit,int nExit)
{
    int tmp = 0,min = g->nV + 1,*pre,i;
    sol S = malloc(sizeof(sol)),bSol,x;
    pre   = malloc(g->nV* sizeof(int));
    for(i=0;i<g->nV;i++) pre[i]=-1;

    for(g->nU = nExit;g->nU == 0;){
        dfs_searchPath(g,EDcreate(I,I),S,'U',0,pre,&tmp,false);
        if(tmp<min){
            min = tmp; bSol = S;
        }
    }
    for(x=bSol;x!=NULL;x=x->next)
        printf("step %d to %d",x->e.a,x->e.b);
}

void GRfree(GR g)
{
    LISTfree(g->adjL,g->nV);
    STfree(st);
    free(g);
}