#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define in_fileName "frecce.txt"

/*--------------------------------------------------------------------------------------------------------------------*/

//init functions
int *acquireArrows(const char *path,int *N);
int *findSolutionA(int *arrows, int N);
int *findSolutionB(int *arrows, int N);
int *init_sol(int N);

//support functions
void incpy(int *s1,const int *s2,int l,int N);
void printSol(const int *sol,int N);

//main functions
int  powerSet(int pos,int *arrows,int *sol,int *bSol,int N);
int  dynamicSol(int pos, int *arrows, int *sol, int *bSol, int N);

bool goodSol(int *arr,const int *sol,int N);
bool bestSol(const int *sol1,const int *sol2,int N);
int turnArrows(int l,int r,const int *arrows,int *sol);

//standard keys (binary)
const int DX = 0,     SX = 1;
const int noTurn = 0, toTurn = 1;
#define turn(A) (((A)==DX)? ((A)=SX):((A)=DX))

/*--------------------------------------------------------------------------------------------------------------------*/

int main(void) {

    int *arrows,N,*bestSolution;

    arrows = acquireArrows(in_fileName,&N);
    if(arrows==NULL) {printf("No file found\n"); return 0;}

    bestSolution = findSolutionA(arrows, N);

    printSol(bestSolution,N);

    bestSolution = findSolutionB(arrows, N);

    printSol(bestSolution,N);

    return 0;
}

/*--------------------------------------------------------------------------------------------------------------------*/

int *acquireArrows(const char *path,int *N)
{
    FILE *fp=fopen(path,"r");
    int *toReturn,i;

    if(fp==NULL) return NULL;

    fscanf(fp,"%d",N);
    toReturn = malloc((*N)*sizeof(*toReturn));

    for(i=0;(i<*N)&&(fscanf(fp,"%d",&toReturn[i])!=EOF);i++);

    return toReturn;
}


/* this function is a wrapper, i will use the
 * power set to find every configuration saving
 * the best one by reference, sol will be an array
 * of just 1 and 0s, index is the relation to the
 * arrow to turn (0 = no turn, 1 = turn) in arrows
 */
int *findSolutionA(int *arrows, int N)
{
    int *sol = init_sol(N),*bestSol = init_sol(N);
    int nSol;
    nSol = powerSet(0,arrows,sol,bestSol,N);
    free(sol);

    printf("%d solutions were analyzed with PS, the best is\n",nSol);
    return bestSol;
}

int *init_sol(int N)
{
    int i,*sol;
    sol=malloc(N* sizeof(*sol));

    for(i=0;i<N;i++)
        sol[i] = toTurn;

    return sol;
}
/*--------------------------------------------------------------------------------------------------------------------*/

void incpy(int *s1,const int *s2,int l,int N)
{
    int i;
    for(i=l;i<N;i++) s1[i]=s2[i];
}

void printSol(const int *sol,int N)
{
    int i;

    for(i=0;i<N;i++)
        if(sol[i]==toTurn)
            printf("turn arrow n%d\n",i+1);

}
/*--------------------------------------------------------------------------------------------------------------------*/

int powerSet(int pos,int *arrows,int *sol,int *bSol,int N)
{
    int cnt=0;

    if(pos>=N){
        if(goodSol(arrows,sol,N)&&(bestSol(sol,bSol,N)))
            incpy(bSol,sol,0,N); //copy sol in bSol
        return 1;
    }

    sol[pos] = noTurn;
    cnt     += powerSet(pos+1,arrows,sol,bSol,N);
    sol[pos] = toTurn;
    cnt     += powerSet(pos+1,arrows,sol,bSol,N);
    return cnt;
}

bool goodSol(int *arr,const int *sol,int N)
{
    int *supp = init_sol(N);
    int i,tmp1=0,tmp2=0;

    incpy(supp,arr,0,N);

    for(i=0;i<N;i++)
        if(sol[i]==toTurn) turn(supp[i]);

    for(i=0;i<N;i++){
        if(supp[i]==DX) {
            while((supp[++i]==DX) && (i<N)) tmp1++;
            while((supp[++i]==SX) && (i<N)) tmp2++;

        }else{ return false; }

        if(tmp1!=tmp2) return false;

        tmp1=0; tmp2=0; i--;
    }
    return true;
}

bool bestSol(const int *sol1,const int *sol2,int N)
{
    int i,cnt1,cnt2;

    for(i=0,cnt1=0,cnt2=0;i<N;i++){
        if(sol1[i]==toTurn) cnt1++;
        if(sol2[i]==toTurn) cnt2++;
    }
    return (cnt1<cnt2);
}
/*--------------------------------------------------------------------------------------------------------------------*/

int *findSolutionB(int *arrows, int N)
{
    int *sol = init_sol(N),*bestSol = init_sol(N);
    int nSol;
    nSol = 0;
    free(sol);

    nSol = dynamicSol(0, arrows, sol, bestSol, N);

    printf("%d solutions were analyzed with DP, the best is\n",nSol);
    return bestSol;
}

int dynamicSol(int l, int *arrows, int *sol, int *bSol, int N)
{
    int cnt=0, i, k, min=0, nTurns = N+1 ,supp;

    for(i=l;i<=N;i=min){
        for(k=i+2;k<=N;k+=2){
            if((supp=turnArrows(i,k,arrows,sol))<nTurns) {
                min = k; nTurns = supp;
                incpy(bSol,sol,i,k);
            }
        }
        cnt++; nTurns = N+1;
    }

    return cnt;
}

int turnArrows(int l,int r,const int *arrows,int *sol)
{
    int i,cnt,half = r/2;

    for(cnt=0,i=l;i<r;i++)
        if(i<=half) {
            if (arrows[i] == SX)
                {sol[i] = toTurn; cnt++;}  else sol[i] = noTurn;
        }else if(arrows[i]==DX)
            {sol[i] = toTurn; cnt++;} else sol[i] = noTurn;
    return cnt;
}