#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_S 30
#define MAX_L 200

int main()
{
    FILE *src,*sost,*dest;
    char nuovo[MAX_S][MAX_L+1],supp[MAX_L+2],parola[MAX_L+2];
    //n è il numero di righe in sost.txt
    int S[MAX_S],n,i;
    int inizio,fine;
    int flag=0,trovato=0;

    //apriamo tutti i file
    if((src=fopen("src.txt","r"))==NULL){return -1;}
    if((sost=fopen("sost.txt","r"))==NULL){return -2;}
    if((dest=fopen("dest.txt","w"))==NULL){return -3;}

    //leggiamo ciò che ci serve da sost.txt e chiudiamolo
    fscanf(sost,"%d",&n);
    for(i=0;(fscanf(sost,"%d %s",&S[i],&nuovo[i])!=EOF)&&i<n;i++){}
    fclose(sost);

    //leggiamo da src e, parola per parola, attraverso funzione,
    //sostituiamo le parole che rispecchiano le richieste
    while(fgets(supp,MAX_L+2,src)!=NULL){
        for(inizio=0, fine=inizio+1 ;(supp[fine-1]!='\0'); fine++){
            if(supp[fine]==' '||(supp[fine]=='\n')){
                if(ispunct(supp[fine-1])){flag=1;}
                for(i=0;(i<n)&&(!trovato);i++){
                    if((fine-inizio-flag)==S[i]){
                        strcpy(parola,nuovo[i]);
                        if(flag){strncpy(&parola[strlen(nuovo[i])],&supp[fine-1],flag);}
                        trovato=1;
                    }
                }
                if(!trovato){strncpy(parola,&supp[inizio],fine-inizio);}
                fprintf(dest,"%s ",parola);
                fine++;
                inizio=fine;
                flag=0;
                trovato=0;

                for(i=0;i<MAX_L;i++){
                    parola[i]='\0';
                }
            }
        }
        fprintf(dest,"\n");
    }

    //chiudo i file
    fclose(src);
    fclose(dest);

    return 0;
}

