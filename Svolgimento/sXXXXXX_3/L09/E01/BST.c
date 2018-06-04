#include "BST.h"

#include <stdlib.h>
#include <mem.h>

struct BST{
    link root, z;
    int n_nodes;
};

struct BSTnode{
    Item item;
    link l,r;
    bool left,right;
};

BST BSTinit(void)
{
    BST new = malloc(sizeof(*new));
    new->n_nodes = 0;
    new->root = new->z = newnode(ITEMsetvoid(),NULL,NULL);
    return new;
}

link newnode(Item x,link l,link r)
{
    link a = malloc(sizeof(*a));
    a->item = x; a->l = l; a->r = r;
    return a;
}

BST  BSTreadPreorder(FILE *input)
{
    BST new = BSTread(input,false);
    return new;
}

BST BSTread(FILE *input,bool mirrored)
{
    BST new = BSTinit();
    link h;
    Item x;

    while((x = ITEMnew(input))!=NULL){
        h = BSTinsertR(new->root,new->z,x,!mirrored);
        fscanf(input,"%d%d",&h->left,&h->right);
        new->n_nodes++;
    }
    return new;
}

link BSTinsertR(link alpha,link omega,Item x,bool compare_less)
{
    if(alpha==omega)
        return newnode(x,omega,omega);
    if((compare_less)? (ITEMless(x,alpha->item)):(ITEMmajor(x,alpha->item))){
        BSTinsertR(alpha->l, omega, x, compare_less);
        alpha = rotR(alpha);
    }else{
        BSTinsertR(alpha->r, omega, x, compare_less);
        alpha = rotL(alpha);
    }
    return alpha;
}

link rotL(link h)
{
    link x = h->r;
    h->r = x->l;
    x->l = h;
    return x;
}

link rotR(link h)
{
    link x = h->l;
    h->l = x->r;
    x->r = h;
    return x;
}

#ifdef DBG
/*function to test correct input*/
bool BSTcorrect(link alpha,link omega)
{
    if(alpha==omega) return true;

    if(alpha->left == true)
        if(alpha->l == NULL)
            return false; else BSTcorrect(alpha->l,omega);

    if(alpha->right == true)
        if(alpha->r == NULL)
            return false; else BSTcorrect(alpha->r,omega);
}
#endif //Debug

void BSTwritePreorder(FILE *output,BST tree)
{ BSTprintR(tree->root,tree->z,output); }

void BSTprintR(link alpha,link omega,FILE *fp)
{
    if(alpha==omega)
        return;
    BSTprintR(alpha->l,omega,fp);
    BSTprintR(alpha->r,omega,fp);

    ITEMprint(alpha->item,fp);
    fprintf(fp,"%d %d",alpha->left,alpha->right);
}

int  BSTcount(BST tree,int l1,int l2)
{ return BSTcountR(tree->root,tree->z,l1,l2,0); }

int BSTcountR(link alpha,link omega,int start,int stop,int pos)
{
    int cnt = 0;
    if(alpha==omega) return 0;

    if(pos==stop)
        return ((alpha->right)&&(alpha->left));

    if((pos>=start)&&(pos<stop))
        cnt += ((alpha->right)&&(alpha->left));

    BSTcountR(alpha->l,omega,start,stop,pos+1);
    BSTcountR(alpha->r,omega,start,stop,pos+1);
    return cnt;
}

int  BSTdistance(BST tree,char *k1,char *k2)
{
    int *path1 = KEYpos(tree->root,k1,tree->n_nodes);
    int *path2 = KEYpos(tree->root,k2,tree->n_nodes);
    int cnt;

    for(cnt=0;((*path1)!=-1)&&((*path2)!=-1);){
        while((*path1)==(*path2)) {
            path1++; path2++; }
        while(*path1!=-1){
            path1++; cnt++;}
        while(*path2!=-1){
            cnt++; path2++; }
    }

    return cnt;
}

int* KEYpos(link root,char *s,int maxN)
{
    int path[maxN],i;
    link h = root;
    bool flag = true;

    for(i=0;flag;i++)
        if(strcmp(ITEMgetKey(h->item),s)<0){
            path[i] = 1;
            h = h->r;
        }else if(strcmp(ITEMgetKey(h->item),s)>0){
            path[i] = 0;
            h = h->l;
        }else{
            path[i] = -1;
            flag = false;
        }
    return &path[0];
}

void BSTmirror(BST *tree)
{
    FILE *fp=fopen("support.txt","w+");

    BSTwritePreorder(fp,*tree);
    BSTfree(*tree);
    *tree = BSTread(fp,true);
}

void BSTfree(BST tree)
{
    BSTfreeR(tree->root,tree->z);
    free(tree);
}

void BSTfreeR(link a,link z)
{
    if(a==z) return;
    BSTfreeR(a->l,z);
    BSTfreeR(a->r,z);
    ITEMfree(a->item);
    free(a);
}