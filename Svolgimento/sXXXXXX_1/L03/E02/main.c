#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define fname "voli.txt"
#define MAXC 30
#define MAXN 1000

typedef struct{
    char codice_volo[MAXC+1],partenza[MAXC+1],destinazione[MAXC+1],data_e_ora[MAXC+1],vettore[MAXC+1];
}s_tabella;

typedef enum{
r_date, r_origine, r_destinazione, r_vettore, r_fine, r_err
}comando_e;

int leggiTabella(s_tabella tabella[MAXN],FILE *fp);
comando_e leggiComando(char supp[MAXC+1]);
void selezionaDati(comando_e comando, s_tabella tabella[MAXN], int nDati, char istruzione[MAXC+1]);
void cercaDate(s_tabella tabella[MAXN], char istruzione[MAXC+1], int nDati);
void cercaPartiti(s_tabella tabella[MAXN], char istruzione[MAXC+1], int nDati);
void cercaDiretti(s_tabella tabella[MAXN], char istruzione[MAXC+1], int nDati);
void cercaVett(s_tabella tabella[MAXN], char istruzione[MAXC+1], int nDati);
int isrepeated(char elenco[MAXN][MAXC+1],char elemento[MAXC+1],int nDati);


int main(){

    s_tabella tabella[MAXN];
    FILE *fp;
    comando_e comando;
    char istruzione[MAXC+1];
    int nDati;

    if((fp=fopen(fname,"r"))==NULL){return -1;}
    nDati=leggiTabella(tabella,fp);
    fclose(fp);
    if(nDati==-1){return -2;}

    if((comando=leggiComando(istruzione))==r_err){return -3;}

    selezionaDati(comando,tabella,nDati,istruzione);

    return 0;
}


int leggiTabella(s_tabella tab[MAXN],FILE *fp){

    int nDati,i;

    fscanf(fp,"%d",&nDati);
    if((nDati<0||nDati>MAXN)){return -1;}

    for(i=0;i<nDati;i++)
        fscanf(fp,"%s %s %s %s %s",tab[i].codice_volo,tab[i].partenza,tab[i].destinazione,tab[i].data_e_ora,tab[i].vettore);

    return nDati;
}

comando_e leggiComando(char supp[MAXC+1]){

    int i,flag=0;
    char scelte[r_err][MAXC]={"date","origine","destinazione","vettore","fine"};


    printf("Cosa si vuol fare?\ncerca tra 2 date: digita 'date <data1> <data2>'\ncerca da origine: digita 'origine <luogo>'\n"
           "cerca da destinazione: digita 'destinazione <luogo>'\ncerca da vettore: digita 'vettore <vettore>'\ntermina programma: digita 'fine'\n");

    while(1){
        fgets(supp,MAXC,stdin);
        for(i=0;(i<r_err)&&(!flag);i++){
            if(strncmp(supp,scelte[i],strlen(scelte[i]))==0){
                return i;
            }
        }
        printf("\ndigitare una parola accettabile\n");
    }

    return r_err;
}

void selezionaDati(comando_e comando, s_tabella tabella[MAXN], int nDati, char istruzione[MAXC+1]){

    switch(comando){
    case r_date: cercaDate(tabella,istruzione,nDati); break;
    case r_origine: cercaPartiti(tabella,istruzione,nDati); break;
    case r_destinazione: cercaDiretti(tabella,istruzione,nDati); break;
    case r_vettore: cercaVett(tabella,istruzione,nDati); break;
    case r_fine: case r_err: break;
    }
    return;
}

void cercaDate(s_tabella tabella[MAXN], char istruzione[MAXC+1], int nDati){

    char data1[MAXC+1],data2[MAXC+1];
    int i;

    sscanf(&istruzione[strlen("date")],"%s %s",data1,data2);
    printf("voli da %s a %s:",data1,data2);

    for(i=0;i<nDati;i++){
        if(((strncmp(data1,tabella[i].data_e_ora,strlen(data1)))<=0)&&((strncmp(data2,tabella[i].data_e_ora,strlen(data2)))>=0)){
            printf("\n%s",tabella[i].codice_volo);
        }
    }
    return;
}

void cercaPartiti(s_tabella tabella[MAXN], char istruzione[MAXC+1], int nDati){

    char origine[MAXC+1];
    int i;

    sscanf(&istruzione[strlen("origine")],"%s",origine);
    printf("Partiti da %s:",origine);

    for(i=0;i<nDati;i++){
        if(strcmp(tabella[i].partenza,origine)==0){
            printf("\n%s",tabella[i].codice_volo);
        }
    }
    return;
}

void cercaDiretti(s_tabella tabella[MAXN], char istruzione[MAXC+1], int nDati){

    char destinazione[MAXC+1];
    int i;

    sscanf(&istruzione[strlen("destinazione")],"%s",destinazione);
    printf("Diretti a %s:",destinazione);

    for(i=0;i<nDati;i++){
        if(strcmp(tabella[i].destinazione,destinazione)==0){
            printf("\n%s",tabella[i].codice_volo);
        }
    }
    return;
}

void cercaVett(s_tabella tabella[MAXN], char istruzione[MAXC+1], int nDati){

    char vettore[MAXC+1];
    char acquisiti[nDati][MAXC+1];
    int i;

    sscanf(&istruzione[strlen("vettore")],"%s",vettore);
    printf("con questo vettore %s sono partiti da:",vettore);

    for(i=0;i<nDati;i++){
        if(strcmp(tabella[i].vettore,vettore)==0){
            if(!isrepeated(acquisiti,tabella[i].partenza,nDati)){
                printf("\n%s",tabella[i].partenza);
            }
        }
    }
    return;
}

int isrepeated(char elenco[MAXN][MAXC+1],char elemento[MAXC+1],int nDati){

    int i;

    for(i=0;i<nDati;i++){
        if(strcmp(elenco[i],elemento)==0){
            return 1;
        }
    }
    return 0;
}

