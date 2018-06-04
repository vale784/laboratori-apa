#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*--------------------------------------------------------------------------------------------------------------------*/
#define fileName "atleti.txt"
#define maxLength 25
#define Err_filename (-1)
#define Err_fileRead (-2)
#define Err_menuSwitch (-3)
#define Err_filename2 (-4)
/*--------------------------------------------------------------------------------------------------------------------*/

#define p_swap(A,B) {void *p=(A); (A)=(B); (B)=p;}

/*--------------------------------------------------------------------------------------------------------------------*/
typedef struct Atl_s {
    char *name,*surname,*cate,*date;
    int hours,code; //>=0
} atlS;

typedef struct Node_Atl *link;
typedef struct Node_Atl {
    atlS atl;
    link next;
};

/*--------------------------------------------------------------------------------------------------------------------*/

//do not edit the last element of the enum
typedef enum Menu_e {m_print,m_filePrint,m_cate,m_hours,m_code,
    m_surname,m_new,m_del,m_exit,m_errMenu} menuE;
typedef int bool;

/*--------------------------------------------------------------------------------------------------------------------*/
//DECLARATIONS

//file reading part
int readFile(FILE *fp,link *head,int nAt,link *tail);

//menu handling part
char* menuDisplay(char options[][maxLength]);
menuE matchInput(char options[][maxLength],char *choice);
bool acceptableChoice(menuE ch);
void menuSwitch(menuE ch,link *head,link *tail);

//print function (link to first to print, link to right to stop)
void print(link l,link r,FILE *fp);

//sort by...
void cateDisplay(link head,menuE command);
void insertionSort(link *list,int l,int r,menuE command);
bool less(link element1,link element2,menuE command);

//update hours
void updateHours(link head,link tail,menuE command);

//search by...
void searchByCode(link head,link tail,menuE command);
void searchBySurname(link head,link tail,menuE command);

//search functions (linear one)
link linearSearch(link l,link r,char *key_c,int key_i,menuE command);
bool fullNameMatches(char *s,link l);
bool codeMatches(int n,link l);
bool surnameMatches(char *s,link l);

//node editing functions
void newTail(link *tail,FILE *fp);
link newNode(link *node,FILE **fp);
void removeNode(link *head,link tail,menuE command);
void delNode(link prNode);
link delHead(link head);

//update file
void updateFile(FILE *fp,link head,link tail);

/*--------------------------------------------------------------------------------------------------------------------*/
//MAIN

int main(void)
{
    FILE *fp;
    link head=NULL,tail=NULL;
    int nAt;
    char *choice;
    char options[m_errMenu][maxLength]={"read","export","category","hours","code","surname","new","delete","exit"};
    menuE m_choice;

    if((fp=fopen(fileName,"r"))==NULL) return Err_filename;
    fscanf(fp,"%d\n",&nAt);
    if(readFile(fp,&head,nAt,&tail)) return Err_fileRead;
    fclose(fp);

    do {
        choice = menuDisplay(options);
        m_choice = matchInput(options,choice);
        if(acceptableChoice(m_choice))
            menuSwitch(m_choice,&head,&tail);

    }while(m_choice!=m_exit);

    if((fp=fopen(fileName,"w"))==NULL) return Err_filename2;
    updateFile(fp,head,tail);
    fclose(fp);

    return 0;
}

/*--------------------------------------------------------------------------------------------------------------------*/
/*FILE READING*/

int readFile(FILE *fp,link *head,int nAt,link *tail)
{
    link x = (*tail);
    int cnt;

    for(*head = x = newNode(&x,&fp),cnt=1;(cnt<=nAt);x = x->next,cnt++) {
        if(cnt==nAt) {*tail=x; x->next = NULL;}
        else         {x->next=newNode(&x,&fp);}
    }
    return 0;
}

/*--------------------------------------------------------------------------------------------------------------------*/
/*MENU*/

