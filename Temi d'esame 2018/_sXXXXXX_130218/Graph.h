#ifndef TEMA_ESAME_GRAPH_H
#define TEMA_ESAME_GRAPH_H

typedef struct Graph *GR;
typedef struct Edge{
    int id1,id2; float val;
}ED;

GR       GRinit(int);
void     GRcpy(GR,GR);
void     GRdfscc(GR,int*);
float*   GRminDists(GR,int);
void     GRinsertED(GR,ED);
int      GRsize(GR);
void     dfs(GR,ED,int*,int*,int*,int*);
int      mstE(GR,ED*,ED*,int*);
void     GRfree(GR);

ED       EDcreate(int,int,float);

#endif //TEMA_ESAME_GRAPH_H