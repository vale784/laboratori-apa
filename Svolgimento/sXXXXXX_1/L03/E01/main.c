#include <stdio.h>
#include <stdlib.h>
#define BIANCO 0
#define NERO 1
#define RCMAX 50
#define fname "nomeArbitrario.txt"


typedef struct{
    int estrSx[2], base, altezza;
}output;

output trovaQuadrato(int mat[RCMAX][RCMAX],int x, int y, int nr, int nc);

int main()
{
    int nr,nc,i,k,cont;
    int mat[RCMAX][RCMAX];
    output s;
    FILE *fp;

    //acquisizione matrice
    if((fp=fopen(fname,"r"))==NULL){return -1;}
    fscanf(fp,"%d %d",&nr,&nc);
    for(i=0;i<nr;i++){
        for(k=0;k<nc;k++){
            fscanf(fp,"%d",&mat[i][k]);
        }
    }
    fclose(fp);

    //scan matrice
    for(i=0,cont=1;i<nr;i++){
        for(k=0;k<nc;k++){
            if(mat[i][k]==NERO){
                s=trovaQuadrato(mat,i,k,nr,nc);
                printf("Regione %d: estr. sup. SX = <%d,%d> b = %d, h = %d, Area = %d\n",cont,s.estrSx[0],s.estrSx[1],s.base,s.altezza,s.base*s.altezza);
                cont++;
            }
        }
    }
    return 0;
}

output trovaQuadrato(int mat[RCMAX][RCMAX],int x, int y, int nr, int nc){
    output s;

    int i,k;
    //base e altezza inizializzati a 0 perchè più avanti a causa dell'iterazione ci sarà
    //un conteggio in più
    int base=0,altezza=0,flag=0;

    //for(i=x,base=1;((i<nr)&&(mat[i][y]==NERO));i++,base++){
    //    mat[i][y]=BIANCO;
    //}
    //for(k=y,altezza=1;((k<nc)&&(mat[x][k]==NERO));k++,altezza++){
    //    mat[x][k]=BIANCO;
    //}

    for(i=x;(i<nr)&&(!flag);i++){
        if(mat[i][y]==BIANCO){flag=1;}
        for(k=y;(k<nc)&&(mat[i][k]==NERO);k++){
            mat[i][k]=BIANCO;
            if(i==x){base++;}
            if(k==y){altezza++;}
        }
    }

    s.base=base;
    s.altezza=altezza;
    s.estrSx[0]=x;
    s.estrSx[1]=y;
    return s;
}
