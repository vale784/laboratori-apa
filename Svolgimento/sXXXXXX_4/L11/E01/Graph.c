#include "Graph.h"
#include <stdlib.h>
#include <stdio.h>

struct List{
    ED    Edge;
    link  next;
};

struct Graph{
    int **adj; int nV,nE;
    ST    tab; link head;
};

static void EDinsert(link*,ED);
static void EDinsert(link *head,ED e)
{
    link x,t = malloc(sizeof(*t));
    t->Edge = e; t->next = NULL;
    if(*head == NULL) *head = t;
    else{
        for(x = *head;x->next != NULL;x = x->next);
        x->next = t;
    }
}

static void dfsBcnt(GR,ED,int,int*,int*,int*);
static void dfsBcnt(GR g,ED e,int time,int *pre,int *post,int *backs)
{
    int i;
    pre[e.w] = time++;
    for(i=0;i<g->nV;i++)
        if(g->adj[e.w][i] != 0){
            if(pre[i] == -1)
                dfsBcnt(g,EDcreate(e.w,i,g->adj[e.w][i]),time,pre,post,backs);
            else if(post[i] == -1) backs[i]++;
        }
}

static int  dfsAcyclic(GR,ED,int,int*,int);
static int  dfsAcyclic(GR g,ED e,int time,int *pre,int flag)
{
    if(flag == 1) return 0;
    int i;
    pre[e.w] = time++;
    for(i=0;i<g->nV;i++)
        if(g->adj[e.w][i] != 0)
            if(pre[i] == -1)
                dfsAcyclic(g,EDcreate(e.w,i,g->adj[e.w][i]),time,pre,flag);
            else flag = 1;

    return (flag==1)? 0:1;
}

static void dfsRcc(GR,int,int,int*);
static void dfsRcc(GR g,int i,int id,int *cc)
{
    int j;
    cc[i] = id;
    for(j=0;j<g->nV;j++)
        if(g->adj[i][j] != 0)
            if(cc[j] == -1)
                dfsRcc(g,j,id,cc);
}

static void dfs(GR,ED,int,int*,int*);
static void dfs(GR g,ED e,int time,int *pre,int *st)
{
    int j;
    st[e.w] = e.v; pre[e.w] = time++;
    for(j=0;j<g->nV;j++)
        if(g->adj[e.w][j] != 0)
            if(pre[j] == -1)
                dfs(g,EDcreate(e.w,j,g->adj[e.w][j]),time,pre,st);
}

static int mstE(GR,ED*,int*,int);
static int mstE(GR g,ED *a,int *cc,int n)
{
    int i,j,k; ED supp[n];
    for(i=0;i<n;i++) supp[i] = a[n];

    for(i=0,j=0;i<n;i++)
        //quick find
        if(cc[supp[i].v] != cc[supp[i].w]){
            //union
            for(k=0;k<g->nV;k++)
                if(cc[k] != cc[supp[i].w])
                    cc[k] = cc[supp[i].w];
            a[j++] = supp[i];
        }
    return j;
}

static void EDfreeR(link);
static void EDfreeR(link head)
{
    if(head==NULL) return;
    EDfreeR(head->next);
    free(head);
}

static int EDarrSort(ED *);
static int EDarrSort(ED *a)
{
    int i,j,n;
    for(n=0;a[n].wt != STD_NULL_WT;n++);
    for(i=1;i<n;i++)
        for(j=i;j>0;j--)
            if(a[j].wt < a[j-1].wt){
                ED x;
                x    = a[j];
                a[j] = a[i];
                a[i] = x;
            }
    return n;
}

static void GRfreeButST(GR);
static void GRfreeButST(GR g)
{
    int i;
    EDfreeR(g->head);
    for(i=0;i<g->nV;i++)
        free(g->adj[i]);
    free(g->adj);
    free(g);
}

ED    EDcreate(int v,int w,int wt)
{
    ED x; x.v = v; x.w = w; x.wt = wt;
    return x;
}

ED    EDvoid(void)
{
    return EDcreate(STD_NULL_NODE,STD_NULL_NODE,STD_NULL_WT);
}

GR    GRinit(int sz)
{
    int i,j;
    GR g   = malloc(sizeof(*g));
    g->adj = malloc(sz * sizeof(int*));
    for(i=0;i<sz;i++){
        g->adj[i] = malloc(sz * sizeof(int));
        for(j=0;j<sz;j++)
            g->adj[i][j] = 0;
    }
    g->nV = sz; g->nE = 0; g->head = NULL;
    return g;
}

