#include <stdio.h>
#define ERRORE -1
#define NMAX 10000

void inizParz(int *v,int max);
void input(const char *v,int* n);
void output(int* parz,int l, int r);
int hofstader(int n,int parz[],int max);

int main(void)
{
    int N,parz[NMAX];
    inizParz(parz,NMAX);
    input("Inserire il numero N di elementi desiderati: ",&N);
    if(N>NMAX) return ERRORE;
    hofstader(N,parz,NMAX);
    output(parz,0,N);
    return 0;
}

void input(const char *v,int* n)
{
    fprintf(stdout,v);
    fscanf(stdin,"%d",n);
}

void inizParz(int *v,int max)
{
    int i;
    for(i=0;i<max;i++) v[i]=-1;
    v[0]=0; //per definizione di hofstader
}

void output(int* parz,int l, int r)
{
    int i;
    for(i=l;i<r;i++) printf("%d ",parz[i]);
}

int hofstader(int n,int parz[],int max)
{
    if(parz[n]!=-1) return parz[n];
    parz[n]=n-hofstader(hofstader(hofstader(n-1,parz,max),parz,max),parz,max);
    return parz[n];
}

