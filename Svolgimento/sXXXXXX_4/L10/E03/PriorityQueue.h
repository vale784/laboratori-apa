#ifndef L10_E03_PRIORITYQUEUE_H
#define L10_E03_PRIORITYQUEUE_H

#include <stdbool.h>

typedef struct PriorityQueue *PQ;
typedef int priority;

PQ     PQinit(void);
void   PQput(PQ,int,priority);
bool   PQempty(PQ);
int    PQget(PQ);
void   PQfree(PQ);

#endif //L10_E03_PRIORITYQUEUE_H
