#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*--------------------------------------------------------------------------------------------------------------------*/
#define atlFile_Name "atleti.txt"
#define exFile_Name "esercizi.txt"
#define pathEx "Exercises\\"
#define maxLength 25
#define maxEx_len 50
#define Err_filename (-1)
#define Err_fileRead1 (-2)
#define Err_menuSwitch (-3)
#define Err_filename2 (-4)
#define Err_filename3 (-5)
#define Err_fileRead2 (-6)
#define DBG 0

/*--------------------------------------------------------------------------------------------------------------------*/
//MACROS

#define p_swap(A,B) {void *p=(A); (A)=(B); (B)=p;}

/*--------------------------------------------------------------------------------------------------------------------*/

typedef struct Node_Athlete *atLink;
typedef struct Node_Exercise_Plan *exLink;

struct Node_Athlete {
    struct {
        char *name,*surname,*cate,*date;
        int hours,code; //>=0
    } atl;
    atLink next;
    exLink head;
    exLink tail;
};

typedef struct Exercise_Type{
    char *exName,*excat,*extype;
}exS;

struct Node_Exercise_Plan{
    exS *exercise;
    int  set,rep;
    exLink next;
};

typedef struct Atl_Wrapper{
    int nAthletes;
    atLink head,tail;
}*Atl_Label;

typedef struct Exercise_Wrapper{
    exS *vettEx;
    int  nExer;
}*Ex_Label;

/*--------------------------------------------------------------------------------------------------------------------*/

//do not edit the last element of the enum
typedef enum Menu_e {m_print,m_filePrint,m_cate,m_hours,m_code,
    m_surname,m_new,m_del,m_exUpdate,m_exit,m_errMenu} menuE;
typedef int bool;

/*--------------------------------------------------------------------------------------------------------------------*/
//DECLARATIONS

//file reading part
void readAllFiles(Atl_Label *atlLabel, Ex_Label *exLabel);
int  readAtl(FILE *fp, atLink *head, int nAt, atLink *tail);
int  readExercises(FILE *fp, Ex_Label *exercises);

//menu handling part
char* menuDisplay(void);
menuE matchInput(char *choice);
bool  acceptableChoice(menuE ch);
void  menuSwitch(menuE ch,Atl_Label *atlLabel,Ex_Label *exLabel);

//print function (link to first to print, link to right to stop)
void print(atLink l,atLink r,FILE *fp);

//sort by...
void cateDisplay(Atl_Label atlLabel,menuE command);
void insertionSort(atLink *list,int l,int r,menuE command);
bool less(atLink element1,atLink element2,menuE command);

//update hours
void updateHours(atLink head,atLink tail,menuE command);

//search by...
void searchByCode(atLink head,atLink tail,menuE command);
void searchBySurname(atLink head,atLink tail,menuE command);

//search functions (linear one)
atLink linearSearch(atLink l,atLink r,char *key_c,int key_i,menuE command);
bool   fullNameMatches(char *s,atLink l);
bool   codeMatches(int n,atLink l);
bool   surnameMatches(char *s,atLink l);

//node editing functions
void   at_newTail(atLink *tail, FILE *fp);
atLink at_newNode(atLink *node, FILE **fp);
void   at_removeNode(atLink *head, atLink tail, menuE command);
void   at_delNode(atLink prNode);
atLink at_delHead(atLink *head);

//exercise handling
void  exerciseLoad(Atl_Label *atlLabel,Ex_Label *exercises, const char *path,menuE command);
char* extractPath(const char *path,int code);
void  explanLoad(FILE *fp,atLink atNode,Ex_Label exLabel);
exS*  matchEx(char *s,Ex_Label exLabel);
bool  goAhead(void);
void explanDisplay(exLink head,exLink tail,FILE *fp);
exLink ex_linearSearch(exLink head,exLink tail,char *exName);
char *getexName(void);
bool exMatches(char *ex1,char *ex2);
bool wantExport(void);


