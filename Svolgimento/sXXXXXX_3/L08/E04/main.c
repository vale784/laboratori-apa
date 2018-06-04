#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "athList.h"
/*--------------------------------------------------------------------------------------------------------------------*/
#define atlFile_Name "atleti.txt"
#define exFile_Name  "esercizi.txt"
#define pathEx       "Exercises\\"
/*--------------------------------------------------------------------------------------------------------------------*/
#define MAXL 25
/*--------------------------------------------------------------------------------------------------------------------*/

typedef enum Menu_e {m_print,m_filePrint,m_acqExPlan,
    m_surname,m_new,m_del,m_exUpdate,m_exit,m_errMenu} menuE;

//menu handling part
char* menuDisplay(void);
menuE matchInput(char *choice);
bool  acceptableChoice(menuE ch);
void  menuSwitch(menuE ch,atWrap ATall,ExArr EXall);


void filePrint(atWrap all);
void acquireExPlan(atWrap ATall,ExArr EXall);
char* extractPath(int code);

void searchSurn(atWrap ATall,ExArr EXall);
void delete(atWrap all);
void srUpdate(atWrap ATall,ExArr EXall);

/*--------------------------------------------------------------------------------------------------------------------*/
//MAIN

const char *options[m_errMenu]=
        {"read","export","exPlan","surname","new","delete","exercise","exit"};

int main(void)
{
    atWrap ATall = ATLbuild(atlFile_Name);
    ExArr  EXall = EXAbuild(exFile_Name);

    if(ATall==NULL) return -2;
    if(EXall==NULL) return -3;

    char *choice;
    menuE m_choice;

    do {
        choice   = menuDisplay();
        m_choice = matchInput(choice);
        free(choice);
        if(acceptableChoice(m_choice))
            menuSwitch(m_choice,ATall,EXall);

        printf("\n");

    }while(m_choice != m_exit);

    ATLfree(ATall);
    EXAfree(EXall);

    return 0;
}

/*--------------------------------------------------------------------------------------------------------------------*/
/*MENU*/

char* menuDisplay(void)
{
    char choice[MAXL],*s;

    printf("Read or Export List: '%s'/'%s'\n",  options[m_print], options[m_filePrint]);
    printf("Acquire an exercise plan: '%s'\n",  options[m_acqExPlan]);
    printf("Surname Search: '%s'\n",            options[m_surname]);
    printf("Add New: '%s'\n",                   options[m_new]);
    printf("Delete One: '%s'\n",                options[m_del]);
    printf("Update Exercise plan: '%s'\n",      options[m_exUpdate]);
    printf("Kill the Program: '%s'\n",          options[m_exit]);

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

void menuSwitch(menuE ch,atWrap ATall,ExArr EXall)
{
    switch(ch){
        case m_print:     ATLdisplayAll(ATall,stdout);                  break;
        case m_filePrint: filePrint(ATall);                             break;
        case m_acqExPlan: acquireExPlan(ATall,EXall);                   break;
        case m_surname:   searchSurn(ATall,EXall);                      break;
        case m_new:       ATLnew(ATall,stdin);                          break;
        case m_del:       delete(ATall);                                break;
        case m_exUpdate:  srUpdate(ATall,EXall);                        break;
        case m_exit:      ATLfree(ATall); EXAfree(EXall);               exit(0);
        case m_errMenu:   exit(-1);
    }
}

/*--------------------------------------------------------------------------------------------------------------------*/
void filePrint(atWrap all)
{
    FILE *fp;
    char fName[MAXL];

    printf("file name?\n"); scanf("%s",fName);
    fp=fopen(fName,"r");
    if(fp==NULL){
        printf("file not found\n");
        return;
    }

    ATLdisplayAll(all,fp);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void acquireExPlan(atWrap ATall,ExArr EXall)
{
    int code;
    char *path;
    atLink x;

    printf("code: "); scanf("%d",&code);
    x = ATLsearch(ATall,NULL,code,s_code);
    path = extractPath(code);

    ATLloadPlan(EXall,x,path);
}

char* extractPath(int code)
{
    int l;
    char buffer[maxLength],*s;

    //copying the path in s
    sprintf(buffer,"A%.4d",code);
    l=strlen(pathEx)+strlen(buffer)+5;

    s = malloc(l*sizeof(char)); memset(s,'\0',(size_t) l);

    strcpy(s,pathEx); strcat(s,buffer); strcat(s,".txt");

    return s;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void searchSurn(atWrap ATall,ExArr EXall)
{
    char s[MAXL];

    printf("what's the surname?\n"); scanf("%s",s);
    ATLdisplayOne(ATall,EXall,s,stdout);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void delete(atWrap all)
{
    int code;
    printf("what's the code?\n"); scanf("%d",&code);

    ATLdelete(all,code);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void srUpdate(atWrap ATall,ExArr EXall)
{
    atLink x;
    int code;
    char toEdit[MAXL];

    printf("what's the code?\n"); scanf("%d",&code);
    printf("what's the name of exercise?\n"); scanf("%s",toEdit);

    x = ATLsearch(ATall,NULL,code,s_code);

    ATLeditplan(x,EXall,toEdit);
}
/*--------------------------------------------------------------------------------------------------------------------*/