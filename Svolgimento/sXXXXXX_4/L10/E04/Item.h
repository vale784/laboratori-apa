#ifndef L10_E04_ITEM_H
#define L10_E04_ITEM_H

typedef struct Item{
    int x,y,type;
}Item;

Item   ITEMsetVoid(void);
int    ITEMcmp(Item,Item);
Item   ITEMcreate(int,int,int);

#endif //L10_E04_ITEM_H