char* menuDisplay(char options[][maxLength])
{
    char choice[maxLength],*s;

    printf("Read or Export List: '%s'/'%s'\n",options[m_print], options[m_filePrint]);
    printf("Display for Category: '%s'\n",    options[m_cate]);
    printf("Update weekly hours: '%s'\n",     options[m_hours]);
    printf("Code Search: '%s'\n",             options[m_code]);
    printf("Surname Search: '%s'\n",          options[m_surname]);
    printf("Add New: '%s'\n",                 options[m_new]);
    printf("Delete One: '%s'\n",              options[m_del]);
    printf("Kill the Program: '%s'\n",        options[m_exit]);

    scanf("%s",choice);
    printf("\n");
    s = malloc((strlen(choice)+1)*sizeof(char));
    strcpy(s,choice);

    return s;
}

menuE matchInput(char options[][maxLength],char *choice)
{
    menuE i;
    for(i = (menuE) 0;i < m_errMenu; i++)
        if(strcmp(options[i],choice)==0)
            return i;
    return m_errMenu;
}

bool acceptableChoice(menuE ch)
{return ((ch >= (menuE)0) && (ch < m_errMenu));}

void menuSwitch(menuE ch,link *head,link *tail)
{
    switch(ch){
        case m_print: print(*head,(*tail)->next,stdout); break;
        case m_filePrint: print(*head,(*tail)->next,NULL); break;
        case m_cate: cateDisplay(*head,ch); break;
        case m_hours: updateHours(*head,*tail,ch); break;
        case m_code: searchByCode(*head,*tail,ch); break;
        case m_surname: searchBySurname(*head,*tail,ch); break;
        case m_new: newTail(tail,stdin); break;
        case m_del: removeNode(head,*tail,ch); break;
        case m_exit: exit(0);
        case m_errMenu: exit(Err_menuSwitch);
    }
}

/*--------------------------------------------------------------------------------------------------------------------*/
/*PRINT AND FILE PRINT*/

void print(link l,link r,FILE *fp)
{
    char supp[maxLength];
    link x;
    if(fp==NULL) {
        printf("File name: "); scanf("%s",supp);
        if((fp==fopen(supp,"w"))==NULL) return;
    }
    for(x=l;(x!=((r==NULL)?(NULL):(r->next)));x=x->next)
        fprintf(fp,"A%d %s %s %s %s %d\n",x->atl.code,x->atl.name,
                x->atl.surname,x->atl.cate,x->atl.date,x->atl.hours);

    if(fp!=stdout) fclose(fp);
}

/*--------------------------------------------------------------------------------------------------------------------*/
//LINEAR SEARCH
link linearSearch(link l,link r,char *key_c,int key_i,menuE command)
{
    link i;
    for(i=l;i!=r;i=i->next)
        switch(command) {
            case m_hours:
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
                if(codeMatches(key_i,i->next))
                    return i;
                break;
            default: break;
        }
    return NULL;
}

bool fullNameMatches(char *s,link l)
{
    char fullName[2*maxLength];
    strcpy(fullName,l->atl.name);
    strcat(fullName,l->atl.surname);
    return (strcmp(s,fullName)==0);
}

bool codeMatches(int n,link l)
{return (n==l->atl.code);}

bool surnameMatches(char *s,link l)
{
    int length=strlen(s);
    return (strncmp(s,l->atl.surname,(size_t)length)==0);
}

/*--------------------------------------------------------------------------------------------------------------------*/
//CATEGORY DISPLAY

void cateDisplay(link head,menuE command)
{
    link *list,x;
    int cnt,i;

    //number of athletes and list creation
    for(cnt=1,x=head;(x->next != NULL);cnt++,x=x->next);
    list = malloc(cnt*sizeof(*list));
    for(i=0,x=head;(x->next != NULL);i++,x=x->next)
        list[i]=x;

    insertionSort(list,0,cnt,command); //sorting by category
    //printing sorted list-array
    for(i=0;i<cnt;i++)
        printf("A%d %s %s %s %s %d\n",list[i]->atl.code,list[i]->atl.name,list[i]->atl.surname,
               list[i]->atl.cate,list[i]->atl.date,list[i]->atl.hours);
}

