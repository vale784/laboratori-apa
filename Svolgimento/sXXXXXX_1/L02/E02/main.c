#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define File_sost "sequenze.txt"
#define File_testo "testo.txt"
#define Max_Seq 20
#define Max_L_seq 5
#define Max_L_line 200
#define Max_L_word 25
#define VERO 1
#define FALSO 0

void acquisizioneSequenze(FILE *fp,int N_sequenze,char dest[Max_L_seq][Max_L_seq+1]);
int trovaPrefisso(char riga[Max_L_line+2],char sost[Max_L_seq+1],int cont);
int testParola(char parola[Max_L_word+1],char pref[Max_L_seq]);
void azzeramento(char stringa[Max_L_word]);


int main()
{
    FILE *fp_sost,*fp_testo;
    int N_sequenze,i,cont;
    char sost[Max_L_seq][Max_L_seq+1];
    char riga[Max_L_line+2],parola[Max_L_seq+1];

    //con questo ciclo salviamo i prefissi
    //da sostituire in un vettore di stringhe
    fp_sost=fopen(File_sost,"r");
    fscanf(fp_sost,"%d",&N_sequenze);
    acquisizioneSequenze(fp_sost,N_sequenze,sost);
    fclose(fp_sost);


    //Ad ogni iterazione riapriamo il file controllando
    //tutte le parole nel vettore di prima
    for(i=0,cont=0;i<N_sequenze;i++,cont=0){
        //copiamo in parola il prefisso da cercare e
        //lo stampiamo
        strcpy(parola,sost[i]);
        printf("%s\n",parola);

        fp_testo=fopen(File_testo,"r");
        while(fgets(riga,Max_L_line,fp_testo)!=NULL){
            cont=trovaPrefisso(riga,parola,cont);
        }
        printf("\n");
        fclose(fp_testo);
        azzeramento(parola);
    }

    return 0;
}

void acquisizioneSequenze(FILE *fp,int N_sequenze,char dest[Max_L_seq][Max_L_seq+1]){
    int i;
    //ciclo di acquisizione dal file e eliminazione di maiuscole
    for(i=0;i<N_sequenze;i++){
        fscanf(fp,"%s",dest[i]);
        if(isupper(dest[i][0])){
            dest[i][0]=tolower(dest[i][0]);
        }
    }
    return;
}



int trovaPrefisso(char riga[Max_L_line+2],char sost[Max_L_seq],int cont){
    int i,inizio;
    char supp[Max_L_word];

    for(i=0,inizio=0;i<strlen(riga);i++){
        //non si deve presentare la situazione che uno spazio venga contato come parola
        if(((ispunct(riga[i]))||(isspace(riga[i])))&&(!(ispunct(riga[i+1]))&&!(isspace(riga[i+1])))){
            cont++;
            strncpy(supp,&riga[inizio],i-inizio);
            //Questa riga la aggiungo per problemi di cui non mi spiego la
            //natura sulla prima parola e qualche altra
            supp[i-inizio]='\0';
            inizio=i+1;
            if(testParola(supp,sost)){
                printf("%s %d\n",supp,cont);
            }
            //non so perchè ma le funzioni di manipolazione delle stringhe fanno
            //fatica a mettere un terminatore di stringa alla fine della sequenza
            //quindi azzero la stringa ogni volta e faccio prima
            azzeramento(supp);
        }
    }
    return cont;
}


//qeuesta è una funzione booleana che verifica che la parola passata ad essa
//abbia il prefisso (passato anch'esso)
int testParola(char parola[Max_L_word],char pref[Max_L_seq]){

    int l=strlen(pref),flag=0;

    //controllo per eventuali maiuscole
    if(isupper(parola[0])){parola[0]=tolower(parola[0]); flag=1;}

    //se i primi n caratteri sono uguali allora return vero,
    //in caso contrario falso
    if(!strncmp(parola,pref,l)){
        if(flag){parola[0]=toupper(parola[0]);}
        return VERO;
    }else{return FALSO;}

}

//Questa è la funzione di azzeramento della stringa
void azzeramento(char stringa[Max_L_word]){
    int i;

    for(i=0;i<Max_L_word;i++){
        stringa[i]='\0';
    }
    return;
}

