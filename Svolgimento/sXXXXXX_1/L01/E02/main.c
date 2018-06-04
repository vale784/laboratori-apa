#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

    char mat[20][20]=   {{'x','f','o','o','x'},
                        {'y','o','x','z','f'},
                        {'x','o','2','f','o'},
                        {'g','4','x','a','o'}};

    char s[20]={'f','o','o'};
    int r=4,c=5;

    printf("\n%d\n",mat_search(mat,r,c,s));


    return 0;
}

int mat_search(char mat[20][20],int r, int c, char s[20]){
    int i,k,tot=0;
    char supp[r+2];

    //printf("%s\n\n",s);

    //orizzontale
    for(i=0;i<r;i++){
        tot+=vet_scan(mat[i],s,c);
        //printf("orizz: %s  %d\n",mat[i],tot);
        vet_invert(mat[i],c);
        tot+=vet_scan(mat[i],s,c);
        //printf("inv orizz: %s  %d\n",mat[i],tot);
    }

    //verticale
    for(i=0;i<c;i++){
        for(k=0;k<r;k++){
            supp[k]=mat[k][i];
        }
        supp[k]='\0';
        tot+=vet_scan(supp,s,r);
        //printf("vert: %s  %d\n",supp,tot);
        vet_invert(supp,r);
        tot+=vet_scan(supp,s,r);
        //printf("inv vert: %s  %d\n",supp,tot);
    }

    return tot;
}

int vet_scan(char v[],char s[],int l){
    int i,t;

    for(i=0,t=0;i<l-strlen(s)+1;i++){
        if(strncmp(&v[i],s,strlen(s))==0){t++;}
    }

    return t;
}


void vet_invert(char v[],int l){
    char supp;
    int i;

    for(i=0;i<(l/2);i++){
        supp=v[i];
        v[i]=v[l-i-1];
        v[l-i-1]=supp;
    }
    return;
}



