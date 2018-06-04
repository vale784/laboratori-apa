#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include "Graph.h"

#define MAX 10

typedef struct point{
    int x,y;
}point;

typedef struct ST{
    point *tab;
    char **id;
    int n,maxN;
}ST;

ST st;

void    readGraph(GR*,int*,FILE*);
void    verifyFiles(FILE*,FILE*,GR);
void    editedKruskal(GR);
void    addEdges(GR,FILE*);

/*
 * da qui i prototipi non presenti
 * nel compito cartaceo
 */
float   getDist(point,point);
int     connected(int*);
float   getMin(float*);
void    sort(ED*,int,int);
int     STgetIndex(char*);
void    GRshow(GR);
void    GRedKruskal(GR,ED*,int*);


int main(int argc,char *argv[])
{
    if(argc!=4) return 1;
    FILE *fp_in,*fp_s1,*fp_s2;
    int N;
    GR *g;

    if((fp_in=fopen(argv[1],"r"))==NULL) return 2;
    readGraph(g,&N,fp_in);
    fclose(fp_in);

    if((fp_s1=fopen(argv[2],"r"))==NULL) return 3;
    if((fp_s2=fopen(argv[3],"r"))==NULL) return 4;

    verifyFiles(fp_s1,fp_s2,*g);
    fclose(fp_s1); fclose(fp_s2);

    editedKruskal(*g);

    GRfree(*g);
    return 0;
}//main

/*--------------------------------------------------------------------------------------------------------------------*/

void readGraph(GR *g,int *N,FILE *fp)
{
    int i;
    char buf[MAX+1];
    fscanf(fp,"%d",N);

    st.tab = malloc((*N)*sizeof(point));
    st.id  = malloc((*N)* sizeof(char*));
    st.n = 0; st.maxN = *N;
    *g = GRinit(*N);
    for(i=0;i<*N;i++){
        fscanf(fp,"%s %d %d",buf,&st.tab[i].x,&st.tab[i].y);
        st.id[i] = strdup(buf); st.n++;
    }
    addEdges(*g,fp);
}//readGraph

void verifyFiles(FILE *fp1,FILE *fp2,GR g)
{
    GR gcpy1,gcpy2;
    float *minDist1,*minDist2,min1 = FLT_MAX,min2 = FLT_MAX,supp;
    int i,*cc1 = malloc(st.maxN* sizeof(int)),*cc2 = malloc(st.maxN* sizeof(int));
    gcpy1 = GRinit(st.maxN); gcpy2 = GRinit(st.maxN);
    GRcpy(gcpy1,g);          GRcpy(gcpy2,g);
    addEdges(gcpy1,fp1);     addEdges(gcpy2,fp2);
    GRdfscc(gcpy1,cc1);      GRdfscc(gcpy2,cc2);
    if(connected(cc1)) {
        if (!connected(cc2))
            printf("sol 1 is the best\n");
        else {
            for (i = 0; i < st.maxN; i++) {
                minDist1 = GRminDists(gcpy1, i);
                minDist2 = GRminDists(gcpy2, i);
                if ((supp = getMin(minDist1)) < min1)
                    min1 = supp;
                if ((supp = getMin(minDist2)) < min2)
                    min2 = supp;
            }
            if(min1 < min2)
                printf("sol 1 is the best\n");
            else printf("sol 2 is the best\n");
        }
    }else if(connected(cc2))
        printf("sol 2 is the best\n");
    else printf("no sol\n");
}//verifyFiles

void editedKruskal(GR g)
{
    int *cc = malloc(st.maxN* sizeof(int)),i,j,k;
    ED *supp;
    GRdfscc(g,cc);
    supp = malloc(GRsize(g)*(GRsize(g)-1)* sizeof(ED));

    for(k=0,i=0;i<GRsize(g);i++)
        for(j=i+1;j<GRsize(g);j++)
            if(cc[i] != cc[j])
                supp[k++] = EDcreate(i,j,getDist(st.tab[i],st.tab[j]));

    sort(supp,0,k);
    printf("min diameter graph:\n");
    GRshow(g);
    printf("with these edges added:\n");
    GRedKruskal(g,supp,cc);
}//editedKruskal

void addEdges(GR g,FILE *fp)
{
    char buf1[MAX+1],buf2[MAX+1];
    int i,j;
    while(fscanf(fp,"%s %s",buf1,buf2)!=EOF){
        i = STgetIndex(buf1);
        j = STgetIndex(buf2);
        GRinsertED(g,EDcreate(i,j,getDist(st.tab[i],st.tab[j])));
    }
}//addEdges

/*--------------------------------------------------------------------------------------------------------------------*/

float getDist(point a,point b)
{
    return (float) (sqrt(pow(a.x-b.x,2)+pow(a.y-b.y,2)));
}//getDist

int connected(int *cc)
{
    int i,tmp = cc[0];
    for(i=1;i<st.maxN;i++)
        if(tmp != cc[i])
            return 0;
    return 1;
}//connected

float getMin(float *arr)
{
    float min=FLT_MAX;
    int i;
    for(i=0;i<st.maxN;i++)
        if(arr[i] < min)
            min = arr[i];
    return min;
}//getMin

void sort(ED *arr,int l,int r)
{
    int i,j; ED x;
    for(i=l;i<r;i++)
        for(j=r;j > i;j--)
            if(arr[j-1].val > arr[j].val){
                x = arr[j-1];
                arr[j-1] = arr[j];
                arr[j] = x;
            }
}//sort

int STgetIndex(char *s)
{
    int i;
    for(i=0;i<st.n;i++)
        if(strcmp(st.id[i],s)==0)
            return i;
    return -1;
}

void GRshow(GR g)
{
    int i,time,*pre,*post,*fath;
    pre  = malloc(GRsize(g)* sizeof(int));
    post = malloc(GRsize(g)* sizeof(int));
    fath = malloc(GRsize(g)* sizeof(int));

    for(i=0;i<GRsize(g);i++)
        pre[i] = post[i] = fath[i] = -1;

    for(i=0;i<GRsize(g);i++)
        if(pre[i] == -1)
            dfs(g,EDcreate(i,i,0.0),&time,pre,post,fath);

    for(i=0;i<GRsize(g);i++)
        printf("%s's father is %s\n",st.id[i],st.id[fath[i]]);
}//GRshow

void GRedKruskal(GR g,ED *supp,int *cc)
{
    int i,k;
    ED *mst = malloc((GRsize(g)-1)*sizeof(ED));

    k = mstE(g,mst,supp,cc);

    for(i=0;i<k;i++){
        printf("Edge: %s - %s\n",st.id[mst[i].id1],st.id[mst[i].id2]);
    }
}//GRedKruskal