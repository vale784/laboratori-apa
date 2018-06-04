#ifndef L09_E03_ATHLETE_H
#define L09_E03_ATHLETE_H

typedef struct athlete *AT;


AT ATnew(FILE*);

int getNA(AT);
int getAA(AT);
int getAI(AT);
int getSum(AT);


#endif //L09_E03_ATHLETE_H
