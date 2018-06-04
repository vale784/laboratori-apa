#include <stdio.h>
#include <mem.h>
#include "Graph.h"

#define MAXL 30

GR      readFile(FILE*);
void    showEdges(GR);
void    calculateWt(GR);

int main(int argc,char *argv[])
{
    if(argc!=2)  return 1;
    FILE *fp = fopen(argv[1],"r");
    if(fp==NULL) return 2;
    int c;
    GR g = readFile(fp);

    do{
        printf("1 - print all nodes\n");
        printf("2 - print Edges from a node\n");
        printf("3 - create adj matrix\n");
        printf("4 - calculate all flow\n");
        printf("5 - exit\n");
        c = getc(stdin);

        switch(c) {
            case '1': GRshowNodes(g);                  break;
            case '2': showEdges(g);                    break;
            case '3': GRliToMat(g);                    break;
            case '4': calculateWt(g);                  break;
            case '5': printf("\nSee you next time\n"); break;
            default:  printf("\ninput error\n");       break;
        }
    }while(c != '5');

    GRfree(g);
    return 0;
}

GR readFile(FILE *fp)
{
    Item a,b;
    char buf1[MAXL],buf2[MAXL];
    int N,i,tmp;
    fscanf(fp,"%d",&N);
    ST s = STinit(N);
    GR g = GRinit(N);
    for(i=0;i<N;i++){
        fscanf(fp,"%s %s",buf1,buf2);
        a.name  = strdup(buf1);
        a.netId = strdup(buf2);
        STinsert(s,a);
    }
    while(fscanf(fp,"%s %s %d",buf1,buf2,&tmp)!=EOF)
        GRread(g,STsearch(s,buf1),STsearch(s,buf2),tmp);

    STfree(s);
    return g;
}

void showEdges(GR g)
{
    char ch[MAXL];
    printf("which is the start node? write the name: \n");
    fgets(ch,MAXL,stdin);
    GRshowEdges(g,ch);
}

void calculateWt(GR g)
{
    printf("total flow is %d",GRcalculatewt(g));
}