int   GRsizeV(GR g)
{
    return g->nV;
}

void  GRinsert(GR g,ED e)
{
    g->adj[e.v][e.w] = g->adj[e.w][e.v] = e.wt;
    g->nE++; EDinsert(&g->head,e);
}

int   GRsizeE(GR g)
{
    return g->nE;
}

void  GRinsertST(GR g,ST s)
{
    g->tab = s;
}

int*  GRspBF(GR g,int start)
{
    //skipping the part of checking edges
    //because there aren't negative edges
    int i,k,j,*minDist;
    minDist = malloc(g->nV*sizeof(int));
    for(i=0;i<g->nV;i++)
        minDist[i] = MAX_WT;
    minDist[start] = 0;

    for(i=0;i<g->nV;i++)
        for(k=0;k<g->nV;k++)
            if(minDist[k] < MAX_WT)
                for(j=0;j<g->nV;j++)
                    if(g->adj[k][j] != 0)
                        if(minDist[j] > minDist[k] + g->adj[k][j])
                            minDist[j] = minDist[k] + g->adj[k][j];
    return minDist;
}

int*  GRdfsBcnt(GR g)
{
    int *pre,*post,*backs,i;
    pre   = malloc(g->nV* sizeof(int));
    post  = malloc(g->nV* sizeof(int));
    backs = malloc(g->nV* sizeof(int));
    for(i=0;i<g->nV;i++)
        if(pre[i] == -1)
            dfsBcnt(g,EDcreate(i,i,0),0,pre,post,backs);
    free(pre); free(post);
    return backs;
}

GR    GRdelete(GR g,int index)
{
    GR newG = GRinit(g->nV-1);
    link t;
    int i,j,k,l,EDcnt;

    for(i=0,k=0;i<g->nV;i++,k++){
        if(i==index) i++;
        for(j=0,l=0;j<g->nV;j++,l++){
            if(j==index) j++;
            newG->adj[k][l] = g->adj[i][j];
        }
    }
    EDcnt = g->nE;
    for(i=0,t = g->head;i < g->nE;t = t->next,i++)
        if((t->Edge.v != index)&&(t->Edge.w != index))
            EDinsert(&newG->head,t->Edge); else EDcnt--;

    newG->tab = g->tab;
    newG->nE  = EDcnt; newG->nV = g->nV-1;
    GRfreeButST(g);
    return newG;
}

int   GRacyclic(GR g)
{
    int *pre,flag = 1,i;
    pre  = malloc(g->nV* sizeof(int));
    for(i=0;i<g->nV;i++)
        if(pre[i] == -1)
            flag = dfsAcyclic(g,EDcreate(i,i,0),0,pre,0);
    free(pre);
    return flag;
}

int   GRconnected(GR g)
{
    int i,tmp;
    int *cc = GRcc(g);
    tmp = cc[0];
    for(i=0;i<g->nV;i++)
        if(tmp != cc[i]){
            free(cc);
            return 0;
        }
    free(cc);
    return 1;
}

int*  GRcc(GR g)
{
    int *cc,id = 0,i;
    cc = malloc(g->nV*sizeof(int));
    for(i=0;i<g->nV;i++) cc[i] = -1;
    for(i=0;i<g->nV;i++)
        if(cc[i]==-1)
            dfsRcc(g,i,id++,cc);
    return cc;
}

void  GRdfs(GR g)
{
    int *pre,*st,i;
    pre = malloc(g->nV* sizeof(int));
    st  = malloc(g->nV* sizeof(int));
    for(i=0;i<g->nV;i++)
        if(pre[i] == -1)
            dfs(g,EDcreate(i,i,0),0,pre,st);
    for(i=0;i<g->nV;i++)
        printf("%s's father is %s",STretrieve(g->tab,i),STretrieve(g->tab,st[i]));
    free(pre); free(st);
}

void GRkruskal(GR g,ED *a,int *cc)
{
    int i,k,n,weight = 0;
    n = EDarrSort(a);
    k = mstE(g,a,cc,n);

    for(i=0;i<k;i++)
        printf("add edge %s - %s",STretrieve(g->tab,a[i].v),STretrieve(g->tab,a[i].w));

}

void  GRfree(GR g)
{
    STfree(g->tab);
    GRfreeButST(g);
}