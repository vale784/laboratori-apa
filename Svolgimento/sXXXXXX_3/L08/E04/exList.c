#include "exList.h"
#include <stdlib.h>

struct Node_Exercise_Plan{
    int indEx,set,rep;
    exLink next;
};

struct Exercises_wrapper{
    exLink head,tail;
    int nEx;
};

exWrap EXLbuild(ExArr all, const char *path)
{
    FILE *fp = fopen(path,"r");
    exWrap toReturn;
    exLink x,p;
    int cnt;

    if(fp==NULL) {printf("No ex. plan found\n"); return NULL;}
    toReturn           = malloc(sizeof(*toReturn));
    x = toReturn->head = malloc(sizeof(*x));

    for(p=x,cnt=0;EXLadd(x,all,fp);cnt++,p=x,x=x->next){
        x->next = malloc(sizeof(*x));
    }

    free(x); toReturn->tail = p; p->next = NULL;
    toReturn->nEx = --cnt;

    fclose(fp);

    return toReturn;
}

void EXLfree(exWrap x)
{
    EXLfreeR(x->head);
    free(x);
}

void EXLfreeR(exLink i)
{
    if(i==NULL) return;

    EXLfreeR(i->next);
    free(i);
}

void EXLdisplay(exWrap plan,ExArr all,FILE *fp)
{
    exLink x;
    char *tmpName;

    for(x=plan->head; x!=NULL ;x = x->next) {
        tmpName = EXAgetName(all,x->indEx);
        fprintf(fp, "%s -> set: %d rep: %d\n",tmpName,x->set,x->rep);
    }
}

void EXLdelete(exWrap plan,ExArr all,const char *toDelete)
{
    exLink x,p;
    int i;

    i = EXAsearch(all,toDelete,0,s_name);

    for(p=x=plan->head; x!=NULL; p=x,x=x->next)
        if(x->indEx==i) {
            if(x == plan->head)
                plan->head = x->next;
            else if (x == plan->tail)
                plan->tail = p;
            else p->next = x->next;

            free(x);
            return;
        }
}

void EXLnew(exWrap plan,ExArr all,FILE *fp)
{
    exLink new = EXLadd(NULL,all,fp);
    plan->tail->next = new;
    plan->tail = new;
    new->next = NULL;
    plan->nEx++;
}

exLink EXLadd(exLink x,ExArr all,FILE *fp)
{
    char name[maxEx_len];

    if(x==NULL)
        x = malloc(sizeof(*x));
    if(fp==stdin)
        printf("<Ex name> <set> <rep>\n");

    if(fscanf(fp,"%s %d %d",name,&x->set,&x->rep)==EOF) return NULL;
    x->indEx    = EXAsearch(all,name,0,s_index);
    x->next     = malloc(sizeof(*x));

    return x;
}

void EXLedit(exWrap plan,ExArr all,const char *toEdit)
{
    exLink x=NULL,p=NULL;
    bool found = false;
    int i,choice,new;

    i = EXAsearch(all,toEdit,0,s_name);

    for(x = plan->head; x!=NULL && !found;p = x, x = x->next)
        if(x->indEx==i)
            found = true;
    if(x==NULL) {printf("ex not found\n"); return;}

    do {
        printf("What do you want to edit:\n1-set\n2-repetitions\n3-back\n");
        scanf("%d", &choice);
    }while(choice!=1&&choice!=2&&choice!=3);

    printf("New: \n"); scanf("%d",&new);

    switch(choice) {
        case 1: p->set = new; break;
        case 2: p->rep = new; break;
        case 3: return;
        default: printf("generic error\n"); return;
    }
}