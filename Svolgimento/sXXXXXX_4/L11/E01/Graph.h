#ifndef L11_E01_GRAPH_H
#define L11_E01_GRAPH_H

#include <limits.h>
#include "SymbleTable.h"

#define STD_NULL_NODE (-1)
#define STD_NULL_WT   (-1)
#define MAX_WT        (5000)

typedef struct Graph *GR;
typedef struct List *link;
typedef struct Edge{
    int v,w,wt;
}ED;

GR      GRinit(int);
int     GRsizeV(GR);
void    GRinsert(GR,ED);
int     GRsizeE(GR);
void    GRinsertST(GR,ST);
int*    GRspBF(GR,int);
int*    GRdfsBcnt(GR);
GR      GRdelete(GR,int);
int     GRacyclic(GR);
int     GRconnected(GR);
int*    GRcc(GR);
void    GRdfs(GR);
void    GRkruskal(GR,ED*,int*);
void    GRfree(GR);

ED      EDcreate(int,int,int);
ED      EDvoid(void);

#endif //L11_E01_GRAPH_H