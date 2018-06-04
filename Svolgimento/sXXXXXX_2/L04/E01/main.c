#define fname "nomeArbitrario"
#include <stdio.h>
#define BIANCO 0
#define NERO 1
#define RCMAX 50

typedef struct{
    int  larghezza, altezza, maxX, maxY, minX, minY;
}dimensioni;

int trovaQuadrato(int mat[RCMAX][RCMAX],int x, int y, int nr, int nc,dimensioni *s);
void acquisizioneMatrice(int mat[RCMAX][RCMAX],FILE *fp,int *nr,int *nc);
void scanMatrice(int mat[RCMAX][RCMAX],int nr,int nc);
void inizializzaStruttura(dimensioni* s, int i, int k);

/*versione migliorata in grado di leggere anche le figure non rettangolari*/

int main(void) {
    int nr,nc;
    int mat[RCMAX][RCMAX];
    FILE *fp;


    if((fp=fopen(fname,"r"))==NULL){return -1;}
    acquisizioneMatrice(mat,fp,&nr,&nc);
    fclose(fp);

    scanMatrice(mat,nr,nc);

    return 0;
}

void acquisizioneMatrice(int mat[RCMAX][RCMAX],FILE *fp,int *nr,int *nc)
{
    int i,k;
    fscanf(fp,"%d %d",nr,nc);
    for(i=0;i<(*nr);i++){
        for(k=0;k<(*nc);k++){
            fscanf(fp,"%d",&mat[i][k]);
        }
    }
}

void scanMatrice(int mat[RCMAX][RCMAX],int nr,int nc)
{
    int i,k,cont,area;
    dimensioni s;

    for(i=0,cont=1;i<nr;i++){
        for(k=0;k<nc;k++){
            if(mat[i][k]==NERO){
                inizializzaStruttura(&s,i,k);
                area=trovaQuadrato(mat,i,k,nr,nc,&s);
                printf("Regione %d: estr. sup. SX = <%d,%d> larghezza = %d, h = %d, Area = %d\n",cont,i,k,s.larghezza,s.altezza,area);
                cont++;
            }
        }
    }
}

void inizializzaStruttura(dimensioni *s, int i, int k)
{
    s->minY=i;
    s->maxY=i;
    s->minX=k;
    s->maxX=k;
    s->larghezza=1;
    s->altezza=1;
}

int trovaQuadrato(int mat[RCMAX][RCMAX],int x, int y, int nr, int nc, dimensioni *s)
{
    //genera confusione ma mi muovo in un piano cartesiano ruotato di 90 gradi
    //dove sulla verticale abbiamo la x e in orizzontale la y
    int area = 1;

    mat[x][y] = BIANCO; //questa riga altrimenti la ricorsione impazzisce

    //da qui fino al prossimo commento si tratta di incrementare
    //le dimensioni massime lette e aggiornare le caselle più estreme
    if (s->maxY < y) {
        s->larghezza++;
        s->maxY = y;
    } else if (s->minY > y) {
        s->larghezza++;
        s->minY = y;
    }

    if (x > s->maxX) {
        s->altezza++;
        s->maxX = x;
    } else if (x < s->minX) {
        s->altezza++;
        s->minX = x;
    }
    //sarà difficile spiegarlo con un pianco cartesiano ruotato

    //qui inizia la ricorsione vera a propria
    if (mat[x][y + 1] == NERO) { area += trovaQuadrato(mat, x, y + 1, nr, nc, s); }
    if (mat[x][y - 1] == NERO) { area += trovaQuadrato(mat, x, y - 1, nr, nc, s); }
    if (mat[x + 1][y] == NERO) { area += trovaQuadrato(mat, x + 1, y, nr, nc, s); }
    if (mat[x - 1][y] == NERO) { area += trovaQuadrato(mat, x - 1, y, nr, nc, s); }

    return area;
}