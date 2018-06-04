#include <stdio.h>
#include "athlete.h"

#ifndef L09_E03_LISTATHLETE_H
#define L09_E03_LISTATHLETE_H

typedef struct athlete_node *ATlink;
typedef struct athletes_w   *AW;

ATlink newNode(FILE*);
AW     AWbuild(FILE*);
int    AWsize(AW);

ATlink getHead(AW);
ATlink getNext(ATlink);
AT     getAthlete(ATlink);


#endif //L09_E03_LISTATHLETE_H
