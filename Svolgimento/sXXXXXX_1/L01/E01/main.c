#include <stdio.h>
#include <stdlib.h>
#define MAX 20

int leggivett(int*,int);
int eliminadoppie(int*,int);
void sommavettori(int*,int*,int*,int,int);
void stampavettore(int*,int);
void ordinavettore(int*,int);

int main()
{
    int l1,l2,i,k,j,p,flag=0;
    int v1[MAX],v2[MAX];
    int U[2*MAX],D[2*MAX],I[MAX];
    char scelta,a;

    //la funzione ritorna i valori delle lunghezze
    //e rimuove anche gli elementi che si ripetono
    l1=leggivett(v1,1);
    l2=leggivett(v2,2);


    //scan tra vettori: le doppie le metto in I e il resto in D
    for(i=0,j=0,p=0;i<l1;i++){
        for(k=0,flag=0;(k<l2)&&(!flag);k++){
            if(v1[i]==v2[k]){
                flag=1;
                I[j]=v1[i];
                j++;
            }
        }
        if(!flag){D[p]=v1[i]; p++;}
    }

    //aggiungo gli elementi mancanti dal vettore 2 in D
    for(i=0;i<l2;i++){
        for(k=0,flag=0;(k<j)&&(!flag);k++){
            if(v2[i]==I[k]){
                flag=1;
            }
        }
        if(!flag){D[p]=v2[i]; p++;}
    }


    //non so precisamente per quale motivo ma sono costretto a mettere questo scanf
    //per metterci un \n che mi porto dietro dalle funzioni
    scanf("%c",&a);
    //ora che abbiamo l'insieme D e l'insieme I definiamo un menu
    //nel quale daremo all'utente la facoltà di scegliere cosa visualizzare
    while(1){
        printf("\nDigita:\n'U' se vuoi l'unione dei due vettori\n'I' se vuoi l'intersezione"
               "\n'D' se vuoi la differenza\n'T' per ridigitare i vettori\nE' per uscire\n\n\n\n");
        scanf("%c",&scelta);

        if(scelta=='U'){sommavettori(D,I,U,p,j); stampavettore(U,p+j);
        }else if(scelta=='D'){stampavettore(D,p);
        }else if(scelta=='I'){stampavettore(I,j);
        }else if(scelta=='T'){main();
        }else if(scelta=='E'){exit(0);
        }else {printf("\nDigitare un carattere nella lista per favore");}

        printf("\nDigita invio per continuare: ");
        scanf("%c",&a);
    }

    return 0;
}

int leggivett(int v[],int t){
    int i,l;

    //chiedo la lunghezza del vettore e richiamo
    //la funzione se un parametro non è rispettato
    printf("Quanto è lungo il vettore %d?\n",t);
    scanf("%d",&l);
    if(l>MAX||l<=0){printf("\nErrore input"); leggivett(v,t);}

    //inizio ad acquisire gli elementi
    for(i=0;i<l;i++){
        printf("\nVettore %d Elemento %d: ",t,i+1);
        scanf("%d",&v[i]);
    }

    //salvo in un altro vettore eliminando le doppie
    l=eliminadoppie(v,l);


    return l;
}

int eliminadoppie(int v[],int l){
    int i,k,j=0,flag;
    int u[l+1];

    for(i=0;i<l;i++){
        for(k=0,flag=0;(k<j)&&(!flag);k++){
            if(v[i]==u[k]){flag=1;}
        }
        if(!flag){u[j]=v[i]; j++;}
    }

    for(i=0;i<j;i++){
        v[i]=u[i];
    }
    return j;
}

void sommavettori(int v1[],int v2[],int dest[],int l1, int l2){
    int i;
    for(i=0;i<l1+l2;i++){
        if(i<l1){dest[i]=v1[i];
        }else{dest[i]=v2[i-l1];}
    }
    return;
}

void stampavettore(int v[],int l){
    int i;
    ordinavettore(v,l);
    printf("\n{");
    for(i=0;i<l;i++){

        if(i==l-1){printf("%d",v[i]);

        }else{printf("%d, ",v[i]);}
    }
    printf("}\n");
    return;
}

void ordinavettore(int v[],int l){
    int i,k,supp;
    for(i=0;i<l;i++){
        for(k=i+1;k<l;k++){
            if(v[i]>v[k]){
                supp=v[i];
                v[i]=v[k];
                v[k]=supp;
            }
        }
    }
    return;
}


