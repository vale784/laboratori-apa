#include <stdio.h>
#include <stdlib.h>
#define MAXRC 50
#define OK '-'
#define PASSATO '+'
#define MURO 'X'
#define INGRESSO 'I'
#define USCITA 'U'

typedef enum{
ok, muro, uscita, non_semplice, errore
}e_casi;

void leggiLabirinto(int mat[MAXRC][MAXRC], FILE *fp, int *nr, int *nc);
void controlloMovimenti(int lab[MAXRC][MAXRC],FILE *fp,int nr,int nc);
int trovaEntrata(int lab[MAXRC][MAXRC],int *x,int *y,int nr,int nc);
e_casi controllaValido(int *posx,int *posy,int mx,int my, int lab[MAXRC][MAXRC],int nr,int nc);
void risultato(e_casi flag,int nots);

int main(int argc,char*argv[]){
    if(argc!=3){return -1;}

    FILE *fplab,*fpmov;
    int nr,nc;
    int lab[MAXRC][MAXRC];

    if((fplab=fopen(argv[1],"r"))==NULL){return -2;}
    leggiLabirinto(lab,fplab,&nr,&nc);
    fclose(fplab);

    if((fpmov=fopen(argv[2],"r"))==NULL){return -3;}
    controlloMovimenti(lab,fpmov,nr,nc);
    fclose(fpmov);

    return 0;
}

void leggiLabirinto(int mat[MAXRC][MAXRC],FILE *fp,int *nr,int *nc){

    int i,k;

    fscanf(fp,"%d %d",nr,nc);

    for(i=0;i<*nr;i++)
        for(k=0;k<*nc;k++)
            fscanf(fp,"%d",&mat[i][k]);

    return;
}

void controlloMovimenti(int lab[MAXRC][MAXRC],FILE *fp,int nr,int nc){

    int Ix=0,Iy=0; //coordinate dell'ingresso
    int mx,my,posx,posy; //mosse dal file e posizione attuale
    int nots=0;
    e_casi flag;

    if(trovaEntrata(lab,&Ix,&Iy,nr,nc)){
        printf("NON CI STA UN'ENTRATA");
        return;
    }

    posx=Ix; posy=Iy; flag=ok;

    while((fscanf(fp,"%d %d",&mx,&my)!=EOF)&&((flag==ok)||(flag==non_semplice))){
        my=-my; //per l'utente serve un numero negativo per scendere, a me positivo e vversa
        flag=controllaValido(&posx,&posy,mx,my,lab,nr,nc);
        if(flag==non_semplice){nots=1;}
    }

    risultato(flag,nots);

    return;
}

int trovaEntrata(int mat[MAXRC][MAXRC],int *x,int *y,int nr,int nc){
    int i,k;

    for(i=0;i<nr;i++){
        for(k=0;k<nc;k++){
            if(mat[i][k]==INGRESSO){
                *x=k;
                *y=i;
                return 1;
            }
        }
    }
    return 0;
}

e_casi controllaValido(int *posx,int *posy,int mx,int my, int lab[MAXRC][MAXRC],int nr,int nc){

    int i,k,flag=0;
    int extrOr,extrVert;

    extrOr=*posx+mx;
    extrVert=*posy+my;

    if((extrOr<0)||(extrOr>nc)||(extrVert<0)||(extrVert>nr)){
        return errore;
    }

    if(mx>0){
        for(i=*posx,k=*posy;i<extrOr;i++){
            if(lab[i][k]==MURO){return muro;

            }else if(lab[i][k]==PASSATO){flag=1;

            }else if(lab[i][k]==USCITA){return uscita;

            }else{lab[i][k]=PASSATO;}
        }
    }else if(mx<0){
        for(i=*posx,k=*posy;i>extrOr;i--){
            if(lab[i][k]==MURO){return muro;

            }else if(lab[i][k]==PASSATO){flag=1;

            }else if(lab[i][k]==USCITA){return uscita;

            }else{lab[i][k]=PASSATO;}
        }
    }
    if(my>0){
        for(i=*posy,k=*posy;k<extrVert;k++){
            if(lab[i][k]==MURO){return muro;

            }else if(lab[i][k]==PASSATO){flag=1;

            }else if(lab[i][k]==USCITA){return uscita;

            }else{lab[i][k]=PASSATO;}
        }
    }else if(my<0){
        for(i=*posx,k=*posy;k>extrOr;k--){
            if(lab[i][k]==MURO){return muro;

            }else if(lab[i][k]==PASSATO){flag=1;

            }else if(lab[i][k]==USCITA){return uscita;

            }else{lab[i][k]=PASSATO;}
        }
    }

    *posy+=my;
    *posx+=mx;

    if(flag){return non_semplice;}

    return ok;
}

void risultato(e_casi flag,int nots){

    switch(flag){
        case muro:
            printf("Il cammino non è valido");
            break;
        case uscita:
            printf("Il cammino è valido e%s semplice",(nots==1)? " non":"");
            break;
        case errore:
            printf("Errore imprevisto\n");
        default:
            printf("Non trovato cammino da I ad U");
    }

    return;
}


