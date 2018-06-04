#include "athList.h"
#include <stdlib.h>

struct Node_Athlete{
    Atl atl;
    atLink next;
    exWrap exPlan;
};

struct Atl_Wrapper{
    atLink head,tail;
    int nAt;
};

atWrap ATLbuild(const char *path)
{
    FILE *fp;
    atWrap toReturn;
    atLink x,p;
    int i;

    if((fp=fopen(path,"r"))==NULL) return NULL;
    toReturn = malloc(sizeof(*toReturn));

    fscanf(fp,"%d",&toReturn->nAt);
    x = toReturn->head = malloc(sizeof(*x));

    for(i=0,p=x; i<toReturn->nAt; i++, p=x, x = x->next) {
        x->atl    = ATbuild(fp);
        x->exPlan = NULL;
        x->next   = malloc(sizeof(*x));
    }
    free(x); toReturn->tail = p; p->next = NULL;
    fclose(fp);

    return toReturn;
}

void ATLfree(atWrap x)
{
    ATLfreeR(x->head);
    free(x);
}

void ATLfreeR(atLink i)
{
    if(i==NULL) return;

    ATLfreeR(i->next);
    ATdelete(i->atl);
    EXLfree(i->exPlan);
    free(i);
}

void ATLnew(atWrap all,FILE *fp)
{
    atLink new = malloc(sizeof(*new));
    new->atl = ATbuild(fp);
    all->tail->next = new; all->tail = new;
    new->next = NULL;
}

void ATLdelete(atWrap all,int toDelete)
{
    atLink x;

    x = ATLsearch(all, NULL, toDelete, s_code);

    if (x == all->head){
        all->head = all->head->next;
        free(x);
    }else{
        free(x->next);
        x->next = x->next->next;
    }
    all->nAt--;
}

void ATLdisplayAll(atWrap all,FILE *fp)
{
    int i;
    char fName[maxLength];
    atLink x = all->head;

    for(i=0;i<all->nAt,x = x->next;i++)
        ATdisplay(x->atl,fp);
}

void ATLdisplayOne(atWrap ATall, ExArr EXall, const char *surname,FILE *fp)
{
    atLink x = ATLsearch(ATall,surname,0,s_surname);

    ATdisplay(x->atl,fp);
    if(x->exPlan!=NULL)
        EXLdisplay(x->exPlan,EXall,fp);
    else printf("No ex plan found\n");

}

atLink ATLsearch(atWrap all,const char *s,int n,ATkey_cmp k)
{
    atLink x,p;

    for(p = x = all->head; x!=NULL ; p = x, x = x->next)
        if(ATkey_compare(x->atl,s,n,k))
            return p;
    return NULL;
}

void ATLloadPlan(ExArr all,atLink at,const char *path)
{
    at->exPlan = EXLbuild(all,path);
}

void ATLeditplan(atLink at,ExArr EXall,const char *toEdit)
{
    EXLedit(at->exPlan,EXall,toEdit);
}