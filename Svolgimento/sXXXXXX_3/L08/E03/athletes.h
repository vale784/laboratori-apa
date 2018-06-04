#ifndef L08_E03_ATHLETES_H
#define L08_E03_ATHLETES_H

#include "exercises.h"

#include <stdio.h>
#include <stdbool.h>

/*--------------------------------------------------------------------------------------------------------------------*/

typedef struct Node_Athlete *atLink;
typedef struct Atl_Wrapper *Atl_Label;

typedef enum Menu_e {m_print,m_filePrint,m_acqExPlan,m_code,
    m_surname,m_new,m_del,m_exUpdate,m_exit,m_errMenu} menuE;

/*--------------------------------------------------------------------------------------------------------------------*/

//file reading functions
int  readAtl(FILE *fp, Atl_Label wrap_atl);
//print athlete
void atl_print(Atl_Label wrap_atl, FILE *fp);

/*--------------------------------------------------------------------------------------------------------------------*/

//new node (insert in tail or in any place)
void   at_newTail(Atl_Label wrap_atl, FILE *fp);
atLink at_newNode(atLink node, FILE *fp);

//deletion functions
void   at_removeNode(Atl_Label wrap_atl, menuE command);
void   at_delNode(atLink prNode);
atLink at_delHead(atLink *head);

/*--------------------------------------------------------------------------------------------------------------------*/

//search by code
void searchByCode(Atl_Label wrap_atl,menuE command);
//search by surname
void searchBySurname(Atl_Label wrap_atl,menuE command);
//linear search
atLink at_linearSearch(atLink l, atLink r, char *key_c, int key_i, menuE command);

/*--------------------------------------------------------------------------------------------------------------------*/

//boolean functions for searches
bool   fullNameMatches(char *s,atLink l);
bool   codeMatches(int n,atLink l);
bool   surnameMatches(char *s,atLink l);

/*--------------------------------------------------------------------------------------------------------------------*/

//for exercises part
void acquireExPlan(Atl_Label wrap_atl,Ex_Label wrap_ex,const char *path);
char* extractPath(const char *path,int code);
void  updateEx(Atl_Label *atlLabel,Ex_Label *exercises);

/*--------------------------------------------------------------------------------------------------------------------*/

//exercise plan edit
void editEx(exLink *head,exLink *tail,Ex_Label wrap_ex);

/*--------------------------------------------------------------------------------------------------------------------*/

//file update
int atl_updateFile(const char *fileName, Atl_Label wrap_atl);

#endif //L08_E03_ATHLETES_H
