#ifndef L10_E04_SYMBLETABLE_H
#define L10_E04_SYMBLETABLE_H

#include "Item.h"

typedef struct SybmleTable *ST;

ST     STinit(int,int,int);
int    STinsert(ST,Item);
int    STgetIndex(ST,Item);
Item   STretrieve(ST,int);
void   STeditType(ST,int,int);
void   STfree(ST);

#endif //L10_E04_SYMBLETABLE_H
