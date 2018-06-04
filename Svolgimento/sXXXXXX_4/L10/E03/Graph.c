#include <stdlib.h>
#include <stdio.h>
#include <mem.h>
#include "Graph.h"

/*--------------------------------------------------------------------------------------------------------------------*/

typedef struct adjNode *adjLink;

struct adjNode{
    int node;
    int wt;
    adjLink next;
};

struct Graph{
    int    **adjM;
    adjLink *adjL;
    ST       st;
    int     *adjCnt;
    int      nE,nV;
};

typedef enum {e_read,e_makeMatrix};

/*--------------------------------------------------------------------------------------------------------------------*/

static int ** MATRIXinit(int,int,int);
static int ** MATRIXinit(int r,int c,int val)
{
    int **m,i,j;
    m = malloc(r*sizeof(int*));
    for(i=0;i<r;i++){
        m[i] = malloc(c*sizeof(int));
        for(j=0;j<c;j++)
            m[i][j]=val;
    }
    return m;
}

static adjLink * LISTinit(int);
static adjLink * LISTinit(int v)
{
    adjLink *x = malloc(v*sizeof(*x));
    int i;
    for(i=0;i<v;i++) x[i] = NULL;
    return x;
}

static void GRinsertMatrix(GR,ED);
static void GRinsertMatrix(GR g,ED e)
{
    if(g->adjM[e.a][e.b]!=0) return;
    g->adjM[e.a][e.b] = e.wh;
    g->adjM[e.b][e.a] = e.wh;
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

    t->node = e.b; t->wt=e.wh; t->next = NULL;
    x->node = e.a; x->wt=e.wh; x->next = NULL;
}

static int bfs(GR,int*,int*);
static int bfs(GR g,int *time,int *pre)
{
    int sum = 0,supp;
    PQ  pq = PQinit();
    adjLink t;
    PQput(pq,0,0);
    while(!PQempty(pq))
        if(pre[(supp = PQget(pq))] == -1){
            pre[supp] = (*time)++;
            for(t = g->adjL[supp];t != NULL;t = t->next)
                if(pre[t->node] == -1){
                    PQput(pq,t->node,*time);
                    sum += t->wt;
                }
        }
    return sum;
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

static void MATRIXfree(int**,int);
static void MATRIXfree(int **mat,int r)
{
    int i;
    for(i=0;i<r;i++)
        free(mat[i]);
    free(mat);
}

/*--------------------------------------------------------------------------------------------------------------------*/

GR GRinit(int V)
{
    GR g      = malloc(sizeof(*g));
    g->adjCnt = calloc((size_t)V,sizeof(int));
    g->adjL   = LISTinit(V);
    g->st     = STinit(V);
    g->nV     = V;
    g->nE     = 0;
    return g;
}

void GRread(GR g,Item a,Item b,int wt)
{
    int i1,i2;
    i1 = STgetIndex(g->st,a);
    i2 = STgetIndex(g->st,b);
    if(i1==-1) i1 = STinsert(g->st,a);
    if(i2==-1) i2 = STinsert(g->st,b);
    GRinsert(g,EDGEcreate(i1,i2,wt),e_read);
}

void GRinsert(GR g,ED e,int en)
{
    if(e.a==e.b) return;
    else{
        g->adjCnt[e.a]++; g->adjCnt[e.b]++;
    }
    switch(en){
        case e_makeMatrix: GRinsertMatrix(g,e); break;
        case e_read:       GRinsertList(g,e);   break;
        default:                                break;
    }
}

void GRshowNodes(GR g)
{
    int i;
    Item x;
    printf("All nodes:\n");
    for(i=0;i<g->nV;i++)
        if(g->adjL[i]!=NULL){
            x = STretrieve(g->st,i);
            printf("%s\n",x.name);
        }
}

void GRshowEdges(GR g,char *s)
{
    int     i = STgetIndex(g->st,STsearch(g->st,s)),k;
    adjLink t = g->adjL[i];

    printf("there are %d nodes connected to %s:\n",g->adjCnt[i],s);

    for(k=1;t->NULL;t = t->next,k++)
        printf("Edge %d: %s - %s",k,s,STretrieve(g->st,t->node).name);
}

void GRliToMat(GR g)
{
    int i;
    adjLink t;
    g->adjM = MATRIXinit(g->nV,g->nV,0);
    for(i=0;i<g->nV;i++)
        for(t=g->adjL[i];t!=NULL;t=t->next)
            g->adjM[i][t->node] = t->wt;
}

int GRcalculatewt(GR g)
{
    int sum, time = 0, *pre;
    pre = malloc(g->nV*sizeof(int));
    memset(pre,-1,g->nV*sizeof(int));
    sum = bfs(g,&time,pre);
    free(pre);
    return sum;
}

void GRfree(GR g)
{
    LISTfree(g->adjL,g->nV);
    MATRIXfree(g->adjM,g->nV);
    STfree(g->st);
    free(g->adjCnt);
    free(g);
}