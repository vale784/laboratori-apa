#ifndef L10_E02_ITEM_H
#define L10_E02_ITEM_H

#include <stdbool.h>
#define MAX_USR_ID_LENGTH 10
#define MAX_STAT_ID_LENGTH 10

typedef struct User *Item;
typedef char * Key;

Item  ITEMsetVoid(void);
int   KEYcompare(Key,Key);
Key   KEYget(Item);
bool  ITEMless(Item,Item);
void  ITEMshow(Item);
Item  ITEMnew(char*,char*,int);
bool  ITEMactiveRent(Item);
void  ITEMstartRent(Item,char*,int);
void  ITEMstopRent(Item,int);



#endif //L10_E02_ITEM_H
