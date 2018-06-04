#ifndef TEMA_ESAME_SYMBOLTABLE_H
#define TEMA_ESAME_SYMBOLTABLE_H

typedef struct SymbolTable *ST;

ST      STinit(int);
void    STinsert(ST,char*);
int     STgetIndex(ST,char*);
char*   STretrieve(ST,int);

#endif //TEMA_ESAME_SYMBOLTABLE_H
