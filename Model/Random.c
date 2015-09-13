#include "Random.h"
#include <stdlib.h>
void Model_Random_Init()
{
    srand((unsigned) time(NULL));
}

int Model_Random_Next()
{
    return rand();
}

int Model_Random_Between(int min, int max)
{
    return (rand() % (max - min)) + min;
}
