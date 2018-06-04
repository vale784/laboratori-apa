#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define fName "nomeArbitrario.txt"
#define Max_Card 10

//ERRORI VARI
#define ERR_FNAME (-1)

typedef struct livelloS{int *scelte;
    int num_scelte;} Livello;

int leggiSet(FILE *fp,Livello *set,int n);
int princ_molt(int k, Livello *set, int *sol,int n,int cont);

int main() {
    int *sol;
    Livello *set;
    int n,lsol;
    FILE *fp;

    if((fp=fopen(fName,"r"))==NULL) return ERR_FNAME;
    fscanf(fp,"%d",&n);
    set = malloc(n*sizeof(Livello));
    lsol=leggiSet(fp,set,n);
    fclose(fp);

    sol = malloc(lsol*sizeof(char));

    princ_molt(0,set,sol,lsol,0);


    return 0;
}

int leggiSet(FILE *fp,Livello *set,int n)
{
    int i,k,l,tot=1;
    char buf[Max_Card+1];

    for(i=0;i<n;i++){
        fscanf(fp,"%s",buf);
        l=strlen(buf);
        set[i].scelte = malloc(l*sizeof(int));
        for(k=0;k<l;k++) set->scelte[k] = buf[k];
        set->num_scelte=l;
        tot*=l;
    }
    return tot;
}

int princ_molt(int k, Livello *set, int *sol,int n,int cont)
{
    int i;
    if(k >= n){
        for(i=0;i<n;i++)
            printf("%c",(char) sol[i]);
        printf("\n");
        return cont+1;
    }
    for(i=0;i<set[k].num_scelte;i++){
        sol[k]=set[k].scelte[i];
        cont = princ_molt(k+1,set,sol,n,cont);
    }
    return cont;
}