void insertionSort(link *list,int l,int r,menuE command)
{
    int i,k;
    link supp;
    //placing the minor element in first position
    for(i=r-1;i>l;i--)
        if(less(list[i],list[i-1],command))
        p_swap(list[i],list[i-1])

    for(i=l;i<r;i++) {
        supp = list[i];
        for(k=i;less(list[k],list[k-1],command);k--)
            list[k]=list[k-1];
        list[k]=supp;
    }
}

bool less(link element1,link element2,menuE command)
{
    switch(command)
    {
        case m_cate: return strcmp(element1->atl.cate, element2->atl.cate) <= 0;
        default: printf("Case not handled yet\n"); return 0;
    }
}

/*--------------------------------------------------------------------------------------------------------------------*/
//UPDATE HOURS

void updateHours(link head,link tail,menuE command)
{
    char fullName[2*maxLength];
    int newHours;
    link toUpdate;

    printf("Type <name> <surname>: "); gets(fullName);
    toUpdate = linearSearch(head,tail,fullName,0,command);
    if(toUpdate==NULL){printf("\nNot Found"); return;}
    printf("\nNew amount of hours: "); scanf("%d",&newHours);
    toUpdate->atl.hours=newHours;
}

/*--------------------------------------------------------------------------------------------------------------------*/
//SEARCH BY...

void searchByCode(link head,link tail,menuE command)
{
    int code;
    link toDisplay;

    printf("Type code: "); scanf("%d",&code);
    toDisplay = linearSearch(head,tail,NULL,code,command);
    if(toDisplay==NULL){printf("\nNot Found"); return;}
    print(toDisplay,toDisplay->next,stdout);
}

void searchBySurname(link head,link tail,menuE command)
{
    char s[maxLength];
    link toDisplay;

    printf("Type Surname: "); scanf("%s",s);
    toDisplay = linearSearch(head,tail,s,0,command);
    if(toDisplay==NULL){printf("\nNot Found"); return;}
    print(toDisplay,toDisplay->next,stdout);
}

/*--------------------------------------------------------------------------------------------------------------------*/
//NODE EDITING

void newTail(link *tail,FILE *fp)
{*tail = newNode(tail,&fp);}

link newNode(link *node,FILE **fp)
{
    link newNode;
    char name[maxLength],surname[maxLength],cate[maxLength],date[maxLength];

    newNode = malloc(sizeof(*newNode));

    if(*fp == stdin) {
        printf("Type New Athlete in format: \n");
        printf("A<code> <name> <surname> <category> <date of birth> <weekly training hours>\n");
    }

    fscanf(*fp,"A%d %s %s %s %s %d\n",&newNode->atl.code,name,surname,cate,date,&newNode->atl.hours);
    (newNode->atl.name) = (char *) malloc((strlen(name)+1)*sizeof(char)); strcpy(newNode->atl.name,name);
    (newNode->atl.surname) = (char *) malloc((strlen(surname)+1)*sizeof(char)); strcpy(newNode->atl.surname,surname);
    (newNode->atl.cate) = (char *) malloc((strlen(cate)+1)*sizeof(char)); strcpy(newNode->atl.cate,cate);
    (newNode->atl.date) = (char *) malloc((strlen(date)+1)*sizeof(char)); strcpy(newNode->atl.date,date);

    if(*node!=NULL)
    {newNode->next = (*node)->next; (*node)->next = newNode;}

    return newNode;
}

void removeNode(link *head, link tail,menuE command)
{
    int code;
    link toRemove;

    printf("Write the Code of the Athlete You Want to Remove: "); scanf("A%d",&code);
    toRemove = linearSearch(*head,tail,NULL,code,command); //returns the precedent
    if(toRemove==NULL) {printf("\nNot Found"); return;}
    if(toRemove==*head) *head = delHead(*head);

    delNode(toRemove);
}

void delNode(link prNode)
{
    link toDelete = prNode->next;
    prNode->next = toDelete->next;
    free(toDelete);
}

link delHead(link head){
    link newHead = head->next; free(head);
    return newHead;
}

/*--------------------------------------------------------------------------------------------------------------------*/
//UPDATE FILE

void updateFile(FILE *fp,link head,link tail)
{print(head,tail->next,fp);}