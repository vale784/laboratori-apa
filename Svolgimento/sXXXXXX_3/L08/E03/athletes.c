#include "athletes.h"

#include <stdlib.h>
#include <string.h>

#define maxLength 25

/*----------------------------------------------------------------------------------------------------------------------
 * STRUCTURES
 */

struct Atl_Wrapper{
    int nAthletes;
    atLink head,tail;
};

struct Node_Athlete {
    struct {
        char *name,*surname,*cate,*date;
        int hours,code; //>=0
    } atl;
    atLink next;
    exLink head;
    exLink tail;
};

/*----------------------------------------------------------------------------------------------------------------------
 * ATHLETE FILE READING
 */

int readAtl(FILE *fp, Atl_Label wrap_atl)
{
    wrap_atl = malloc(sizeof(*wrap_atl));
    atLink x;
    int cnt,nAt;

    fscanf(fp,"%d",&nAt);  wrap_atl->nAthletes = nAt;

    for( x = wrap_atl->head = at_newNode(NULL, &fp),cnt = 1;(cnt <= nAt);x = x->next,cnt++) {
        if(cnt==nAt) {wrap_atl->tail = x; x->next = NULL;}
        else         {x->next = at_newNode(NULL, &fp);}
    }

    return 0;
}


/*----------------------------------------------------------------------------------------------------------------------
 * PRINT ATHLETE INFORMATION
 */

void atl_print(Atl_Label wrap_atl, FILE *fp)
{
    char supp[maxLength];
    atLink x,l = wrap_atl->head,r = wrap_atl->tail;
    if(fp==NULL) {
        printf("File name: "); scanf("%s",supp);
        if((fp=fopen(supp,"w"))==NULL) return;
    }
    for(x=l;(x!=r->next);x=x->next)
        fprintf(fp,"A%0.6d %s %s %s %s %d\n",x->atl.code,x->atl.name,
                x->atl.surname,x->atl.cate,x->atl.date,x->atl.hours);

    fprintf(fp,"\n");
    if(fp!=stdout) fclose(fp);
}

/*----------------------------------------------------------------------------------------------------------------------
 *  ADD - REMOVE ATHLETES
 */

void at_newTail(Atl_Label wrap_atl, FILE *fp)
{getchar(); wrap_atl->tail = at_newNode(wrap_atl->tail, fp);}

atLink at_newNode(atLink node, FILE *fp)
{
    atLink newNode;
    char name[maxLength],surname[maxLength],cate[maxLength],date[maxLength];

    newNode = malloc(sizeof(*newNode));

    if(fp == stdin) {
        printf("Type New Athlete in format: \n");
        printf("A<code> <name> <surname> <category> <date of birth> <weekly training hours>\n");
    }

    if(fp!=stdin)
        fscanf(fp,"A%d %s %s %s %s %d\n",&newNode->atl.code,name,surname,cate,date,&newNode->atl.hours);
    else
        scanf("A%d %s %s %s %s %d",&newNode->atl.code,name,surname,cate,date,&newNode->atl.hours);
    (newNode->atl.name)    = strdup(name);
    (newNode->atl.surname) = strdup(surname);
    (newNode->atl.cate)    = strdup(cate);
    (newNode->atl.date)    = strdup(date);

    if(node!=NULL)
    {newNode->next = (node)->next; (node)->next = newNode;}

    return newNode;
}

void at_removeNode(Atl_Label wrap_atl, menuE command)
{
    int code;
    atLink toRemove,head = wrap_atl->head,tail = wrap_atl->tail;

    printf("Write the Code of the Athlete You Want to Remove: "); scanf("\nA%d",&code);
    if(!codeMatches(code,head)) {
        toRemove = at_linearSearch(head, tail->next, NULL, code, command); //returns the precedent
        if (toRemove == NULL) { printf("\nNot Found"); return; }
        at_delNode(toRemove);

    } else head = at_delHead(&head);
}

void at_delNode(atLink prNode)
{
    atLink toDelete = prNode->next; prNode->next = toDelete->next;
    free(toDelete->atl.name); free(toDelete->atl.surname);
    free(toDelete->atl.cate); free(toDelete->atl.date);
    free(toDelete);
}

atLink at_delHead(atLink *head)
{
    atLink newHead = (*head)->next;
    free((*head)->atl.name); free((*head)->atl.surname);
    free((*head)->atl.cate); free((*head)->atl.date);
    free(*head);
    return newHead;
}

/*----------------------------------------------------------------------------------------------------------------------
 *  SEARCH BY CODE
 */

void searchByCode(Atl_Label wrap_atl,menuE command)
{
    int code;
    atLink toDisplay,x;

    printf("Type code: "); scanf("%d",&code);
    toDisplay = at_linearSearch(wrap_atl->head, wrap_atl->tail->next, NULL, code, command);
    if(toDisplay==NULL){printf("\nNot Found"); return;}
    x = toDisplay;
    fprintf(stdout,"A%0.6d %s %s %s %s %d\n",
            x->atl.code,x->atl.name,x->atl.surname,x->atl.cate,x->atl.date,x->atl.hours);
    ex_print(x->head,x->tail,stdout);
}

/*----------------------------------------------------------------------------------------------------------------------
 *  SEARCH BY SURNAME
 */

