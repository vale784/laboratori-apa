#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define nFile "C:\\Users\\valer\\Desktop\\atleti.txt"
#define MAXL 25
#define maxlOpz 15
#define NON_TROVATO (-1)
#define TROVATO (!NON_TROVATO)
#define INTEROP 1
#define CARATTEREP 2

#define scambioP(A,B) {void *gp=(A); (A)=(B); (B)=(A);}

//errori
#define ERRORE_FILE (-1)

typedef struct Atleta_s {
    char *nome,*cogn,*cate;
    int oreSett,codice,data; //>=0
} atletaS;

typedef enum Atleta_e {e_codice,e_nome,e_cogn,e_cate,
    e_data,e_oreSett,e_err} atletaE;

typedef enum Menu_e {m_print,m_fprint,m_ordData,m_ordCodice,m_ordCognome,
    m_leggiCat,m_aggOre,m_ricCodice,m_ricCogn,m_esci,m_errMenu} menuE;

typedef enum Ordinato_e { Ord_per_Codice,Ord_per_cognome,
    Ord_per_data,Ord_Not} ordE;

//Questo blocco di funzioni serve per salvare il file nelle struct
void leggiFile(FILE *fp,int nAtleti,atletaS *sp,int maxl);
void salvaInStr(atletaS *sp,char *buf,atletaE campo);
int dataInt(char *s); //formato gg/mm/aaaa

//questa funzione serve a riconvertire da int a str le date
void intData(int n,int *giorno,int *mese,int *anno);

//questo blocco serve per smistare la richiesta nella giusta funzione
menuE trovaCorrispondenza(char *s,char opzioni[][maxlOpz+1]);
ordE menu(atletaS *sp, int nAtleti, menuE comando,ordE Ord);

//Opzione 1 e 2
void prints(atletaS *sp,int ind_partenza,int ind_fine,FILE *fp);

//per gli ordinamenti (opz 3 to 5)
//creaVettP genera un vettore di puntatori da ordinare
void* creaVettP(void *p,int nAtleti,int opz);
void bubbleSortInt(int **p,int l,int r);
void bubbleSortChar(char **p,int l,int r);
//bubble sort (cerca di capire come passare la funzione come parametro)

//opzione 6


//opzione 7
void aggiornaOre(atletaS *sp,int nAtleti);
int ricercaLineareperNome(atletaS *sp, char *s,int nAtleti);

//peparazione alle opzioni 8 e 9
int leggiChiave_i(void);
char* leggiChiave_c(void);

//opzioni 8 e 9 in caso di dati ordinati
int ricercaDicotomicaInt(atletaS *p,int l, int r,int chiave);
int ricercaDicotomicaChar(atletaS *p,int l, int r,char *chiave,size_t lungh);

//opzioni 8 e 9 in caso di dati non ordinati
int ricercaLineareInt(atletaS *p,int nAtleti,int chiave);
int ricercaLineareChar(atletaS *p,int nAtleti, char *chiave, size_t lungh);

int* trovaOmonimi(char **p,int nAtleti);
void ordinaOmonimi(char **p,const int *ind);

