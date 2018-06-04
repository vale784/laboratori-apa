#include <stdio.h>
#define nFile "nomeArbitrario.txt"
#define MAXS 10
#define N 5

//errori
#define ERRORE_FILE (-1)

typedef int condizione;
#define CONTINUA 1
#define STOP 0

typedef struct item_s
{
    int a,b; char c; float d; char s[MAXS];
}Item;

void leggiFile(FILE *fp,Item mat[][N],int n);
condizione acquisisciCoord(int *r,int *c,int max);

int main(void)
{
    Item M[N][N];
    FILE *fp;
    int r,c;

    if((fp=fopen(nFile,"r"))==NULL) return ERRORE_FILE;
    leggiFile(fp,M,N);
    fclose(fp);

    while(acquisisciCoord(&r,&c,N)==CONTINUA)
        printf("\n%d%d %c%f %s\n",M[r][c].a,M[r][c].b,M[r][c].c,M[r][c].d,M[r][c].s);

    return 0;
}

void leggiFile(FILE *fp,Item mat[][N],int n)
{
    int i, k;
    Item *p;
    for (i = 0, k = 0; i < n; i++)
        for (; k < n; k++) {
            p = &mat[i][k];
            fscanf(fp, "%d%d %c%f %s", &p->a, &p->b, &p->c, &p->d, p->s);
        }
}

condizione acquisisciCoord(int *r,int *c,int max)
{
    printf("inserire riga e colonna 'r c', inserire coordinate impossibili per uscire");
    scanf("%d %d",r,c);
    if(*r<0||*c<0||*r>max||*c>max) return STOP;
    return CONTINUA;
}