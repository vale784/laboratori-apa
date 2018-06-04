#ifndef L10_E03_GRAPH_H
#define L10_E03_GRAPH_H

#include "SymbleTable.h"
#include "PriorityQueue.h"
#include "Edge.h"

typedef struct Graph *GR;

GR     GRinit(int);
void   GRread(GR,Item,Item,int);
void   GRinsert(GR,ED,int);
void   GRshowNodes(GR);
void   GRshowEdges(GR,char*);
void   GRliToMat(GR);
int    GRcalculatewt(GR);
void   GRfree(GR);


#endif //L10_E03_GRAPH_H
