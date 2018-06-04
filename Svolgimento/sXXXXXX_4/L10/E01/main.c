#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mem.h>

#define Max(a,b) (((a)>(b))? (a):(b))
#define Cost(R,i,top) (((i)==(top))? 0:((R)[(i)].cost))

bool flag_stop = false;

/*--------------------------------------------------------------------------------------------------------------------*/

/* coord and value*/
typedef struct Mob{ int x,y,value; } mob;
/* radiant, attack value, defence value and cost*/
typedef struct Res{ int rad,att,def,cost; } res;

/*--------------------------------------------------------------------------------------------------------------------*/

mob* MOB_build(char*,int*);
res* RES_build(char*,int*);

int MAP_init(int*,int,int,mob*,int,mob*,int);
int max_mob_value(mob*,int);

void RES_placing(int*,int,int,res*,int,int,int);
int  justPlace(int,int,int*,int,int,res*,int,int,int);

bool goodSol(const int*,int,int,res*,int);
bool effective(const int*,int,int,res*,int,int,int);

void printSol(const int*,int,int,res*);


/*--------------------------------------------------------------------------------------------------------------------*/

int main(int argc,char *argv[])
{
    /* <main.c> <budget> <n_rows> <n_columns>
     * <ally_file> <enemies_file> <resources_file>
     */
    if(argc!=7) return argc;

    int budget = atoi(argv[1]),  R = atoi(argv[2]), C = atoi(argv[3]);
    int map[R][C];
    int nA,nE,nR,delta;

    /* A,E stand for 'Allies' and 'Enemies' defined as mob*/
    mob *A = MOB_build(argv[4],&nA), *E = MOB_build(argv[5],&nE);
    /* T stands for 'Towers' defined as res*/
    res *T = RES_build(argv[6],&nR);

    if(A==NULL) return 2;
    if(E==NULL) return 3;
    if(T==NULL) return 4;

    delta = MAP_init(&map[0][0],R,C,A,nA,E,nE);
    RES_placing(&map[0][0],R,C,T,nR,budget,delta);

    return 0;
}

/*----------------------------------------------------------------------------------------------------------------------
 * File reading
 */

mob* MOB_build(char *filename,int *N)
{
    FILE *fp = fopen(filename,"r");
    if(fp==NULL) return NULL;
    mob *x;
    int i;

    fscanf(fp,"%d",N);
    x = malloc(((size_t) *N)*sizeof(*x));

    for(i=0;fscanf(fp,"%d %d %d",&x[i].y,&x[i].x,&x[i].value)!=EOF;i++);
    fclose(fp);

    return x;
}

res* RES_build(char *filename,int *N)
{
    FILE *fp = fopen(filename,"r");
    if(fp==NULL) return NULL;
    res *x;
    int i;

    fscanf(fp,"%d",N);
    x = malloc(((size_t) *N)*sizeof(*x));

    for(i=0;fscanf(fp,"%d %d %d %d",&x[i].rad,&x[i].att,&x[i].def,&x[i].cost)!=EOF;i++);
    fclose(fp);

    return x;
}

/*----------------------------------------------------------------------------------------------------------------------
 * initializing function and support
 */

int MAP_init(int *map,int r,int c,mob *A,int nA,mob *E,int nE)
{
    int i,tmp_x,tmp_y;

    /* delta is a quantity to add twice to value of allies to recognise them from enemies */
    int delta = Max(max_mob_value(A,nA),max_mob_value(E,nE))+1;

    memset(map,0,r*c*sizeof(int));

    for(i=0;i<nA;i++){
        tmp_x = A[i].x; tmp_y = A[i].y;
        if((tmp_x < c)&&(tmp_y < r)&&(tmp_x > 0)&&(tmp_y > 0))
            *(map+tmp_y*c+tmp_x) = -(A[i].value + delta);
    }

    for(i=0;i<nE;i++){
        tmp_x = E[i].x; tmp_y = E[i].y;
        if((tmp_x < c)&&(tmp_y < r)&&(tmp_x > 0)&&(tmp_y > 0))
            *(map+tmp_y*c+tmp_x) = -(E[i].value);
    }

    return delta;
}

int max_mob_value(mob *x,int n)
{
    int i, max = -1;
    for(i=0;i<n;i++)
        max = Max(x[i].value,max);
    return max;
}

