#include <stdbool.h>
#include "exercises.h"

#ifndef L09_E03_STEXERCISES_H
#define L09_E03_STEXERCISES_H

#define DEFAULT_M 97
#define C1_PROBING 1
#define C2_PROBING 1

typedef struct ST_ex *ST;

ST STbuild(FILE*,int);
static ST STinit(int);
static inline Cate_e cate(int,int,int);

int *getCompatibleEX(ST,int,int,int,int*);
float STgetScore(ST,int);

/*inserts element with probing*/
void STinsert(ST,EX);

/*tells if a cell of ST is full*/
bool full(ST,int);

int getIndex(ST,EX);
EX  getExercise(ST,int);

#endif //L09_E03_STEXERCISES_H