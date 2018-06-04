//
// Created by Valerio Casalino on 03/12/2017.
//

#ifndef L08_E01_HABITAT_H
#define L08_E01_HABITAT_H

/*--------------------------------------------------------------------------------------------------------------------*/

#include <stdbool.h>

/*--------------------------------------------------------------------------------------------------------------------*/

typedef struct koala_Array   *koalas;
typedef struct incomp_node   *fam_link;
typedef struct koala_Wrapper *wrap_koala;
typedef struct Solution      *sol;
typedef struct family_list   *wrap_family;

typedef enum Opt{allLimits,noEnemies}option;

/*--------------------------------------------------------------------------------------------------------------------*/

bool koala_fileRead(const char *path,wrap_koala *koalas);
bool family_fileRead(const char *path,wrap_koala *koalas);
bool enemies_fileRead(const char *path,wrap_family *incompatibilities);

/*--------------------------------------------------------------------------------------------------------------------*/

void simpleCombWrap(wrap_koala koalas,wrap_family incompatibilities,int m);
void simple_comb(int pos, int m, wrap_koala val, sol solution, fam_link head,
                 int k_start, int k, bool *flag,option a);

void printSolution(sol solution,int nKoalas);

bool goodSolution(sol solution,int nKoalas,int m,fam_link head,option a);
void insertionSort(int *supp,int l,int r);

/*--------------------------------------------------------------------------------------------------------------------*/

#endif //L08_E01_HABITAT_H