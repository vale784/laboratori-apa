#ifndef L08_E04_EXLIST_H
#define L08_E04_EXLIST_H

#include "exArray.h"

typedef struct Node_Exercise_Plan *exLink;
typedef struct Exercises_wrapper  *exWrap;

exWrap EXLbuild(ExArr all, const char *path);

void   EXLfree(exWrap x);
void   EXLfreeR(exLink i);

void   EXLdisplay(exWrap plan,ExArr all,FILE *fp);

void   EXLdelete(exWrap plan,ExArr all,const char *toDelete);
void   EXLnew(exWrap plan,ExArr all,FILE *fp);
exLink EXLadd(exLink x,ExArr all,FILE *fp);

void   EXLedit(exWrap plan,ExArr all,const char *toEdit);

#endif //L08_E04_EXLIST_H