#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "athletes.h"
/*--------------------------------------------------------------------------------------------------------------------*/

#define atlFile_Name "atleti.txt"
#define exFile_Name "esercizi.txt"
#define pathEx "Exercises\\"

/*--------------------------------------------------------------------------------------------------------------------*/
#define MAXL 25
#define wantUpdate false

//ERRORS
#define Err_filename (-1)
#define Err_fileRead1 (-2)
#define Err_menuSwitch (-3)
#define Err_filename2 (-4)
#define Err_filename3 (-5)
#define Err_fileRead2 (-6)

/*--------------------------------------------------------------------------------------------------------------------*/

//file reading part
void readAllFiles(Atl_Label *atlLabel, Ex_Label *exLabel);

//menu handling part
char* menuDisplay(void);
menuE matchInput(char *choice);
bool  acceptableChoice(menuE ch);
void  menuSwitch(menuE ch,Atl_Label *atlLabel,Ex_Label *exLabel);

/*--------------------------------------------------------------------------------------------------------------------*/
//MAIN

const char *options[m_errMenu]=
        {"read","export","exPlan","code","surname","new","delete","exercise","exit"};

int main(void)
{
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

#if wantUpdate==true

    if(atl_updateFile(atlFile_Name,atlLabel)==-1) return Err_filename2;

#endif

    return 0;
}

/*--------------------------------------------------------------------------------------------------------------------*/
/*FILE READING*/

void readAllFiles(Atl_Label *atlLabel, Ex_Label *exLabel)
{
    FILE *fp;

    if((fp=fopen(atlFile_Name,"r"))==NULL)     exit(Err_filename);
    if(readAtl(fp,*atlLabel)==Err_fileRead1)   exit(Err_fileRead1);
    fclose(fp);

    if((fp=fopen(exFile_Name,"r"))==NULL)      exit(Err_filename3);
    if(readExercises(fp, exLabel))             exit(Err_fileRead2);
    fclose(fp);
}

/*--------------------------------------------------------------------------------------------------------------------*/
/*MENU*/

char* menuDisplay(void)
{
    char choice[MAXL],*s;

    printf("Read or Export List: '%s'/'%s'\n",  options[m_print], options[m_filePrint]);
    printf("Acquire an exercise plan: '%s'\n",  options[m_acqExPlan]);
    printf("Code Search: '%s'\n",               options[m_code]);
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

void menuSwitch(menuE ch,Atl_Label *atlLabel,Ex_Label *exLabel)
{
    switch(ch){
        case m_print:     atl_print(*atlLabel, stdout);                 break;
        case m_filePrint: atl_print(*atlLabel, NULL);                   break;
        case m_acqExPlan: acquireExPlan(*atlLabel,*exLabel,pathEx);     break;
        case m_code:      searchByCode(*atlLabel,ch);                   break;
        case m_surname:   searchBySurname(*atlLabel,ch);                break;
        case m_new:       at_newTail(*atlLabel, stdin);                 break;
        case m_del:       at_removeNode(*atlLabel, ch);                 break;
        case m_exUpdate:  updateEx(atlLabel,exLabel);                   break;
        case m_exit:      exit(0);
        case m_errMenu:   exit(Err_menuSwitch);
    }
}