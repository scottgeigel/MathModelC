#include "ModelApp.h"
#include "Model/Model.h"
#include "Humans.h"
#include "Zombies.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void AppInit(int mapHeight, int mapWidth)
{
    HumansInit(10);
    ZombiesInit(10);
}

void AppNext(void)
{
    //TODO: implement if necessary
}

void AppGraphIteration()
{
    /*char buf[80];
    snprintf(buf, 80, "%d", liveCells);
    Model_Grapher_AddRowToCol("live", buf);
    snprintf(buf, 80, "%d", deadCells);
    Model_Grapher_AddRowToCol("dead", buf);*/
    //TODO: implement
    //ZombieGraphIteration();
    //HumanGraphIteration();
}

void AppEnd()
{
    HumansEnd();
    ZombiesEnd();
}
