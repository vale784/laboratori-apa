#ifndef L08_E04_EXERCISES_H
#define L08_E04_EXERCISES_H

#include "exArray.h"
#include <stdio.h>
#include <stdbool.h>

#define maxEx_len 50

typedef struct Exercise_Type{
    char *exName,*exCate,*exType;
}Ex;

typedef enum EXkeyCompare{s_name,s_index}EXkey_cmp;

Ex   EXbuild(FILE *fp);
void EXdisplay(Ex x,FILE *fp);
bool EXkey_compare(Ex x,const char *s,int n,EXkey_cmp k);

#endif //L08_E04_EXERCISES_H