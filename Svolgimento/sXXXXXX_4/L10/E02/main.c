#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "BST.h"
#include "Item.h"

typedef struct Stat{
    char stationId[MAX_STAT_ID_LENGTH]; int parking,nBikes;
}stat_t;

bool stop(void);
stat_t * readStations(FILE*,int*);
bool scanCard(char*,char*,int,BST,stat_t*,int);

int main(int argc, char *argv[])
{
    if(argc!=4) return 1;

    int N,time;
    char cardId[MAX_USR_ID_LENGTH],stationId[MAX_STAT_ID_LENGTH];
    stat_t *stations;
    BST bst = BSTinit();
    FILE *f_in,*f_update,*f_out;

    if((f_in =fopen(argv[1],"r"))==NULL) return 2;
    stations = readStations(f_in,&N);
    fclose(f_in);

    if((f_out   =fopen(argv[2],"w"))==NULL) return 3;
    if((f_update=fopen(argv[3],"r"))==NULL) return 4;

    do{

        fscanf(f_update,"%s %s %d",cardId,stationId,&time);
        if(!scanCard(cardId,stationId,time,bst,stations,N))
            return 5;

    }while(!stop);
    fclose(f_out); free(stations);

    return 0;
}

stat_t * readStations(FILE *fp,int *N)
{
    int i;

    fscanf(fp,"%d",N);
    stat_t *all = malloc((*N)*sizeof(*all));

    for(i=0;i<*N;i++)
        fscanf(fp,"%s %d %d",all[i].stationId,&all[i].parking,&all[i].nBikes);

    return all;
}

bool stop(void)
{
    int a;
    printf("\nPress Y to continue, any other key to stop"); scanf("%d",&a);
    if(a=='Y') return true;
    return false;
}

bool scanCard(char cardId[MAX_USR_ID_LENGTH],char stationId[MAX_STAT_ID_LENGTH],int time,
              BST bst,stat_t* s,int n)
{
    Item x = BSTsearch(bst,cardId);
    stat_t station;
    int sz = BSTcount(bst),i;

    for(i=0;i<n;i++) if(strcmp(s[i].stationId,stationId)==0) station = s[i];

    if(x==ITEMsetVoid()){
        free(x); x = ITEMnew(cardId,stationId,time);
        BSTinsert_leafR(bst,x);
    }

    if((!ITEMactiveRent(x)) && station.nBikes!=0)
        ITEMstartRent(x,stationId,time);

    else if(station.nBikes!=station.parking)
        ITEMstopRent(x,time);

    return true;
}