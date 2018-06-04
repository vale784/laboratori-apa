#include "exercises.h"
#include <string.h>



Ex EXbuild(FILE *fp)
{
    Ex newEX;
    char exName[maxEx_len],exCat[maxEx_len],exType[maxEx_len];

    fscanf(fp,"%s %s %s",exName,exCat,exType);
    newEX.exName = strdup(exName);
    newEX.exCate = strdup(exCat);
    newEX.exType = strdup(exType);

    return newEX;
}

void EXdisplay(Ex x,FILE *fp)
{
    fprintf(fp,"ex. name: %s cate: %s type: %s\n",x.exName,x.exCate,x.exType);
}

bool EXkey_compare(Ex x,const char *s,int n,EXkey_cmp k)
{
    if(k==s_name) return (strcmp(x.exName,s)==0);
    return false;
}