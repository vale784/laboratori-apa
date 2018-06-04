#ifndef L08_E04_EXARRAY_H
#define L08_E04_EXARRAY_H

#include "exercises.h"

typedef struct ExArray *ExArr;

ExArr EXAbuild(const char *path);
int   EXAsearch(ExArr x, const char *s,int n,EXkey_cmp k);
void  EXAfree(ExArr x);
char* EXAgetName(ExArr x, int i);

#endif //L08_E04_EXARRAY_H