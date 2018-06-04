#ifndef L07_E01_ATOHANDLE_H
#define L07_E01_ATOHANDLE_H

#include <stdbool.h>

//my data structures
typedef struct cityData *city_l;
typedef struct cityDist *dist_l;
typedef struct cityWrap *wrap_l;

//my enum
typedef enum Cases{forward,backward,stay}m_case;

//input functions
wrap_l  city_acquireData(const char *);
void    calculateDistances(wrap_l citiesData);

//To evaluate solutions
void    simpleComb(wrap_l val);
int     simpleCombR(int pos,wrap_l val, int *sol,int *bestSol,int n,int k,int start,int *minSum);

//support-functions
int    sumDist(wrap_l w_cities,int *sol,int nSol);
m_case caseAnalysis(dist_l x,int i1,int i2,int i3);
bool   lessDist(dist_l x,int i,int k);

//output function
void    printSolution(city_l *val,const int *sol,int k);

#endif //L07_E01_ATOHANDLE_H