int main(void)
{
    //utili al primo blocco
    FILE *fp;
    atletaS *sp;
    ordE Ordine=Ord_Not;
    int nAtleti;

    //utili al secondo blocco
    char opzioni[m_errMenu][maxlOpz+1]={"Llist","Elist","data","codice","cognome",
                                      "categoria","ore","rCodice","rCognome","esci"};
    menuE sceltaM;
    char sceltaS[maxlOpz+1];

    //blocco lettura del file
    if((fp=fopen(nFile,"r"))==NULL) return ERRORE_FILE;
    fscanf(fp,"%d",&nAtleti);
    sp = malloc(nAtleti * sizeof(atletaS));
    leggiFile(fp,nAtleti,sp,MAXL);
    fclose(fp);

    //blocco interazione con l'utente
    do {
        printf("Leggi o Esporta la lista degli atleti: '%s'/'%s'\n"
                       "Ordina per data: '%s'\n"
                       "Ordina per codice: '%s'\n"
                       "Ordina per cognome: '%s'\n"
                       "Leggi per categoria: '%s'\n"
                       "Aggiorna ore settimanali: '%s'\n"
                       "Ricerca per codice: '%s'\n"
                       "Ricerca per cognome: '%s'\n"
                       "Esci dal programma: 'esci'\n" ,
               opzioni[m_print], opzioni[m_fprint], opzioni[m_ordData], opzioni[m_ordCodice],
               opzioni[m_ordCognome], opzioni[m_leggiCat], opzioni[m_aggOre],
               opzioni[m_ricCodice], opzioni[m_ricCogn]);
        scanf("%s", sceltaS);
        printf("\n");
        sceltaM = trovaCorrispondenza(sceltaS,opzioni);

        if((sceltaM!=m_esci)&&(sceltaM<m_errMenu)&&(sceltaM>=m_print))
            menu(sp,nAtleti,sceltaM,Ordine);

    }while(sceltaM!=m_esci);

    free(sp);

    return 0;
}

void leggiFile(FILE *fp,int nAtleti,atletaS *sp,int maxl)
{
    int i;
    char buf[maxl+1];
    atletaE k;
    for (i = 0; i < nAtleti; i++)
        for (k = e_codice; k < e_err; k++) {
            fscanf(fp,"%s",buf);
            salvaInStr(&sp[i],buf,k);
        }
}

void salvaInStr(atletaS *sp,char *buf,atletaE campo)
{
    switch(campo){
        case e_nome:
            (sp->nome)=malloc((strlen(buf)+1)*sizeof(char));
            strcpy(sp->nome,buf);
            break;
        case e_cogn:
            (sp->cogn)=malloc((strlen(buf)+1)*sizeof(char));
            strcpy(sp->cogn,buf);
            break;
        case e_cate:
            (sp->cate)=malloc((strlen(buf)+1)*sizeof(char));
            strcpy(sp->cate,buf);
            break;
        case e_data:
            (sp->data)= dataInt(buf);
            break;
        case e_oreSett:
            (sp->oreSett)=atoi(buf);
            break;
        case e_codice:
            (sp->codice)=atoi(&buf[1]); //salta la A
        default: break;
    }
}

int dataInt(char *s)
{
    int giorno, mese, anno;
    sscanf(s,"%d/%d/%d",&giorno,&mese,&anno);
    return giorno+mese*100+anno*10000;
}

void intData(int n,int *giorno,int *mese,int *anno)
{
    *anno = n/10000;
    *mese = (n-*anno*10000)/100;
    *giorno = n-*anno*10000-*mese*100;
}

menuE trovaCorrispondenza(char *s,char opzioni[][maxlOpz+1])
{
    menuE output;
    for(output = m_print;output < m_errMenu;output++){
        if(strcmp(s,opzioni[output])==0) return output;
    }
    return m_errMenu;

}

