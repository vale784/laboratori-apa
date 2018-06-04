#include "Item.h"


Item ITEMsetVoid(void)
{
    Item a;
    a.x = a.y = a.type = -1;
    return a;
}

int ITEMcmp(Item a,Item b)
{
    return ((a.x == b.x)&&(a.y == b.y));
}

Item   ITEMcreate(int y,int x,int type)
{
    Item t;
    t.x = x; t.y = y; t.type = type;
    return t;
}