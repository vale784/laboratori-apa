#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "PQ.h"

typedef enum menu{
    m_display, m_new, m_delete, m_goAhead, m_saveandexit, m_exit
}menuCh;

menuCh choice(void);
void   delete(Heap h);
void   goAhead(Heap h);

int main(int argc, char *argv[]) {
    FILE *in,*out;
    Heap h;
    int exit=0;
    menuCh ch;

    in  = fopen(argv[1],"r");
    if(in  == NULL) return 1;

    out = fopen(argv[2],"w");
    if(out == NULL) return 2;

    h = PQinit(DEFAULT_INIT_ENTRANTS);
    PQread(in,h);
    PQheapbuild(h);

    do{
        ch = choice();
        switch(ch) {
            case m_display:
                PQdisplay(stdout,h);      break;
            case m_new:
                PQadd(h,ENTRbuild(NULL)); break;
            case m_delete:
                delete(h);                break;
            case m_goAhead:
                goAhead(h);               break;
            case m_saveandexit:
                PQdisplay(out,h); exit=1; break;
            case m_exit:
                exit = 1;                 break;
        }
        //for every change it re-sort the heap
        PQheapbuild(h);
        //it deletes every entrant with 0 score
        PQdelete(h);
    }while(exit!=1);

    PQfree(h);

    return 0;
}

menuCh choice(void)
{
    menuCh ch;
    printf("%d-stampa stato\n",        m_display);
    printf("%d-nuovo partecipante\n",  m_new);
    printf("%d-elimina partecipante\n",m_delete);
    printf("%d-nuovo turno\n",         m_goAhead);
    printf("%d-salva ed esci\n",       m_saveandexit);
    printf("%d-esci\n",                m_exit);

    scanf("%d",&ch);
    return ch;
}

void delete(Heap h)
{
    int ID;
    printf("\nID? "); scanf("%d",&ID);
    ENTRzeroscore(PQsearch(h,ID));
}

void goAhead(Heap h)
{
    srand((unsigned int)time(NULL));
    PQresults(h, rand()%2);
}