/*----------------------------------------------------------------------------------------------------------------------
 * Recursive functions
 */

void RES_placing(int *map,int r,int c,res *R,int nR,int budget,int delta)
{
    int sol_analyzed;
    flag_stop = false;

    sol_analyzed = justPlace(0,r*c,map,r,c,R,nR,budget,delta);
    printf("\n\nSol Analyzed = %d",sol_analyzed);
}

/* it is repeated dispositions (?!translation!?) of resources on the map*/
int justPlace(int pos,int n,int *map,int r,int c,res *R,int nR,int budget,int delta)
{
    if(flag_stop==true) return 0;
    int i,cont=0;
    if(pos>n){
        if(goodSol(map,r,c,R,delta)) {
            printSol(map,r,c,R);
            flag_stop = true;
            printf("Remaining budget: %d\n",budget);
        }
        return 1;
    }

    //check if it is a space to insert
    if(*(map + pos) < 0) return cont;

    for(i=0;i<nR+1;i++){
        //pruning: cost covered by budget
        if((budget-Cost(R,i,nR)) >= 0){
            //pruning: someone can be hit/healed by this resource?
            if(effective(map,r,c,R,pos,i,delta))
                *(map+pos) = i; else *(map+pos) = 0;
            cont += justPlace(pos +1,n,map,r,c,R,nR,budget-Cost(R,i,nR),delta);
            if(flag_stop==true) return cont;
        }
    }
    return cont;
}

/*----------------------------------------------------------------------------------------------------------------------
 * check functions
 */

bool goodSol(const int *map,int r,int c,res *R,int delta)
{
    /* we'll assume, because of pruning, that we
     * have no problems concerning the budget
     */
    int map_cpy[r][c],i,j;

    for(i=0;i<r;i++)
        for(j=0;j<c;j++)
            map_cpy[i][j] = *(map + i*c + j);

    for(i=0;i<r;i++)
        for(j=0;j<c;j++)
            if(*(map + i*c + j)>=0)
            {
                int rad = (*(map + i*c + j)==0)? 0:R[*(map + i*c + j)-1].rad;
                int att = (*(map + i*c + j)==0)? 0:R[*(map + i*c + j)-1].att;
                int def = (*(map + i*c + j)==0)? 0:R[*(map + i*c + j)-1].def;
                int left = (j-rad<0)? (0):(j-rad), right = (j+rad>c-1)? (c):(j+rad);
                int top  = (i-rad<0)? (0):(i-rad), down  = (i+rad>r-1)? (r):(i+rad);
                int p,q;
                for(p=top;p<down;p++)
                    for(q=left;q<right;q++)
                        if(map_cpy[p][q]<0){
                            if(map_cpy[p][q]<-delta){
                                map_cpy[p][q] = (map_cpy[p][q]+def >= -delta)? (0):(map_cpy[p][q]+def);
                            }else map_cpy[p][q] += att;
                        }
            }

    for(i=0;i<r;i++)
        for(j=0;j<c;j++)
            if(map_cpy[i][j]<0)
                return false;

    return true;
}

bool effective(const int *map,int r,int c,res *R,int pos,int i,int delta)
{
    int x_cent = pos%r , y_cent = pos/r, rad = R[i].rad;
    int left,right,top,down,j,k;

    left  = (x_cent-rad < 0)? (0):(x_cent-rad); right = (x_cent+rad >(c-1))? (c):(x_cent+rad);
    top   = (y_cent-rad < 0)? (0):(y_cent-rad); down  = (y_cent+rad >(r-1))? (r):(y_cent+rad);

    for(j=top;j<down;j++)
        for(k=left;k<right;k++)
            if(*(map + j*c + k)<0) {
                //ally?
                if (*(map + j*c + k)< (-delta)){
                    if(R[i].def!=0) return true;
                }else{
                    if(R[i].att!=0) return true;
                }
            }

    return false;
}

/*----------------------------------------------------------------------------------------------------------------------
 * support functions
 */

void printSol(const int *map,int r,int c,res *R)
{
    int i,j,tmp;

    for(i=0;i<r;i++)
        for(j=0;j<c;j++)
            if((tmp=(*(map + i*c + j))>0))
                printf("Res %d (range: %d, atk: %d, def: %d) in <%d,%d>\n",
                       tmp,R[tmp].rad,R[tmp].att,R[tmp].def,i,j);
}