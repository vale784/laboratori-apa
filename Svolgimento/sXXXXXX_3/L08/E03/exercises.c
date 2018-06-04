#include "exercises.h"

#include <stdlib.h>
#include <string.h>

#define maxEx_len 50


struct Exercise_Type{
    char *exName,*exCate,*exType;
};

struct Node_Exercise_Plan{
    exS exercise;
    int  set,rep;
    exLink next;
};

struct Exercise_Wrapper{
    exS  exArray;
    int  nEx;
};

/*--------------------------------------------------------------------------------------------------------------------*/

int readExercises(FILE *fp, Ex_Label *exercises)
{
    int E,i;
    char exName[maxEx_len],exCat[maxEx_len],exType[maxEx_len];
    Ex_Label supp = *exercises = malloc(sizeof(*supp));

    fscanf(fp,"%d",&E);
    supp->nEx=E;
    supp->exArray = malloc(E * sizeof(*supp->exArray));

    for(i=0;(i<E)&&(fscanf(fp,"%s %s %s",exName,exCat,exType)==3);i++){
        supp->exArray[i].exName = strdup(exName);
        supp->exArray[i].exCate  = strdup(exCat);
        supp->exArray[i].exType = strdup(exType);
    }
    return 0;
}

/*--------------------------------------------------------------------------------------------------------------------*/

void ex_print(exLink head,exLink tail,FILE *fp)
{
    if(head==NULL) {printf("No exercise plan loaded"); return;}
    exLink x;

    for(x=head;x!=tail->next;x=x->next)
        fprintf(fp,"ex. name: %s set: %d rep: %d\n",x->exercise->exName,x->set,x->rep);

}

/*--------------------------------------------------------------------------------------------------------------------*/

void loadExercises(exLink *head,exLink *tail,Ex_Label wrap_ex,const char *path)
{
    FILE *fp;
    exLink x,p;
    char name[maxEx_len];

    if((fp=fopen(path,"r"))==NULL) {printf("No ex. plan found\n"); return;}

    x = *head = malloc(sizeof(*x));

    for(p=x;fscanf(fp,"%s %d %d",name,&x->set,&x->rep)!=EOF;p=x,x=x->next){
        x->exercise = findExercise(wrap_ex,name);
        x->next     = malloc(sizeof(*x));
    }
    free(x); *tail=p; p->next = NULL;
}

/*--------------------------------------------------------------------------------------------------------------------*/

void ex_removeNode(exLink *head)
{
    exLink toRemove;
    char exName[maxEx_len];

    printf("what's the name of ex. you want to delete?: "); scanf("%s",exName);

    if(strcmp((*head)->exercise->exName,exName)==0) {*head = ex_delHead(head); return;}

    toRemove = ex_linearSearch(*head,NULL,0,exName,e_del);

    ex_delNode(toRemove);
}

void ex_delNode(exLink prNode)
{
    exLink toDelete = prNode->next; prNode->next = toDelete->next;
    free(toDelete->exercise); free(toDelete);
}

exLink ex_delHead(exLink *head)
{
    exLink newHead = (*head)->next;
    free((*head)->exercise); free(*head);
    return newHead;
}

/*--------------------------------------------------------------------------------------------------------------------*/

void ex_addNode(Ex_Label wrap_ex,exLink *tail)
{
    char name[maxEx_len];
    exLink newNode;
    exS toAdd;

    printf("Type the name of exercise you want to add to the athlete plan"); scanf("%s",name);
    toAdd = findExercise(wrap_ex,name);
    if(toAdd==NULL) {printf("exercise not found\n"); return;}

    newNode = malloc(sizeof(*newNode));

    printf("Type <set> <rep>"); scanf("%d %d",&newNode->set,&newNode->rep);
    newNode->exercise = toAdd;
    (*tail)->next = newNode;
    *tail = newNode;
}

/*--------------------------------------------------------------------------------------------------------------------*/

void exDetails(Ex_Label wrap_ex)
{
    exS toDisplay;
    char name[maxEx_len];

    printf("Type the name of exercise you want to add to the athlete plan: "); scanf("%s",name);

    toDisplay = findExercise(wrap_ex,name);

    printf("name: %s type: %s category: %s\n",toDisplay->exName,toDisplay->exType,toDisplay->exCate);

}

/*--------------------------------------------------------------------------------------------------------------------*/

void editSet(exLink head)
{
    char name[maxEx_len];
    int newNumber;
    exLink toEdit;

    printf("Type the name of the exercise of which you want to edit set number: "); scanf("%s",name);

    toEdit = ex_linearSearch(head,NULL,0,name,e_set);

    printf("What's the new number?: "); scanf("%d",&newNumber);
    toEdit->set =newNumber;
}

void editRep(exLink head)
{
    char name[maxEx_len];
    int newNumber;
    exLink toEdit;

    printf("Type the name of the exercise of which you want to edit rep number: "); scanf("%s",name);

    toEdit = ex_linearSearch(head,NULL,0,name,e_set);

    printf("What's the new number?: "); scanf("%d",&newNumber);
    toEdit->rep =newNumber;
}

/*--------------------------------------------------------------------------------------------------------------------*/

exLink ex_linearSearch(exLink head,exLink tail,int key_i,const char *key_c,editE command)
{
    exLink x;

    for(x=head;x!=tail;x=x->next)
        switch(command){
            case e_del: case e_set: case e_rep:
                if(strcmp(x->next->exercise->exName,key_c)==0)
                    return x;
                break;
            default: break;
        }
    return NULL;
}

exS findExercise(Ex_Label wrap_ex,const char *name)
{
    int i;
    for(i=0;i<wrap_ex->nEx;i++)
        if(strcmp(wrap_ex->exArray[i].exName,name)==0)
            return &wrap_ex->exArray[i];
    return NULL;
}