void searchBySurname(Atl_Label wrap_atl,menuE command)
{
    char s[maxLength];
    atLink toDisplay;

    printf("Type Surname: "); scanf("%s",s);
    toDisplay = at_linearSearch(wrap_atl->head, wrap_atl->tail->next, s, 0, command);
    if(toDisplay==NULL){printf("\nNot Found"); return;}
    atl_print(wrap_atl, stdout);
}

/*--------------------------------------------------------------------------------------------------------------------
 * LINEAR SEARCH
 */

atLink at_linearSearch(atLink l, atLink r, char *key_c, int key_i, menuE command)
{
    atLink i;
    for(i=l;i!=r;i=i->next)
        switch(command) {
            case m_exUpdate:
                if(fullNameMatches(key_c,i))
                    return i;
                break;
            case m_code:
                if(codeMatches(key_i,i))
                    return i;
                break;
            case m_surname:
                if(surnameMatches(key_c,i))
                    return i;
            case m_del: //we need the precedent node of the one we want to delete
                if(i->next!=NULL)
                    if(codeMatches(key_i,i->next))
                        return i;
                break;
            default: break;
        }
    return NULL;
}

/*--------------------------------------------------------------------------------------------------------------------
 * BOOLEAN SUPPORT FUNCTIONS
 */

bool fullNameMatches(char *s,atLink l)
{
    char fullName[2*maxLength];
    memset(fullName,'\0',2*maxLength);
    strcpy(fullName,l->atl.name);
    strcat(fullName," ");
    strcat(fullName,l->atl.surname);
    return (strcmp(s,fullName)==0);
}

bool codeMatches(int n,atLink l)
{return (n==l->atl.code);}

bool surnameMatches(char *s,atLink l)
{
    int length=strlen(s);
    return (strncmp(s,l->atl.surname,(size_t)length)==0);
}


/*--------------------------------------------------------------------------------------------------------------------
 * EXERCISES PART
 */

void acquireExPlan(Atl_Label wrap_atl,Ex_Label wrap_ex,const char *path)
{
    atLink toLoad;
    int code;
    char *completePath;

    printf("Write the numerical code: "); scanf("%d",&code);

    toLoad = at_linearSearch(wrap_atl->head, wrap_atl->tail, NULL, code, m_code);
    completePath = extractPath(path,code);

    loadExercises(&toLoad->head,&toLoad->tail,wrap_ex,completePath);

}

char* extractPath(const char *path,int code)
{
    int l;
    char buffer[maxLength],*s;

    //copying the path in s
    sprintf(buffer,"A%.4d",code);
    l=strlen(path)+strlen(buffer)+5;

    s = malloc(l*sizeof(char)); memset(s,'\0',(size_t) l);

    strcpy(s,path); strcat(s,buffer); strcat(s,".txt");

    return s;
}

void  updateEx(Atl_Label *atlLabel,Ex_Label *exercises)
{
    bool repeat;
    int code;
    char choice;
    atLink atFound;


        printf("Who's going to change his exercise plan? A<code> : ");
        scanf("%d",&code); printf("\n");

        atFound = at_linearSearch((*atlLabel)->head, (*atlLabel)->tail, 0, code, m_code);

        if(atFound!=NULL){
            if(atFound->head==NULL){
                printf("Athlete doesn't have an exercise plan!\n");
            }else{
                printf("That's the exercise plan of: %d\n",code);
                ex_print(atFound->head,atFound->tail,stdout);
            }
            printf("Do you want to edit it?: Y/N");
            do{
                scanf("%c",&choice);
                if(choice=='Y'||choice=='N') repeat=false; else repeat=true;
                printf("\n");
            }while(repeat);

            if(choice=='N') return;

            editEx(&atFound->head,&atFound->tail,*exercises);

        }else{printf("Athlete not found\n");}


}

/*--------------------------------------------------------------------------------------------------------------------*/

void editEx(exLink *head,exLink *tail,Ex_Label wrap_ex){
    editE choice;
    while(1) {
        do {
            printf("1-add an exercise\n");
            printf("2-delete an exercise\n");
            printf("3-edit set of an exercise\n");
            printf("4-edit rep of an exercise\n");
            printf("5-read actual ex. plan\n");
            printf("6-know something more about an exercise\n");
            printf("7-return to precedent menu\n");
            scanf("%d", &choice);
        } while (choice < e_add || choice >= e_err);

        switch (choice) {
            case e_add:    ex_addNode(wrap_ex,tail);     break;
            case e_del:    ex_removeNode(head);          break;
            case e_set:    editSet(*head);               break;
            case e_rep:    editRep(*head);               break;
            case e_read:   ex_print(*head,*tail,stdout); break;
            case e_know:   exDetails(wrap_ex);
            case e_return: return;
            case e_err:    printf("something went wrong\n"); return;
        }
    }
}

/*--------------------------------------------------------------------------------------------------------------------
 * UPDATE FILE
 */

int atl_updateFile(const char *fileName, Atl_Label wrap_atl)
{
    FILE *fp;
    if((fp=fopen(fileName,"w"))==NULL) return -1;
    atl_print(wrap_atl, fp);
    fclose(fp);
}