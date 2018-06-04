#include <stdio.h>
#include <stdlib.h>
#define MAX 30

int mediaMobile(int V[],int l, int k, float m[]);

int main()
{
    int v[MAX],k,i,n,l;
    float M[MAX];

    //lunghezza del vettore
    printf("Quanto è lungo il vettore? [MAX=%d]\n",MAX);

    scanf("%d",&n);
    if(n<0||n>MAX){printf("\nInserire un valore accettabile\n"); main();}


    //lettura del vettore
    for(i=0;i<n;i++){
        printf("Inserire Elemento %d: ",i+1);
        scanf("%d",&v[i]);
    }

    //acquisizione di k
    printf("\nScegliere l'ampiezza: ");
    scanf("%d",&k);

    l=mediaMobile(v,n,k,M);

    //stampa dei risultati
    printf("\nVettore iniziale di lunghezza %d: V={",n);
    for(i=0;i<n;i++){
        if(i==n-1){printf("%d}",v[i]);
        }else{printf("%d, ",v[i]);}
    }
    printf("\nVettore medie di lunghezza %d con ampiezza %d: M={",l,k);
    for(i=0;i<l;i++){
        if(i==l-1){printf("%f.3}",M[i]);
        }else{printf("%.3f, ",M[i]);}
    }
    printf("\n");


    return 0;
}


int mediaMobile(int V[MAX],int l, int k, float m[MAX]){
    int i,j,lm;
    float supp;

    lm=(l-(l%k));

    for(i=0;i<lm;i++){
        supp=0.0;
        for(j=i;j<i+k;j++){
            supp+=V[j];
        }
        m[i]=(supp/k);
    }

    return lm;
}
