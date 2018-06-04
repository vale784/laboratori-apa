#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAXR 60
#define MAXC 80
#define MAXP 1000
#define fname "sequenze.txt"
#define scambioP(A,B) {void *p=(A); (A)=(B); (B)=p;}
#define ERR_FNAME (-1)
#define ERR_NRIGHE (-2)
#define ERR_NPAR (-3)

typedef int condizione;
#define CONTINUA 1
#define ESCI 0

typedef int risultato;
#define NON_TROVATO (-1)



int leggiPagina(FILE *fp,int maxr,char pagina[][MAXC+1]);
int riconosciParole(char pagina[MAXR][MAXC+1],int nr,char *p_parole[],int maxp);
void insertionSort(char *p_parole[],int l,int r, int lmax);
int confrontaParole(char *s1, char *s2,int max);
int trovaLunghezza(char *s);
condizione menu(char *p[],const char *primaCella,int maxp,int maxc);
risultato ricercaDicotomica(char *cerca,char *p[],int l,int r);

int main(void)
{
    FILE *fp;
    char pagina[MAXR][MAXC+1];
    char *p_parole[MAXP]; //vettore di puntatori alle parole
    int nr,np;

    if((fp = fopen(fname,"r"))==NULL) return ERR_FNAME;
    if((nr = leggiPagina(fp,MAXR,pagina))==ERR_NRIGHE) return ERR_NRIGHE;
    fclose(fp);

    if((np = riconosciParole(pagina,nr,p_parole,MAXP))==ERR_NPAR) return ERR_NPAR;

    insertionSort(p_parole,0,np-1,MAXC);

    while(menu(p_parole,&pagina[0][0],np,MAXC));

    return 0;
}

int leggiPagina(FILE *fp,int maxr,char pagina[][MAXC+1])
{
    int nr;
    for(nr = 0;(fgets(pagina[nr],MAXC+1,fp) != NULL);nr++);
    return (nr <= maxr)? nr : ERR_NRIGHE;
}

int riconosciParole(char pagina[][MAXC+1],int nr,char *p_parole[],int maxp)
{
    int cnt,i,j;
    for(i=cnt=0;i<nr;i++){
        for(j=0;(pagina[i][j]!='\n')&&(pagina[i][j]!='\0');j++){
            if(isalnum(pagina[i][j])){
                p_parole[cnt]=&pagina[i][j];
                cnt++;
                if(cnt>maxp) return ERR_NPAR;
                while(isalnum(pagina[i][j])) j++;
            }
        }
    }
    return cnt;
}

void insertionSort(char *p_parole[],int l,int r, int lmax)
{
    if(r-l>lmax) return;
    int i,j;
    void *v;
    //scan lineare per mettere l'elemento più piccolo a sinistra
    for(i=r;i>l;i--)
        if (confrontaParole(p_parole[i], p_parole[i-1],lmax) < 0)
            scambioP(p_parole[i - 1], p_parole[i]);

    for(i=l+2;i<=r;i++) {
        //il valore più piccolo è già a sinistra, non serve un controllo sul limite sinistro
        for (j = i, v = p_parole[i]; confrontaParole(v,p_parole[j-1],lmax) < 0; j--)
            p_parole[j] = p_parole[j - 1];
        p_parole[j]=v;
    }
}

int confrontaParole(char *s1, char *s2,int max)
{
    int l1 = trovaLunghezza(s1), l2 = trovaLunghezza(s2);
    int i;
    char st1[max],st2[max];
    strncpy(st1,s1,(size_t)l1);
    strncpy(st2,s2,(size_t)l2);
    for(i=0;i<l1;i++) st1[i]= (char) tolower(st1[i]);
    for(i=0;i<l2;i++) st2[i]= (char) tolower(st2[i]);

    return strncmp(st1, st2, (size_t) ((l1 > l2) ? l2 : l1));
}

int trovaLunghezza(char *s)
{
    int l;
    for(l=0;isalnum(s[l]);l++);
    return l;
}

condizione menu(char *p[],const char *primaCella, int maxp,int maxc)
{
    char cerca[maxc];
    int k,supp;
    risultato ind;
    printf("Inserire la parola da cercare nel testo o digita '$fine' per terminare il programma: ");
    scanf("%s", cerca);

    if (strcmp(cerca, "$fine") == 0) return ESCI;

    ind = ricercaDicotomica(cerca,p, 0, maxp);

    if (ind == NON_TROVATO) {
        printf("\nNon è stato trovato nulla");

    }else{
        for(k=ind;confrontaParole(cerca,p[k],MAXC)==0;k--);
        for(k+=1;confrontaParole(cerca,p[k],MAXC)==0;k++) {
            supp=(int)p[k]-(int)primaCella;
            printf("parola trovata in (%d,%d)\n",supp/maxc,supp%maxc);
        }
    }

    return CONTINUA;
}

//provo questa tr-function
risultato ricercaDicotomica(char *cerca,char *p[],int l,int r)
{
    if(r-l==0) return NON_TROVATO;
    int i=(r-l)/2;
    int ris=confrontaParole(p[i],cerca,MAXC);

    if(ris>0) return ricercaDicotomica(cerca,p,l,l+i-1);
    else if(ris<0) return ricercaDicotomica(cerca,p,l+i+1,r);

    return i;
}
