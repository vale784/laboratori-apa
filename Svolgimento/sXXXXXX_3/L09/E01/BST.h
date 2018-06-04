#ifndef L09_E01_BST_H
#define L09_E01_BST_H

#include <stdio.h>

#include "Item.h"

typedef struct BST *BST;
typedef struct BSTnode *link;

BST  BSTreadPreorder(FILE *input);
link BSTinsertR(link alpha,link omega,Item x,bool compare_less);

void BSTwritePreorder(FILE *output,BST tree);
void BSTprintR(link alpha,link omega,FILE *fp);

int  BSTcount(BST tree,int l1,int l2);
int  BSTcountR(link alpha,link omega,int start,int stop,int pos);

int  BSTdistance(BST tree,char *k1,char *k2);

void BSTmirror(BST *tree);

void BSTfree(BST tree);
void BSTfreeR(link a,link z);


//support
BST  BSTinit(void);
link newnode(Item x,link l,link r);
link rotL(link h);
link rotR(link h);

int* KEYpos(link root,char *s,int maxN);

BST BSTread(FILE *input,bool mirrored);


#endif //L09_E01_BST_H
