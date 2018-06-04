#include <stdlib.h>
#include "atoHandle.h"

//#define err_arguments (-1)
#define err_fileread1 (-2)

//--------------------------------------------------------------------------------------------------------------------//

int main(int argc,char**argv)
{
    /* main.c, file name
     * if(argc!=2) return err_arguments;
     * char fileName[25];
     * strcpy(filename,argv[1]);
     */
    const char fileName[75] = {"cities.txt"};

    wrap_l citiesData;

    /* File reading that returns a wrapper
     * if the reading was successful or NULL
     * if it wasn't
     */
    citiesData = city_acquireData(fileName);
    if(citiesData==NULL) return err_fileread1;

    /* Saves in the data structure mutual
     * distances of every city acquired
     */
    calculateDistances(citiesData);

    /* Finds every combination among cities
     * to find the best one to place k ATOs
     */
    simpleComb(citiesData);

    return 0;
}