#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Graph.h"

#define MAXL 30

void    fillST(FILE*,GR,ST,int);
void    fillGR(FILE*,ST,GR);
int**   bellmanFord(GR);
void    findAcyclic(GR,int**);
int     searchMaxIndex(const int*,int,int);
ED*     getEdges(const int*,int**,int,int);

int main(int argc,char *argv[])
{
    if(argc!=2)  return 1;
    FILE *fp = fopen(argv[1],"r");
    if(fp==NULL) return 2;
    int nV,**minDist;
    fscanf(fp,"%d",&nV);

    GR g = GRinit(nV);
    ST s = STinit(nV);

    fillST(fp,g,s,nV);
    fillGR(fp,s,g);

    fclose(fp);

    /*matrix of distances*/
    minDist   = bellmanFord(g);

    /*
     * Greedy solution that deletes nodes from
     * artPoints starting from the one who has more
     * 'Back' edges tested every time with a dfs
     */
    findAcyclic(g,minDist);

    GRfree(g);
    return 0;
}

void fillST(FILE *fp,GR g,ST s,int nV)
{
    int i;
    char buf[MAXL];

    for(i=0;i<nV;i++){
        fscanf(fp,"%s",buf);
        STinsert(s,strdup(buf));
    }
    GRinsertST(g,s);
}

void fillGR(FILE *fp,ST s,GR g)
{
    char buf1[MAXL],buf2[MAXL];
    int  i,j,wt;

    while((fscanf(fp,"%s %s %d",buf1,buf2,&wt))!=EOF){
        i = STsearch(s,buf1);
        j = STsearch(s,buf2);
        GRinsert(g,EDcreate(i,j,wt));
    }
}

int** bellmanFord(GR g)
{
    int i,**minDist;
    minDist = malloc(GRsizeV(g)*sizeof(int*));

    for(i=0;i < GRsizeV(g);i++)
        minDist[i] = GRspBF(g,i);

    return minDist;
}

void findAcyclic(GR g,int **minDist)
{
    int i,*cntBack,*connComp;
    ED *a;
    do{
        cntBack = GRdfsBcnt(g);
        i       = searchMaxIndex(cntBack,0,GRsizeV(g));
        g       = GRdelete(g,i);
    }while(!GRacyclic(g));

    if(!GRconnected(g)){
        connComp = GRcc(g);
        a = getEdges(connComp,minDist,GRsizeE(g),GRsizeV(g));
        GRdfs(g);
        GRkruskal(g,a,connComp);

    }else GRdfs(g);
}

int searchMaxIndex(const int *arr,int l,int r)
{
    int i,max = -1,max_i = -1;

    for(i=l;i<r;i++)
        if(max < arr[i]){
            max   = arr[i];
            max_i = i;
        }
    return max_i;
}

ED * getEdges(const int *cc,int **mindist,int nE,int nV)
{
    ED *a;
    int i,j,k;

    a = malloc(nE*sizeof(ED));
    for(i=0;i<nE;i++) a[i] = EDvoid();

    for(i=0,k=0;i<nV;i++)
        for(j=i;j<nV;j++)
            if(cc[i] != cc[j])
                a[k++] = EDcreate(i,j,mindist[i][j]);

    return a;
}