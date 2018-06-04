#ifndef L08_E03_EXERCISES_H
#define L08_E03_EXERCISES_H

#include <stdio.h>

/*--------------------------------------------------------------------------------------------------------------------*/

typedef struct Node_Exercise_Plan *exLink;
typedef struct Exercise_Wrapper   *Ex_Label;
typedef struct Exercise_Type      *exS;

typedef enum Menu_edit{e_add,e_del,e_set,e_rep,e_read,e_know,e_return,e_err}editE;

/*--------------------------------------------------------------------------------------------------------------------*/

int  readExercises(FILE *fp, Ex_Label *exercises);

/*--------------------------------------------------------------------------------------------------------------------*/

//exercises load and read
void ex_print(exLink head,exLink tail,FILE *fp);
void loadExercises(exLink *head,exLink *tail,Ex_Label wrap_ex,const char *path);

/*--------------------------------------------------------------------------------------------------------------------*/

//ex del node
exLink ex_delHead(exLink *head);
void   ex_delNode(exLink prNode);
void   ex_removeNode(exLink *head);

/*--------------------------------------------------------------------------------------------------------------------*/

//ex add new node
void ex_addNode(Ex_Label wrap_ex,exLink *tail);

/*--------------------------------------------------------------------------------------------------------------------*/

//print details of an exercise
void exDetails(Ex_Label wrap_ex);


void editRep(exLink head);
void editSet(exLink head);

/*--------------------------------------------------------------------------------------------------------------------*/

//support for note editing
exLink ex_linearSearch(exLink head,exLink tail,int key_i,const char *key_c,editE command);
exS    findExercise(Ex_Label wrap_ex,const char *name);



#endif //L08_E03_EXERCISES_H
