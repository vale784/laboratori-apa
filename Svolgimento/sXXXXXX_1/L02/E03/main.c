#include <stdio.h>
#include <stdlib.h>
#define MAX 20
#define File_Arbitrario "giocoSadico.txt"

/*SI SAREBBE POTUTO FARE IN MOLTI MODI PIU
ELEGANTI, VOLEVO SOLO TESTARMI PER VEDERE SE AVEVO CAPITO IL FUNZIONAMENTO
DELLE STRUCT*/


typedef struct{
    int righe_inizio;
    int righe_fine;
    int colonne_inizio;
    int colonne_fine;
}sottMat;

typedef struct{
    sottMat NE;
    sottMat NO;
    sottMat SE;
    sottMat SO;
}tutteSott;

void acquisizioneMat(FILE *fp,int dest[MAX][MAX],int nr ,int nc);
tutteSott inputUtente(int matrice[MAX][MAX],int nr,int nc);
sottMat associa(int a,int b,int c,int d);
int sommaSott(sottMat smat,int mat[MAX][MAX],char punto_cardinale[4]);


int main()
{
    FILE *fp;
    int i,nr,nc,somma;
    int mat[MAX][MAX];
    tutteSott TUTTE;

    if((fp=fopen(File_Arbitrario,"r"))==NULL){return -1;}
    if((fscanf(fp,"%dX%d",&nr,&nc))==EOF){printf("file vuoto"); return -2;};
    acquisizioneMat(fp,mat,nr,nc);
    fclose(fp);

    TUTTE=inputUtente(mat,nr,nc);

    for(i=0;i<4;i++){
        switch(i){
            case 0: somma=sommaSott(TUTTE.NO,mat,"NO"); break;
            case 1: somma=sommaSott(TUTTE.NE,mat,"NE"); break;
            case 2: somma=sommaSott(TUTTE.SO,mat,"SO"); break;
            case 3: somma=sommaSott(TUTTE.SE,mat,"SE"); break;
            default: printf("ERRORE"); return -2;
        }
        printf(" => somma = %d\n",somma);
    }

    return 0;
}


void acquisizioneMat(FILE *fp,int dest[MAX][MAX],int nr ,int nc){
int i,k;

for(i=0;i<nr;i++){
    for(k=0;k<nc;k++){
        fscanf(fp,"%d",&dest[i][k]);
    }
}
return;
}


tutteSott inputUtente(int matrice[MAX][MAX],int nr,int nc){

tutteSott TUTTE;
int r,c;
printf("inserire delle coordinate valide nel formato RxC\n(o almeno un numero negativo per uscire).\n");
scanf("%dx%d",&r,&c);
if(r<0||c<0){printf("coordinate valide...\n"); exit(-1);}

TUTTE.NO=associa(0,r,0,c);
TUTTE.NE=associa(0,r,c,nc);
TUTTE.SO=associa(r+1,nr,0,c);
TUTTE.SE=associa(r+1,nr,c+1,nc);

return TUTTE;
}

sottMat associa(int a,int b,int c,int d){
    sottMat m;

    m.righe_inizio=a;
    m.righe_fine=b;
    m.colonne_inizio=c;
    m.colonne_fine=d;
    return m;
}

int sommaSott(sottMat smat,int mat[MAX][MAX],char punto_cardinale[4]){
    int i,k,somma=0;
    printf("%s = ",punto_cardinale);
    for(i=smat.righe_inizio; i<smat.righe_fine; i++){
        for(k=smat.colonne_inizio; k<smat.colonne_fine; k++){
            printf("%d, ",mat[i][k]);
            somma+=mat[i][k];
        }
    }
    return somma;
}


