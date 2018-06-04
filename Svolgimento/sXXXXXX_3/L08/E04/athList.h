#ifndef L08_E04_ATHLIST_H
#define L08_E04_ATHLIST_H

#include "athletes.h"
#include "exList.h"

typedef struct Node_Athlete *atLink;
typedef struct Atl_Wrapper  *atWrap;

atWrap ATLbuild(const char *path);
void ATLfree(atWrap x);
void ATLfreeR(atLink i);
void ATLnew(atWrap all,FILE *fp);
void ATLdelete(atWrap all,int toDelete);
void ATLdisplayAll(atWrap all,FILE *fp);
void ATLdisplayOne(atWrap ATall, ExArr EXall, const char *surname,FILE *fp);
atLink ATLsearch(atWrap all,const char *s,int n,ATkey_cmp k);
void ATLloadPlan(ExArr all,atLink at,const char *path);
void ATLeditplan(atLink at,ExArr EXall,const char *toEdit);

#endif //L08_E04_ATHLIST_H