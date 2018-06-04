#include "atoHandle.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAXL 25
#define DBG 0

// c = dist(a,b) saves the positive distance
#define dist(A,B) ((A)>(B))? ((A)-(B)):((B)-(A))

struct cityData{
    char *name;
    int population,distFrom0; //distFrom0 is unique for every city
    dist_l *distFrom;
};

/* Every city will have an array dist_l field in which will be saved
 * the distance between every other city
 */
struct cityDist{
    int dist;    //dist from city we are considering
};

/* Wrapper to hide as much as possible to the client
 * leaving him just the idea of how it works
 */
struct cityWrap{
    city_l *cities;
    int nCities;
};

/*--------------------------------------------------------------------------------------------------------------------*/

wrap_l city_acquireData(const char *fileName)
{
    FILE *fp;
    wrap_l toReturn = malloc(sizeof(*toReturn));
    city_l *x;
    char cName[MAXL];
    int i,N;

    if((fp=fopen(fileName,"r"))==NULL) return NULL;

    fscanf(fp,"%d",&N);
    x = toReturn->cities = malloc(N*sizeof(*toReturn->cities));
    toReturn->nCities = N;

    for(i=0;i<N;i++) x[i] = malloc(sizeof(*x[i]));

    for(i=0;(fscanf(fp,"%s %d %d",cName,&x[i]->population,&x[i]->distFrom0)==3)&&(i<N);i++){
        x[i]->name = strdup(cName);
    }

    fclose(fp);
    return toReturn;
}

void calculateDistances(wrap_l citiesData)
{
    city_l *x = citiesData->cities;
    int i,k;


    for(i=0;i<citiesData->nCities;i++) {
        x[i]->distFrom = malloc( citiesData->nCities* sizeof(dist_l));
        for (k=0;k<citiesData->nCities;k++)
            x[i]->distFrom[k] = malloc(sizeof(*x[i]->distFrom[k]));
    }

    for(i=0;i<citiesData->nCities;i++) {
        x[i]->distFrom[i]->dist  = 0;
        for (k=i+1;k<citiesData->nCities;k++) {
            x[i]->distFrom[k]->dist  = x[k]->distFrom[i]->dist = dist(x[i]->distFrom0, x[k]->distFrom0);
        }
    }
}

int simpleCombR(int pos,wrap_l val, int *sol,int *bestSol,int n,int k,int start,int *minSum)
{
    int i,count=0,supp;
    if(pos == k) {
        supp = sumDist(val,sol,k);
        if(supp < *minSum) {
            printf("%d\n",supp);
            *minSum = supp;
#if DBG==1
            printf("assigned supp to minSum!\n");
            system("pause");
#endif
            for (i=0;i<k;i++)
                bestSol[i] = sol[i];
        }
#if DBG==1
        printf("done assigned best sol!\n");
        system("pause");
#endif
        return 1;
    }

    for(i=start;i<n;i++){
        //save in sol index of cities in val
        sol[pos] = i;
#if DBG==1
        printf("Assigned sol[pos]!\n");
        system("pause");
#endif
        count += simpleCombR(pos+1,val,sol,bestSol,n,k,i+1,minSum);
    }
    return count;
}

int sumDist(wrap_l w_cities,int *sol,int nSol)
{   //it is O(N)
    int i,k,supp_pop;
    int sum=0;
    dist_l *x;
    m_case Case;

#if DBG==1
    for(i=0;i<nSol;i++)
        printf("%d",sol[i]);
    printf("\n");
    system("pause");
#endif

    for(i=0,k=0;i<w_cities->nCities;i++){
        x        = w_cities->cities[i]->distFrom;
        supp_pop = w_cities->cities[i]->population;
        if((k+1 < nSol) && (k-1 >= 0)){
            /* the minimum between distance(city i - city sol[k])
             * and distance (city i - city sol[k+1]) and (city i - city sol[k-1])
             */
            Case = caseAnalysis(x[i],sol[k-1],sol[k],sol[k+1]);
            switch(Case)
            {
                case backward:
                    sum += supp_pop * x[sol[--k]]->dist; break;
                case forward:
                    sum += supp_pop * x[sol[++k]]->dist; break;
                case stay:
                    sum += supp_pop * x[sol[k]]->dist;   break;
            }
        }else{
            sum += supp_pop * x[sol[k]]->dist;
        }
        if((sol[k]==i)&&(k+1 < nSol)) k++;
    }
#if DBG==1
    printf("done sum!\n");
    system("pause");
#endif
    printf("%d\n",sum);
    return sum;
}

m_case caseAnalysis(dist_l x,int i1,int i2,int i3)
{
    if (lessDist(x,i1,i2)){
        if(lessDist(x,i1,i3)) return backward;
        else return forward;
    }else{
        if(lessDist(x,i2,i3)) return stay;
        else return forward;
    }
}

bool lessDist(dist_l x,int i,int k)
{return (x[i].dist < x[k].dist);}

void simpleComb(wrap_l val)
{
    int k,*sol,*bestSol,solutionAnalyzed;
    int minSum = INT_MAX;

    printf("How many ATOs do you want to place?: "); scanf("%d",&k);
    printf("\n"); sol = malloc(k * sizeof(*sol)); bestSol = malloc(k * sizeof(*bestSol));

    solutionAnalyzed = simpleCombR(0,val,sol,bestSol,val->nCities,k,0,&minSum);
    printf("%d solutions were analyzed\n",solutionAnalyzed);
    printSolution(val->cities,bestSol,k);
    printf("%d",minSum);
}

void printSolution(city_l *val,const int *sol,int k)
{
    int i;
    printf("The best solution to place %d ATOs is:\n",k);
    for(i=0;i<k;i++)
        printf("%s ",val[sol[i]]->name);
    printf("\n");
}