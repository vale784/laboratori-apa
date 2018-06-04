#include <stdio.h>
#include <stdlib.h>
#include <mem.h>
#include "LISTathlete.h"
#include "STexercises.h"

#define MAX_EXERCISES_PER_ATHLETE 8
#define DEFAULT_BONUS ((float) 2.5)

#define bonus(flag) ((float)(((flag)==true)? (DEFAULT_BONUS):(0.0)))


void printSol(int,ST,int*,int);
void completeSol(AW,ST,int);
void completeSolR(int,int,int,int*,int*,int*,int,int,ST,float,float);
void printSol(int,ST,int*,int);
bool goodSol(ST,int*,int,bool*,bool);
bool multipleCate(int,ST,int*);
bool lastDiff(ST,int);
bool repetition(ST,int*,int);
void greedySol(AW,ST,int);
int findMax(ST,int*,int*,int,int,int,int,bool*);


int main(int argc,char *argv[])
{
    if(argc!=3) return 1;
    FILE *inAth,*inEx;
    AW at_w;
    ST st;


    inAth = fopen(argv[1],"r");
    if(inAth==NULL)   return 2;

    inEx =  fopen(argv[2],"r");
    if(inEx==NULL)    return 3;

    at_w = AWbuild(inAth);
    st   = STbuild(inEx,DEFAULT_M);

    printf("This is the Greedy Solution:\n\n");
    greedySol(at_w,st,MAX_EXERCISES_PER_ATHLETE);

    printf("This is the Complete Solution:\n\n");
    completeSol(at_w,st,MAX_EXERCISES_PER_ATHLETE);

    return 0;
}

void completeSolR(int pos,int maxEx,int n,int *sol,int *bSol,
                int *compEx,int tmp_diff,int maxTotDiff,ST ex,float tmp_score,float max_score)
{
    if(tmp_diff > maxTotDiff) return;
    int i;
    bool flag = false;

    if(pos>=maxEx){
        if(goodSol(ex,sol,pos,&flag,false)&&(tmp_score + bonus(flag) < max_score))
            memcpy(bSol,sol,pos*sizeof(int));
        return;
    }

    for(i=0;i<n;i++){
        sol[pos] = compEx[i];
        completeSolR(pos+1,maxEx,n,sol,bSol,compEx,tmp_diff + getDiff(getExercise(ex,compEx[i])),maxTotDiff,
                   ex,tmp_score + (float)((repetition(ex,sol,pos))? 0.0:STgetScore(ex,compEx[i])),max_score);
    }
}

void completeSol(AW aw,ST ex,int max_ex_per_athlete)
{
    ATlink x = getHead(aw);
    AT tmp;
    int tmp_na,tmp_aa,tmp_ai,tmp_max,index = 0;
    int *sol,*bSol,*compEx,n;

    sol  = calloc((size_t ) max_ex_per_athlete, sizeof(int));
    bSol = calloc((size_t ) max_ex_per_athlete, sizeof(int));

    while(x!=NULL){
        tmp = getAthlete(x);
        tmp_na = getNA(tmp); tmp_aa  = getAA(tmp);
        tmp_ai = getAI(tmp); tmp_max = getSum(tmp);
        compEx = getCompatibleEX(ex,tmp_na,tmp_aa,tmp_ai,&n);

        completeSolR(0,MAX_EXERCISES_PER_ATHLETE,n,sol,bSol,compEx,0,tmp_max,ex,0.0,0.0);
        printSol(index,ex,bSol,max_ex_per_athlete);

        index++; x = getNext(x);
    }
}

void printSol(int index,ST st,int* bSol,int n)
{
    int i;
    EX tmp;

    printf("Athlete %d:\n",index+1);

    for(i=0;i<n;i++){
        tmp = getExercise(st,bSol[i]);
        EXdisplay(tmp);
    }
}

bool goodSol(ST ex,int *sol,int N,bool *bonus,bool flag)
{
    int i,j;
    int *tmp = malloc(N* sizeof(int));
    memcpy(tmp,sol,N*sizeof(int));
    *bonus = false;

    /*
     * an exercise repeated more than once
     * does not give points (so -1)
     */
    for(i=0;i<N;i++)
        for(j=i;j<N;j++)
            if(tmp[i]==tmp[j]){
                tmp[i]=-1;
                break; //or continue??
            }

    /* check for different categories */
    flag = multipleCate(N,ex,tmp);

    /* not admitted both multi-category and difficult ending*/
    if(flag && lastDiff(ex,tmp[N-1]))
        return false; else *bonus = true;
}

bool multipleCate(int N,ST ex,int *tmp)
{
    int i;

    for(i=0;i<N;i++)
        if(tmp[i]!=-1)
            if(getCate(getExercise(ex,tmp[i])) != getCate(getExercise(ex,tmp[i-1])))
                return true;
    return false;
}

bool lastDiff(ST st,int i)
{
    if(getDiff(getExercise(st,i))>=5)
        return true;
    return false;
}

bool repetition(ST ex,int *sol,int pos)
{
    int i;
    for(i=0;i<pos;i++)
        if(getCate(getExercise(ex,sol[i]))==getCate(getExercise(ex,sol[pos])))
            return true;
    return false;
}

void greedySol(AW aw,ST ex,int max_ex_per_athlete)
{
    ATlink x = getHead(aw);
    AT tmp;
    int tmp_na,tmp_aa,tmp_ai,tmp_max,index = 0;
    int *sol,*bSol,*compEx,n,i,tmp_diff=0;
    bool bonus;

    sol  = calloc((size_t ) max_ex_per_athlete, sizeof(int));

    while(x!=NULL){
        tmp = getAthlete(x);
        tmp_na = getNA(tmp); tmp_aa  = getAA(tmp);
        tmp_ai = getAI(tmp); tmp_max = getSum(tmp);
        compEx = getCompatibleEX(ex,tmp_na,tmp_aa,tmp_ai,&n);

        for(i=0;i<max_ex_per_athlete;tmp_diff=0,i++) {
            tmp_diff += getDiff(getExercise(ex,compEx[i]));
            sol[i]    = findMax(ex, compEx, sol, n, i,tmp_diff,tmp_max,&bonus);
        }

        printSol(index,ex,sol,max_ex_per_athlete);

        index++; x = getNext(x);
    }
}

int findMax(ST st,int *comEx,int *sol,int n,int diff,int max_diff,int maxex,bool *bonus)
{
    int i,j,i_max = 0;
    bool flag;
    float max = getScore(getExercise(st,comEx[0])),tmp;

    for(i=1;i<n;i++){
        if((tmp=getScore(getExercise(st,comEx[i]))) > max){
            flag=true;
            for(j=0;j<maxex;j++)
                if(sol[j]==comEx[i])
                    flag = *bonus = false;
            if(flag && ((diff + getDiff(getExercise(st,comEx[i]))) < max_diff)) {
                i_max = i;
                max = tmp;
            }
        }
    }
    return i_max;
}

