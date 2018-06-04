#include "habitat.h"

#include <stdio.h>
#include <stdlib.h>

#define max(A,B) (((A)>(B))? (A):(B))
#define min(A,B) (((A)<(B))? (A):(B))
#define swap(A,B) {int c = (A); (A)=(B); (B)=(A);}
#define DBG 0

/*--------------------------------------------------------------------------------------------------------------------*/

struct koala_Wrapper{
    koalas koalas_array;
    int    nKoalas;
};

struct koala_Array{
    int idKoala;
    int idFamily;
    int *trees;
    int nTrees;
};

struct Solution{
    int koala;
    int family;
    int tree;
};

struct incomp_node{
    int      incomp;
    fam_link next;
};

struct family_list{
    fam_link  head;
    int nIncomp;
};

/*--------------------------------------------------------------------------------------------------------------------*/

bool koala_fileRead(const char *path,wrap_koala *koalas)
{
    FILE *fp;
    int nKoalas,supp_nTrees,supp_idTree,i,k;

    if((fp=fopen(path,"r"))==NULL) return false;
    *koalas = malloc(sizeof(**koalas));

    fscanf(fp,"%d",&nKoalas); (*koalas)->nKoalas = nKoalas;
    (*koalas)->koalas_array = malloc(nKoalas * sizeof(*(*koalas)->koalas_array));

    for(i=0;i<nKoalas;i++) {
        fscanf(fp, "%d", &supp_nTrees);
        (*koalas)->koalas_array[i].nTrees = supp_nTrees;
        (*koalas)->koalas_array[i].idKoala = i;
        (*koalas)->koalas_array[i].trees = malloc(supp_nTrees*sizeof(*(*koalas)->koalas_array[i].trees));
        for(k=0;k<supp_nTrees;k++) {
            fscanf(fp,"%d",&supp_idTree);
            (*koalas)->koalas_array[i].trees[k] = supp_idTree;
        }
    }
    fclose(fp);
    return true;
}

bool family_fileRead(const char *path,wrap_koala *koalas)
{
    FILE *fp;
    int nFamilies,supp_nKoala,supp_idKoala,i,k;

    if((fp=fopen(path,"r"))==NULL) return false;
    fscanf(fp,"%d",&nFamilies);

    for(i=0;i<nFamilies;i++){
        fscanf(fp,"%d",&supp_nKoala);
        for(k=0;k<supp_nKoala;k++) {
            fscanf(fp, "%d", &supp_idKoala);
            (*koalas)->koalas_array[supp_idKoala].idFamily = i;
        }
    }

    fclose(fp);
    return true;
}

bool enemies_fileRead(const char *path,wrap_family *incompatibilities)
{
    FILE *fp;
    fam_link x,p;
    int a,b,cnt=0;

    if((fp=fopen(path,"r"))==NULL) return false;
    *incompatibilities = malloc(sizeof(**incompatibilities));
    x = (*incompatibilities)->head = malloc(sizeof(*x));
    for(p = x;fscanf(fp,"%d %d",&a,&b)!=EOF;p = x,cnt++,x = x->next){
        x->incomp = max(a,b)*10 + min(a,b);
        x->next = malloc(sizeof(*x));
    }
    p->next=NULL; free(x); (*incompatibilities)->nIncomp = cnt;

    fclose(fp);
    return true;
}

/*--------------------------------------------------------------------------------------------------------------------*/

void simpleCombWrap(wrap_koala koalas,wrap_family incompatibilities,int m)
{
    bool flag    = false;
    option a     = allLimits;
    sol solution = malloc(koalas->nKoalas * sizeof(*solution));

    simple_comb(0,m,koalas,solution,incompatibilities->head,0,koalas->nKoalas,&flag,a);

    if(!flag) {printf("No solutions found\n"); return;}
}

void simple_comb(int pos,int m,wrap_koala val,sol solution,
                 fam_link head,int k_start,int k,bool *flag,option a)
{
    if(*flag) return;
    int i,j;

    if(pos>=k){
#if DBG==1
        //printf("pos = %d\n",pos);
        //printSolution(solution,val->nKoalas);
        //system("pause");
#endif
        if(goodSolution(solution,pos,m,head,a)) {
            printSolution(solution, val->nKoalas);
            printf("\n");
            *flag = true;
            return;
        }else return;

    }

    for(i=k_start;i<val->nKoalas;i++) {
        solution[pos].koala  = val->koalas_array[i].idKoala;
        solution[pos].family = val->koalas_array[i].idFamily;
        for (j = 0; j < val->koalas_array[i].nTrees; j++) {
            solution[pos].tree = val->koalas_array[i].trees[j];
            simple_comb(pos + 1, m, val, solution, head, i+1, k, flag, a);
            if(*flag) return;
        }
    }
}

/*--------------------------------------------------------------------------------------------------------------------*/

void printSolution(sol solution,int nKoalas)
{
    int i;
    for(i=0;i<nKoalas;i++)
        printf("koala n%d on tree n%d\n", solution[i].koala, solution[i].tree);
}

/*--------------------------------------------------------------------------------------------------------------------*/

bool goodSolution(sol solution,int nKoalas,int m,fam_link head,option a)
{
    int supp_sol,i,k,cnt=0,v;
    int supp[nKoalas];
    fam_link x;

    for(i=0;i<nKoalas;i++)
        supp[i] = solution[i].tree;

    //sorting tree array for checking
    insertionSort(supp,0,nKoalas);

    for(i=0;i<nKoalas;i++) {
        v = supp[i];
        while (v == supp[i])
        {
            i++; cnt++;
        }
        if (cnt >= m) return false;
        cnt = 0;
    }

    if(a==noEnemies) return true;

    for(i=0;i<nKoalas;i++){
        for(k=i+1;k<nKoalas;k++) {
            if ((solution[i].family!=solution[k].family) && (solution[i].tree==solution[k].tree)){

                supp_sol = max(solution[i].family,
                               solution[k].family)*10 + min(solution[i].family,
                                                            solution[k].family);
            for (x = head; x != NULL; x = x->next)

                if (x->incomp == supp_sol)
                    return false;
            }
        }
    }
    return true;
}

void insertionSort(int *supp,int l,int r)
{
    int i,j,i_supp;

    for(i=r-1;i<l;i--)
        if(supp[i]<supp[i-1])
            swap(supp[i],supp[i-1]);

    for(i=l+2;i<r;i++){
        i_supp = supp[i];
        for(j=i;i_supp<supp[j-1];j--)
            supp[j] = supp[j-1];
        supp[j] = i_supp;
    }
}

/*--------------------------------------------------------------------------------------------------------------------*/