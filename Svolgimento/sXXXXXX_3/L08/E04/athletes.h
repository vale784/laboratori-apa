#ifndef L08_E04_ATHLETES_H
#define L08_E04_ATHLETES_H

#include <stdio.h>
#include <stdbool.h>

#define maxLength 25

typedef struct Athlete_T{
    char *name,*surname,*cate,*date;
    int hours,code; //>=0
} Atl;

typedef enum ATkeyCompare{s_code,s_surname}ATkey_cmp;

Atl  ATbuild(FILE *fp);
void ATdisplay(Atl x,FILE *fp);
void ATdelete(Atl x);
bool ATkey_compare(Atl x1,const char *s,int n,ATkey_cmp k);

#endif //L08_E04_ATHLETES_H