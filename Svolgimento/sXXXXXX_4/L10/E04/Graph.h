#ifndef L10_E04_GRAPH_H
#define L10_E04_GRAPH_H

#include "SymbleTable.h"

typedef struct Graph *GR;
typedef struct Edge{
    int a,b;
}ED;

GR     GRinit(int,int,int);
ED     EDcreate(Item,Item);
ED     EDcreateInt(int,int);
void   GRinsert(GR,ED);
void   GRdfs_searchPath(GR,Item,int,int);
void   GRfree(GR);

#endif //L10_E04_GRAPH_H
