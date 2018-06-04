#include <stdio.h>
#include <stdbool.h>

#ifndef L09_E01_ITEM_H
#define L09_E01_ITEM_H

typedef struct Item *Item;

Item ITEMsetvoid(void);
Item ITEMnew(FILE *fp);
void ITEMfree(Item x);

void ITEMprint(Item x,FILE *out);

bool ITEMmajor(Item a,Item b);
bool ITEMless(Item a,Item b);

char* ITEMgetKey(Item x);

#endif //L09_E01_ITEM_H
