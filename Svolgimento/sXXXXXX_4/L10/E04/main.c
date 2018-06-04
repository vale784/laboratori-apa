#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"

int   ** MAPinit(int,int);
int      MAPread(FILE*,int**,int,int,int*,Item*);
void     graphFill(GR,int**,int,int);

int main(int argc,char *argv[]) {
    if(argc!=2)  return 1;
    FILE *fp = fopen(argv[1],"r");
    if(fp==NULL) return 2;
    int **map,nr,nc,nU,nV;
    Item I;
    GR g;

    fscanf(fp,"%d %d",&nr,&nc);
    map = MAPinit(nr,nc);
    nU  = MAPread(fp,map,nr,nc,&nV,&I);
    g   = GRinit(nV,nr,nc);
    graphFill(g,map,nr,nc);

    GRdfs_searchPath(g,I,'U',nU);

    GRfree(g);
    return 0;
}

int ** MAPinit(int r,int c)
{
    int i,**map;
    map = (int **) malloc(r * sizeof(int *));
    for(i=0;i<r;i++)
        map[i] = malloc(c * sizeof(int));
    return map;
}

int MAPread(FILE *fp,int **map,int r,int c,int *V,Item *I)
{
    int i,j,nU = 0;
    *V = 0;
    for(i=0;i<r;i++)
        for(j=0;j<c;j++){
            fscanf(fp,"%d",&map[i][j]);
            if(map[i][j]=='I') *I = ITEMcreate(i,j,'I');
            else if(map[i][j]=='U') nU++;
            if(map[i][j]!='X') (*V)++;
        }
    return nU;
}

void graphFill(GR g,int ** map,int nr,int nc)
{
    int i,j,tmp,supp;

    for(i=0;i<nr;i++)
        for(j=0;j<nc;j++)
            if((tmp=map[i][j])!='X'){
                if(((supp=map[i+1][j-1])!='X')&&(i+1 < nr)&&(j-1 > 0))
                    GRinsert(g,EDcreate(ITEMcreate(i,j,tmp),ITEMcreate(i+1,j-1,supp)));
                if(((supp=map[i+1][j])!='X')&&(i+1 < nr))
                    GRinsert(g,EDcreate(ITEMcreate(i,j,tmp),ITEMcreate(i+1,j,supp)));
                if(((supp=map[i+1][j+1])!='X')&&(i+1 < nr)&&(j+1 < nc))
                    GRinsert(g,EDcreate(ITEMcreate(i,j,tmp),ITEMcreate(i+1,j+1,supp)));
                if(((supp=map[i][j+1])!='X')&&(j+1 < nc))
                    GRinsert(g,EDcreate(ITEMcreate(i,j,tmp),ITEMcreate(i,j+1,supp)));
            }
}