//update file
void updateFile(const char *fileName,atLink head,atLink tail);

/*--------------------------------------------------------------------------------------------------------------------*/
//MAIN

const char options[m_errMenu][maxLength]=
        {"read","export","category","hours","code","surname","new","delete","exercise","exit"};

int main(void)
{
    atLink head=NULL,tail=NULL;
    Ex_Label exLabel;
    Atl_Label atlLabel;

    char *choice;
    menuE m_choice;

    readAllFiles(&atlLabel, &exLabel);

    do {
        choice   = menuDisplay();
        m_choice = matchInput(choice);
        if(acceptableChoice(m_choice))
            menuSwitch(m_choice,&atlLabel,&exLabel);

        printf("\n");

    }while(m_choice != m_exit);

    updateFile(atlFile_Name,head,tail);

    return 0;
}

/*--------------------------------------------------------------------------------------------------------------------*/
/*FILE READING*/

void readAllFiles(Atl_Label *atlLabel, Ex_Label *exLabel)
{
    FILE *fp;

    *atlLabel = malloc(sizeof(**atlLabel));
    (*atlLabel)->head = (*atlLabel)->tail = NULL;

    if((fp=fopen(atlFile_Name,"r"))==NULL)  exit(Err_filename);
    fscanf(fp,"%d\n",&(*atlLabel)->nAthletes);
    if(readAtl(fp,&(*atlLabel)->head,(*atlLabel)->nAthletes,&(*atlLabel)->tail))
        exit(Err_fileRead1);
    fclose(fp);

    if((fp=fopen(exFile_Name,"r"))==NULL)  {exit(Err_filename3);}
    if(readExercises(fp, exLabel))         {exit(Err_fileRead2);}
    fclose(fp);
}

int readAtl(FILE *fp, atLink *head, int nAt, atLink *tail)
{
    atLink x = (*tail);
    int cnt;
    for(*head = x = at_newNode(&x, &fp),cnt=1;(cnt<=nAt);x = x->next,cnt++) {
        if(cnt==nAt) {*tail=x; x->next = NULL;}
        else         {x->next= at_newNode(&x, &fp);}
    }
    return 0;
}

int readExercises(FILE *fp, Ex_Label *exercises)
{
    int E,i;
    char exName[maxEx_len],exCat[maxEx_len],exType[maxEx_len];
    Ex_Label supp = *exercises = malloc(sizeof(*supp));

    fscanf(fp,"%d",&E);
    supp->nExer=E;
    supp->vettEx = malloc(E * sizeof(*supp->vettEx));

    for(i=0;(i<E)&&(fscanf(fp,"%s %s %s",exName,exCat,exType)==3);i++){
        supp->vettEx[i].exName = strdup(exName);
        supp->vettEx[i].excat  = strdup(exCat);
        supp->vettEx[i].extype = strdup(exType);
    }
    return 0;
}

/*--------------------------------------------------------------------------------------------------------------------*/
/*MENU*/

char* menuDisplay(void)
{
    char choice[maxLength],*s;

    printf("Read or Export List: '%s'/'%s'\n",options[m_print], options[m_filePrint]);
    printf("Display for Category: '%s'\n",    options[m_cate]);
    printf("Update weekly hours: '%s'\n",     options[m_hours]);
    printf("Code Search: '%s'\n",             options[m_code]);
    printf("Surname Search: '%s'\n",          options[m_surname]);
    printf("Add New: '%s'\n",                 options[m_new]);
    printf("Delete One: '%s'\n",              options[m_del]);
    printf("Update Exercise plan: %s\n",      options[m_exUpdate]);
    printf("Kill the Program: '%s'\n",        options[m_exit]);

    scanf("%s",choice);
    printf("\n");
    s = malloc((strlen(choice)+1)*sizeof(char));
    strcpy(s,choice);

    return s;
}