ordE menu(atletaS *sp, int nAtleti, menuE comando,ordE Ord)
{
    void *p=sp;
    int chiave_i,indice,*indOmonimi;
    char *chiave_c;

    switch(comando){
        //i cast dei puntatori sono solo per chiarezza mentale
        case m_print:
            prints(sp,0,nAtleti,stdout);
            break;
        case m_fprint:
            prints(sp,0,nAtleti,NULL);
            break;
        case m_ordData:
            p = (int *) creaVettP(&(sp->data),nAtleti,INTEROP);
            bubbleSortInt(p,0,nAtleti);
            prints(p,0,nAtleti,stdout);
            Ord = Ord_per_data;
            break;
        case m_ordCodice:
            p = (int *) creaVettP(&(sp->codice),nAtleti,INTEROP);
            bubbleSortInt((int **)p,0,nAtleti);
            prints(p,0,nAtleti,stdout);
            Ord = Ord_per_Codice;
            break;
        case m_ordCognome:
            p = (char *) creaVettP(&(sp->cogn),nAtleti,CARATTEREP);
            bubbleSortChar((char **)p,0,nAtleti);
            if((indOmonimi=trovaOmonimi(p,nAtleti))!=NULL){
                p = (char *) creaVettP(&(sp->nome),nAtleti,CARATTEREP);
                ordinaOmonimi(p,indOmonimi);
            }
            prints(p,0,nAtleti,stdout);
            Ord = Ord_per_cognome;
            break;
        case m_leggiCat:
            p = (char *) creaVettP(&(sp->cate),nAtleti,CARATTEREP);
            bubbleSortChar((char **)p,0,nAtleti);
            prints(p,0,nAtleti,stdout);
            break;
        case m_aggOre:
            //non importa l'ordinamento perchè non è mai richiesto che sia
            //ordinato in base all'ora
            aggiornaOre(sp,nAtleti);
            break;
        case m_ricCodice:
            chiave_i = leggiChiave_i();
            if(Ord == Ord_per_Codice){
                indice=ricercaDicotomicaInt(p,0,nAtleti,chiave_i);
            }else{
                indice=ricercaLineareInt(p,nAtleti,chiave_i);
            }
            (indice==-1)? printf("Non è stato trovato nulla\n"):prints(sp,indice,indice+1,stdout);
            break;
        case m_ricCogn:
            chiave_c=leggiChiave_c();
            if(Ord == Ord_per_cognome){
                indice=ricercaDicotomicaChar(p,0,nAtleti,chiave_c,strlen(chiave_c));
            }else{
                indice=ricercaLineareChar(p,nAtleti,chiave_c,strlen(chiave_c));
            }
            (indice==-1)? printf("Non è stato trovato nulla\n"):prints(sp,indice,indice+1,stdout);
            break;
        case m_esci:
            printf("e stato un piacere!\n");
            break;
        case m_errMenu: break;
    }
    return Ord;
}

void prints(atletaS *sp,int ind_partenza,int ind_fine,FILE *fp)
{
    int i = ind_partenza;
    int giorno,mese,anno;
    char nomeFile[MAXL];

    if(fp==NULL) {
        while(fp==NULL) {
            printf("Scrivere il nome del file: ");
            scanf("%s", nomeFile);
            printf("\n");
            fp = fopen(nomeFile, "w");
        }
    }

    for( ;i<ind_fine;i++) {
        intData(sp[i].data,&giorno,&mese,&anno);
        fprintf(fp, "A%d %s %s %s %d/%d/%d %d\n", sp[i].codice, sp[i].nome, sp[i].cogn,
                sp[i].cate, giorno,mese,anno, sp[i].oreSett);
    }
    if(fp!=stdout) fclose(fp);
}

void* creaVettP(void *p,int nAtleti,int opz)
{
    void *v;
    char *c;
    int lungh;

    //come avrei potuto farlo senza questo if?
    if(opz==CARATTEREP) {
        c = (char *) p;
        lungh = nAtleti * strlen(c) * sizeof(char);
    }else if(opz==INTEROP){
        lungh = nAtleti * sizeof(int);
    }else return NULL;

    v = malloc((size_t) lungh);

    for(;( (unsigned int) p <= nAtleti*sizeof(atletaS)); p += sizeof(atletaS))
        v=p;
    return v;
}

void aggiornaOre(atletaS *sp,int nAtleti)
{
    char scelta[2*(MAXL+1)];
    int i=NON_TROVATO,supp;
    while(i==NON_TROVATO){
        printf("Di chi si vuole cambiare il monte ore settimanali? ");
        scanf("%s",scelta);
        printf("\n");
        i=ricercaLineareperNome(sp,scelta,nAtleti);
        if(i==TROVATO){
            printf("Inserire il nuovo monte ore settimanali: ");
            scanf("%d",&supp);
            printf("\n");
            sp[i].data=supp;
            return;
        }
        printf("Inserire una scelta valida\n");
    }
}

