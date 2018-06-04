#include "habitat.h"

/*--------------------------------------------------------------------------------------------------------------------*/

#define fileKoala       "hab.txt"
#define err_fileKoala   (-1)

#define fileFamily      "fam.txt"
#define err_fileFamily  (-2)

#define fileEnemies     "nem.txt"
#define err_fileEnemies (-3)

/*--------------------------------------------------------------------------------------------------------------------*/
const int m = 2;

int main(void)
{
    wrap_koala k_list;
    wrap_family f_list;

    //file reading
    if(!koala_fileRead(fileKoala,&k_list))     return err_fileKoala;

    if(!family_fileRead(fileFamily,&k_list))   return err_fileFamily;

    if(!enemies_fileRead(fileEnemies,&f_list)) return err_fileEnemies;

    simpleCombWrap(k_list,f_list,m);

    return 0;
}