menuE matchInput(char *choice)
{
    menuE i;
    for(i = (menuE) 0;i < m_errMenu; i++)
        if(strcmp(options[i],choice)==0)
            return i;
    return m_errMenu;
}

bool acceptableChoice(menuE ch)
{return ((ch >= (menuE)0) && (ch < m_errMenu));}

void menuSwitch(menuE ch,Atl_Label *atlLabel,Ex_Label *exLabel)
{
    atLink head=(*atlLabel)->head,tail=(*atlLabel)->tail;
    switch(ch){
        case m_print:     print(head,tail->next,stdout);                break;
        case m_filePrint: print(head,tail->next,NULL);                  break;
        case m_cate:      cateDisplay(*atlLabel,ch);                    break;
        case m_hours:     updateHours(head,tail,ch);                    break;
        case m_code:      searchByCode(head,tail,ch);                   break;
        case m_surname:   searchBySurname(head,tail,ch);                break;
        case m_new:
            at_newTail(&tail, stdin);                         break;
        case m_del:
            at_removeNode(&head, tail, ch);                    break;
        case m_exUpdate:  exerciseLoad(atlLabel,exLabel,pathEx,ch);     break;
        case m_exit:      exit(0);
        case m_errMenu:   exit(Err_menuSwitch);
    }
}

/*--------------------------------------------------------------------------------------------------------------------*/
/*PRINT AND FILE PRINT*/

void print(atLink l,atLink r,FILE *fp)
{
    char supp[maxLength];
    atLink x;
    if(fp==NULL) {
        printf("File name: "); scanf("%s",supp);
        if((fp=fopen(supp,"w"))==NULL) return;
    }
    for(x=l;(x!=((r==NULL)?(NULL):(r->next)));x=x->next)
        fprintf(fp,"A%0.6d %s %s %s %s %d\n",x->atl.code,x->atl.name,
                x->atl.surname,x->atl.cate,x->atl.date,x->atl.hours);

    fprintf(fp,"\n");
    if(fp!=stdout) fclose(fp);
}

