#ifndef L10_E03_SYMBLETABLE_H
#define L10_E03_SYMBLETABLE_H

#include "Item.h"

typedef struct SymbleTable *ST;

ST      STinit(int);
int     STinsert(ST,Item);
int     STgetIndex(ST,Item);
Item    STretrieve(ST,int);
Item    STsearch(ST,char*);
void    STfree(ST);

#endif //L10_E03_SYMBLETABLE_H