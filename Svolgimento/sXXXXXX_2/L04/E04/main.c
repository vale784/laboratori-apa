#include <stdio.h>

int somma(int n);

int main(void)
{
    int n;
    printf("Numero su cui applicare la somma delle cifre: ");
    scanf("%d",&n);
    printf("\nsomma = %d",somma(n));
    return 0;
}

int somma(int n)
{
    if(n==0) return 0;
    int tmp=n/10,sum=0;
    sum+=(n-tmp*10)+somma(tmp);
    return sum;
}