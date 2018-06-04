#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SymbolTable.h"
#define MAX 12

typedef struct{
    int *a,*r;
}solution;

int**   MATinit(int);
void    readFiles(FILE*,FILE*,ST,ST,int**,int**,int);
int     verify(FILE*,ST,ST,int**,int**,int);
void    simDisp(ST,ST,int**,int**,int);
void    simDispR(ST,ST,int**,int**,int,int,solution,int*,int);
int     testSolution(int**,int**,int,solution);
void    printSol(ST,ST,solution,int);

int main(int argc,char *argv[]) {
    if(argc!=5) return argc;
    FILE *fA,*fR,*fT;
    ST stA,stR;
    int **A, **R, N = atoi(argv[1]);

    stA = STinit(N); stR = STinit(N);
    A = MATinit(N); R = MATinit(N);

    fA = fopen(argv[2],"r");
    fR = fopen(argv[3],"r");
    if((fA==NULL)||(fR==NULL))
        return 2;

    readFiles(fA,fR,stA,stR,A,R,N);

    fT = fopen(argv[4],"r");
    if(fT==NULL) return 3;

    if(verify(fT,stA,stR,A,R,N))
        printf("Correct!\n"); else printf("Not Correct!\n");

    simDisp(stA,stR,A,R,N);

    return 0;
}

int **MATinit(int N)
{
    int i,**mat;
    mat = malloc(N*sizeof(int*));
    for(i=0;i<N;i++)
        mat[i] = malloc(N*sizeof(int));
    return mat;
}

void readFiles(FILE *fA,FILE *fR,ST stA,ST stR,int **A,int **R,int N)
{
    int i,j,k;
    char buf[MAX];
    for(i=0;i<N;i++){
        fscanf(fA,"%s",buf);
        STinsert(stA,strdup(buf));
        fscanf(fR,"%s",buf);
        STinsert(stR,strdup(buf));
        for(k=0;k<N;k++){
            fscanf(fA,"%*s");
            fscanf(fR,"%*s");
        }
    }

    rewind(fA); rewind(fR);

    for(i=0;i<N;i++){
        fscanf(fA,"%s",buf);
        k = STgetIndex(stA,buf);
        for(j=0;j<N;j++){
            fscanf(fA,"%s",buf);
            A[k][j] = STgetIndex(stR,buf);
        }
    }
    for(i=0;i<N;i++){
        fscanf(fR,"%s",buf);
        k = STgetIndex(stR,buf);
        for(j=0;j<N;j++){
            fscanf(fR,"%s",buf);
            R[k][j] = STgetIndex(stA,buf);
        }
    }
}

int verify(FILE *fT,ST stA,ST stR,int **A,int **R,int N)
{
    int i,tmp1,tmp2;
    char buf1[MAX],buf2[MAX];
    for(i=0;i<N;i++){
        fscanf(fT,"%s %s",buf1,buf2);
        tmp1 = STgetIndex(stA,buf1);
        tmp2 = STgetIndex(stR,buf2);
        if((A[tmp1][0]!=tmp2)&&(R[tmp2][0]!=tmp1))
            return 0;
    }
    return 1;
}

void simDisp(ST stA,ST stR,int **A,int **R,int N)
{
    int i,*mark;
    solution sol;
    sol.a = malloc(N*sizeof(int));
    sol.r = malloc(N*sizeof(int));
    mark  = malloc(N*sizeof(int));

    for(i=0;i<N;i++){
        sol.a[i] = i;
        mark[i]  = 0;
    }

    simDispR(stA,stR,A,R,N,0,sol,mark,0);
}

void simDispR(ST stA,ST stR,int **A,int **R,int N,int pos,solution sol,int *mark,int flag)
{
    if(flag==1) return;
    int i;
    if(pos>=N)
        if(testSolution(A,R,N,sol)){
            printSol(stA,stR,sol,N);
            flag = 1;
        } else return;

    for(i=0;i<N;i++)
        if(mark[i]==0){
            mark[i] = 1;
            sol.r[pos] = i;
            simDispR(stA,stR,A,R,N,pos+1,sol,mark,flag);
            if(flag==1) return;
            mark[i] = 0;
        }
}

int testSolution(int **A,int **R,int N,solution sol)
{
    int i,tmp1,tmp2;
    for(i=0;i<N;i++){
        tmp1 = sol.a[i];
        tmp2 = sol.r[i];
        if((A[tmp1][0]!=tmp2)&&(R[tmp2][0]!=tmp1))
            return 0;
    }
    return 1;
}

void printSol(ST stA,ST stR,solution sol,int N)
{
    int i;
    for(i=0;i<N;i++){
        printf("%s - %s\n",STretrieve(stA,sol.a[i]),STretrieve(stR,sol.r[i]));
    }
}