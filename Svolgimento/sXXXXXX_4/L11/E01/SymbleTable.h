#ifndef L11_E01_SYMBLETABLE_H
#define L11_E01_SYMBLETABLE_H

typedef struct SymbleTable *ST;

ST     STinit(int);
void   STinsert(ST,char*);
int    STsearch(ST,char*);
char*  STretrieve(ST,int);
void   STfree(ST);

#endif //L11_E01_SYMBLETABLE_H