int ricercaLineareperNome(atletaS *sp, char *s,int nAtleti)
{
    int i;
    char buf[2*MAXL+1];

    for(i=0;i<nAtleti;i++){
        strcpy(buf,sp[i].nome);
        strcat(buf," ");
        strcat(buf,sp[i].cogn);
        if(strcmp(buf,s)==0) return i;
    }
    return NON_TROVATO;
}

int ricercaDicotomicaInt(atletaS *p,int l,int r,int chiave)
{
    if(r-l==1) return -1;
    int i=(r-l/2);
    if(chiave < p[i].codice)
        return ricercaDicotomicaInt(p,l,i-1,chiave);
    else if(chiave > p[i].codice)
        return ricercaDicotomicaInt(p,i+1,r,chiave);

    return i;
}

int ricercaDicotomicaChar(atletaS *p,int l,int r,char *chiave,size_t lungh)
{
    if(r-l==1) return -1;
    int i=(r-l/2);
    if(strncmp(p[i].cogn,chiave,lungh)<0)
        return ricercaDicotomicaChar(p,l,i-1,chiave,lungh);
    else if(strncmp(p[i].cogn,chiave,lungh)>0)
        return ricercaDicotomicaChar(p,i+1,r,chiave,lungh);

    return i;
}

int leggiChiave_i(void)
{
    char str[MAXL];
    int chiave;
    printf("Inserire il codice che si vuole cercare: ");
    scanf("%s",str);
    sscanf(str,"A%d",&chiave);
    return chiave;

}

char* leggiChiave_c(void)
{
    char str[MAXL];
    printf("inserire il cognome (anche parziale): ");
    scanf("%s",str);
    char *p=malloc((strlen(str)+1)*sizeof(char));
    strcpy(p,str);
    return p;
}

int ricercaLineareInt(atletaS *p,int nAtleti,int chiave)
{
    int i;
    for(i=0;i<nAtleti;i++)
        if(p[i].codice==chiave)
            return i;
    return -1;
}

int ricercaLineareChar(atletaS *p,int nAtleti, char *chiave, size_t lungh)
{
    int i;
    for(i=0;i<nAtleti;i++)
        if(strncmp(p[i].cogn,chiave,lungh)==0)
            return i;
    return -1;
}

void bubbleSortInt(int **p,int l,int r)
{
    int i,k;
    for(i=l+1;i<r;i++)
        for(k=r;k>i;k--)
            if((*p)[k]<(*p)[k-1])
                scambioP(p[k],p[k-1]);
}

void bubbleSortChar(char **p,int l,int r)
{
    int i,k;
    for(i=l+1;i<r;i++)
        for(k=r;k>i;k--)
            if(strcmp(p[k],p[k-1])<0)
                scambioP(p[k],p[k-1]);
}

int* trovaOmonimi(char **p,int nAtleti)
{
    char *supp;
    int *out;
    int i,k=1,l,cnt;

    out = (int) malloc(k * sizeof(int));

    for(i=0,cnt=1;i<nAtleti;i++){
        if(strcmp(p[i],p[i+1])==0){
            if(cnt==k) {
                k *= 2;
                out = (int) realloc(out,k * sizeof(int));
            }
            out[i]=i;
            cnt++;
            l = strlen(p[i]);
            supp = (char *) malloc((l+1)*sizeof(char));
            strcpy(supp,p[i]);

            while(strcmp(p[i],p[out[i]])==0) i++;
            i--;
        }
    }

    if(cnt==k) {
        k++;
        out = (int) realloc(out, k * sizeof(int));
    }
    out[cnt]=-1;

    if(cnt==1) return NULL;

    return out;
}

void ordinaOmonimi(char **p,const int *ind)
{
    int iOmonimi=0,iAtleti=0,l,r;

    while(ind[iOmonimi]!=-1){
        l=ind[iOmonimi];
        iAtleti=l;
        while(strcmp(p[l],p[iAtleti])==0)
            iAtleti++;
        r=iAtleti;
        bubbleSortChar(p,l,r);
        iOmonimi++;
    }
}