/*--------------------------------------------------------------------------------------------------------------------*/
//LINEAR SEARCH
atLink linearSearch(atLink l,atLink r,char *key_c,int key_i,menuE command)
{
    atLink i;
    for(i=l;i!=r;i=i->next)
        switch(command) {
            case m_hours: case m_exUpdate:
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

/*--------------------------------------------------------------------------------------------------------------------*/
//CATEGORY DISPLAY

void cateDisplay(Atl_Label atlLabel,menuE command)
{
    atLink *list,x;
    int i,nAth=atlLabel->nAthletes;

    list = malloc(nAth*sizeof(*list));

    //number of athletes and list creation
    for(x = atlLabel->head,i=0;i<nAth;x = x->next, i++)
        list[i]=x;

    insertionSort(list,0,nAth,command); //sorting by category

    //printing sorted list-array
    for(i=0;i<nAth;i++)
        printf("A%0.6d %s %s %s %s %d\n",list[i]->atl.code,list[i]->atl.name,
               list[i]->atl.surname,list[i]->atl.cate,list[i]->atl.date,list[i]->atl.hours);

}

void insertionSort(atLink *list,int l,int r,menuE command)
{
    int i,k;
    atLink supp;
    //placing the minor element in first position
    for(i=r-1;i>l;i--)
        if(less(list[i],list[i-1],command))
        p_swap(list[i],list[i-1])

    for(i=l+2;i<r;i++) {
        supp = list[i];
        for(k=i;less(supp,list[k-1],command);k--)
            list[k]=list[k-1];
        list[k]=supp;
    }
}

bool less(atLink element1,atLink element2,menuE command)
{
    switch(command)
    {
        case m_cate: return strcmp(element1->atl.cate, element2->atl.cate) < 0;
        default: printf("Case not handled yet\n"); return 0;
    }
}

/*--------------------------------------------------------------------------------------------------------------------*/
//UPDATE HOURS

void updateHours(atLink head,atLink tail,menuE command)
{
    char fullName[2*maxLength],name[maxLength],surname[maxLength];
    int newHours;
    atLink toUpdate;

    memset(name,'\0',maxLength); memset(surname,'\0',maxLength);
    printf("Type <name> <surname>: "); scanf("%s %s",name,surname);
    strcpy(fullName,name); strcat(fullName," "); strcat(fullName,surname);

    toUpdate = linearSearch(head,tail,fullName,0,command);
    if(toUpdate==NULL){printf("\nNot Found"); return;}

    printf("\nNew amount of hours: "); scanf("%d",&newHours);
    toUpdate->atl.hours=newHours;
}

/*--------------------------------------------------------------------------------------------------------------------*/
//SEARCH BY...

void searchByCode(atLink head,atLink tail,menuE command)
{
    int code;
    atLink toDisplay;

    printf("Type code: "); scanf("%d",&code);
    toDisplay = linearSearch(head,tail->next,NULL,code,command);
    if(toDisplay==NULL){printf("\nNot Found"); return;}
    print(toDisplay,toDisplay,stdout);
}

void searchBySurname(atLink head,atLink tail,menuE command)
{
    char s[maxLength];
    atLink toDisplay;

    printf("Type Surname: "); scanf("%s",s);
    toDisplay = linearSearch(head,tail,s,0,command);
    if(toDisplay==NULL){printf("\nNot Found"); return;}
    print(toDisplay,toDisplay,stdout);
}

/*--------------------------------------------------------------------------------------------------------------------*/
//NODE EDITING

void at_newTail(atLink *tail, FILE *fp)
{getchar(); *tail = at_newNode(tail, &fp);}

atLink at_newNode(atLink *node, FILE **fp)
{
    atLink newNode;
    char name[maxLength],surname[maxLength],cate[maxLength],date[maxLength];

    newNode = malloc(sizeof(*newNode));

    if(*fp == stdin) {
        printf("Type New Athlete in format: \n");
        printf("A<code> <name> <surname> <category> <date of birth> <weekly training hours>\n");
    }

    if(*fp!=stdin)
        fscanf(*fp,"A%d %s %s %s %s %d\n",&newNode->atl.code,name,surname,cate,date,&newNode->atl.hours);
    else
        scanf("A%d %s %s %s %s %d",&newNode->atl.code,name,surname,cate,date,&newNode->atl.hours);
    #if DBG
    (newNode->atl.name)    = (char *) malloc((strlen(name)+1)*sizeof(char));    strcpy(newNode->atl.name,name);
    (newNode->atl.surname) = (char *) malloc((strlen(surname)+1)*sizeof(char)); strcpy(newNode->atl.surname,surname);
    (newNode->atl.cate)    = (char *) malloc((strlen(cate)+1)*sizeof(char));    strcpy(newNode->atl.cate,cate);
    (newNode->atl.date)    = (char *) malloc((strlen(date)+1)*sizeof(char));    strcpy(newNode->atl.date,date);
    #else
    (newNode->atl.name)    = strdup(name);
    (newNode->atl.surname) = strdup(surname);
    (newNode->atl.cate)    = strdup(cate);
    (newNode->atl.date)    = strdup(date);
    #endif
    if(*node!=NULL)
    {newNode->next = (*node)->next; (*node)->next = newNode;}

    return newNode;
}

void at_removeNode(atLink *head, atLink tail, menuE command)
{
    int code;
    atLink toRemove;

    printf("Write the Code of the Athlete You Want to Remove: "); scanf("\nA%d",&code);
    if(!codeMatches(code,*head)) {
        toRemove = linearSearch(*head, tail->next, NULL, code, command); //returns the precedent
        if (toRemove == NULL) { printf("\nNot Found"); return; }
        at_delNode(toRemove);

    } else *head = at_delHead(head);
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

/*--------------------------------------------------------------------------------------------------------------------*/
//UPDATE FILE

void updateFile(const char *fileName,atLink head,atLink tail)
{
    FILE *fp;
    if((fp=fopen(fileName,"w"))==NULL) exit(Err_filename2);
    print(head,tail->next,fp);
    fclose(fp);
}

/*--------------------------------------------------------------------------------------------------------------------*/
//EXERCISE HANDLING

void  exerciseLoad(Atl_Label *atlLabel,Ex_Label *exercises, const char *path,menuE command)
{
    bool end=0;
    FILE *fp;
    char fullName[2*maxLength],*fullPath,*exName;
    atLink atFound;
    exLink exFound;
    getchar();
    while(!end){

        printf("Who's going to change his exercise plan? <name> <surname> : ");
        gets(fullName); printf("\n");

        atFound = linearSearch((*atlLabel)->head,(*atlLabel)->tail,fullName,0,command);

        if(atFound!=NULL){
            fullPath = extractPath(path,atFound->atl.code);
            if((fp=fopen(fullPath,"r"))==NULL){
                printf("The file doesn't exist\n");
            }else{
                explanLoad(fp,atFound,*exercises);
                fclose(fp);
                explanDisplay(atFound->head,atFound->tail,stdout);
                do {
                    exName  = getexName();
                    exFound = ex_linearSearch(atFound->head, atFound->tail, exName);
                    printf("New <set> <repetitions>: "); scanf("%d %d",&exFound->set,&exFound->rep);
                }while(exFound==NULL);

                explanDisplay(atFound->head,atFound->tail,stdout);

                if(wantExport()) {
                    fp = fopen(fullPath,"w");
                    explanDisplay(atFound->head,atFound->tail,fp);
                }
            }
        }else{printf("Athlete not found\n");}

        if(!goAhead()) end=!end;
    }
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

void explanLoad(FILE *fp,atLink atNode,Ex_Label exLabel)
{
    exLink x,supp=NULL;
    char exName[maxEx_len];

    x = atNode->head = malloc(sizeof(*atNode->head));
    for( ;fscanf(fp,"%s %d %d",exName,&x->set,&x->rep)!=EOF;x = x->next){
        x->exercise = matchEx(exName,exLabel);
        x->next     = malloc(sizeof(*atNode->head));
        supp = x;
    }
    atNode->tail = supp;

    if(supp!=NULL) {
        supp->next = NULL; free(x);
    }
    printf("Exercise plan loaded\n");
}

exS* matchEx(char *s,Ex_Label exLabel)
{
    int i;
    for(i=0;i<exLabel->nExer;i++)
        if(strcmp(s,exLabel->vettEx[i].exName)==0)
            return &exLabel->vettEx[i];
    return NULL;
}

bool goAhead(void)
{
    char choice;
    do{
        printf("Do you want to keep edit something? Y/N: "); scanf("%c",&choice); printf("\n");
    }while(choice!='Y'&&choice!='N');
    if(choice=='Y') return 1;
    return 0;
}

void explanDisplay(exLink head,exLink tail,FILE *fp)
{
    exLink x;
    for(x=head;x!=tail->next;x=x->next)
        fprintf(fp,"%s %d %d\n",x->exercise->exName,x->set,x->rep);
}

exLink ex_linearSearch(exLink head,exLink tail,char *exName)
{
    exLink i;
    for(i=head;i!=tail->next;i=i->next)
        if(exMatches(exName,i->exercise->exName))
            return i;
    return NULL;
}

char *getexName(void)
{
    char buf[maxLength];
    printf("Write the name of the exercise of which you want to change set and repetition: "); scanf("%s",buf);
    return &buf[0];
}

bool exMatches(char *ex1,char *ex2)
{return strcmp(ex1,ex2)==0;}

bool wantExport(void)
{
    char choice;
    do {
        printf("Do you want to save changes? (on the file) Y/N: ");
        scanf("%c", &choice);
        printf("\n");
    }while(choice!='Y'&&choice!='N');

    return (choice=='Y')? 0:1;
}