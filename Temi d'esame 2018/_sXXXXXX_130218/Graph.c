#include "Graph.h"
#include <stdlib.h>
#include <float.h>
#include <stdio.h>

struct Graph{
    float **mat;
    int size;
};

GR GRinit(int N)
{
    int i,j;
    GR g = malloc(N* sizeof(*g));
    g->size = N;
    g->mat  = malloc(N* sizeof(float *));
    for(i=0;i<N;i++)
        g->mat[i] = malloc(N* sizeof(float));

    for(i=0;i<N;i++)
        for(j=0;j<N;j++)
            g->mat[i][j] = 0.0;

    return g;
}//GRinit

void GRcpy(GR g1,GR g2)
{
    int i,j;
    g1->size = g2->size;
    for(i=0;i<g1->size;i++)
        for(j=0;j<g1->size;j++)
            g1->mat[i][j] = g2->mat[i][j];
}//GRcpy

static void dfscc(GR,int,int,int*);
static void dfscc(GR g,int i,int id,int *cc)
{
    int j;
    cc[i] = id;
    for(j=0;j<g->size;j++)
        if(g->mat[i][j] != 0.0)
            if(cc[j] == -1)
                dfscc(g,j,id,cc);
}//dfscc

void GRdfscc(GR g,int *cc)
{
    int i,id=0;
    for(i=0;i<g->size;i++)
        cc[i] = -1;

    for(i=0;i<g->size;i++)
        if(cc[i] == -1)
            dfscc(g,i,id++,cc);
}//GRdfscc

float * GRminDists(GR g,int start)
{
    //algoritmo di Bellman-Ford rivisitato
    int i,k,t;
    float *minDist;
    minDist = malloc(g->size*sizeof(float));

    for(i=0;i<g->size;i++) minDist[i] = FLT_MAX;
    minDist[start] = 0.0;

    for(i=0;i<g->size;i++)
        for(k=0;k<g->size;k++)
            if(minDist[k] < FLT_MAX)
                for(t=0;t<g->size;t++)
                    if(g->mat[k][t] != 0.0)
                        if(g->mat[k][t] < minDist[k] + g->mat[k][t])
                            minDist[t] = minDist[k] + g->mat[k][t];

    //distanze euclediane = impossibile il peso negativo
    return minDist;
}//GRminDists

void GRinsertED(GR g,ED e)
{
    g->mat[e.id1][e.id2] = g->mat[e.id2][e.id1] = e.val;
}//GRinsertED

int GRsize(GR g)
{
    return g->size;
}//GRsize

void dfs(GR g,ED e,int* time,int* pre,int* post,int *fath)
{
    int t;
    fath[e.id2] = e.id1;
    pre[e.id2]  = (*time)++;

    for(t=0;t<g->size;t++)
        if(g->mat[e.id2][t] != 0.0)
            if(pre[t] == -1)
                dfs(g,EDcreate(e.id2,t,0.0),time,pre,post,fath);

    post[e.id2] = (*time)++;
}//dfs

int mstE(GR g,ED* mst,ED* supp,int* cc)
 {
    int i,k,j,x,y,all_connected,maxE=g->size*(g->size-1);

    for(i=0,k=0;(k < g->size-1)&&(i<maxE); i++)
        if((x = cc[supp[i].id1]) != (y = cc[supp[i].id2])){
            mst[k++] = supp[i];
            for(j=0;j<g->size;j++)
                if(cc[j] == x) cc[j] = y;

            for(all_connected=1,j=1;j<g->size;j++)
                if(cc[j] != cc[j-1])
                    all_connected = 0;
            if(all_connected) return k;
        }
    return k;
}//mstE

void GRfree(GR g)
{
    int i;
    for(i=0;i<g->size;i++)
        free(g->mat[i]);
    free(g->mat); free(g);
}//GRfree

ED EDcreate(int a,int b,float val)
{
    ED e;
    e.id1 = a; e.id2 = b;
    e.val = val;
    return e;
}//EDcreate