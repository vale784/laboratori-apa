
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define fileName "prodotti.txt"
#define err_fileName (-1)
#define err_fileRead (-2)

//--------------------------------------------------------------------------------------------------------------------//
//STRUCTS AND TYPEDEFS

typedef struct s_products{
    char *product;
    float price;
}prod_S,*prod_P;

//--------------------------------------------------------------------------------------------------------------------//
//PROTOTYPES

prod_S* fileRead(FILE *fp,int *P);

int simpleComb(int pos,int k,int max,int start,float B,float tmp_price,prod_P *sol,prod_S *val);
void printSolution(prod_P sol,int max,float tmp_price);

//--------------------------------------------------------------------------------------------------------------------//

int main(void)
{
    int P,K,nSol; float B;
    prod_S *val;  prod_P *sol;
    FILE *fp;

    if((fp=fopen(fileName,"r"))==NULL) return err_fileName;
    if((val = fileRead(fp,&P))==NULL) return err_fileRead;
    fclose(fp);

    sol = malloc(P * sizeof(prod_P));

    printf("K= "); scanf("%d",&K); printf("B= "); scanf("%f",&B);

    nSol=simpleComb(0,K,P,0,B,0.0,sol,val);

    switch(nSol){
        case 0:  printf("\nNo solution found"); break;
        default: printf("\nnSol= %d, Have a nice day",nSol);   break;
    }
    return 0;
}

//--------------------------------------------------------------------------------------------------------------------//
//FILE READING

prod_S* fileRead(FILE *fp,int *P)
{
    prod_S *val;
    int i;
    char buf[50];

    fscanf(fp,"%d",P);
    val = malloc(*P * sizeof(*val));

    for(i=0;i<*P;i++) {
        if (fscanf(fp, "%s %f", buf, &val[i].price) != 2)
            return NULL;
        val[i].product = strdup(buf);
        //printf("%s %.2f\n",val[i].product,val[i].price);
    }

    return val;
}

//--------------------------------------------------------------------------------------------------------------------//

int simpleComb(int pos,int k,int max,int start,float B,float tmp_price,prod_P *sol,prod_S *val)
{
    //k is the minimum of element to take //B is the max price //max is N lines
    // pos is the level of recursive function
    //tmp_price is the actual price //cnt is the counter of solutions
    int i,cnt=0;

    if((tmp_price<=B)&&(pos>=k)&&(pos<max)) {
        printSolution(*sol, pos, tmp_price);
        cnt++;
    }else if(pos>=max||tmp_price>B) {
        return cnt;
    }

    for(i=start;i<max;i++){
        sol[pos]=&val[i];
        //printf("check: %s\t product price:%.2f\t total price: %.2f\n",val[i].product,val[i].price,tmp_price+val[i].price);
        cnt += simpleComb(pos+1,k,max,i+1,B,tmp_price+val[i].price,sol,val);
    }
    return cnt;
}

void printSolution(prod_P sol,int max,float tmp_price)
{
    int i;
    for(i=0;i<max;i++)
        printf("%s %.2f\t",sol[i].product,sol[i].price);
    printf("%.2f\n",tmp_price);

}