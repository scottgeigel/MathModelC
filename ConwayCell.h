#ifndef __CONWAY_CELL_H__
#define __CONWAY_CELL_H__
#include "Model/Model.h"
#include <stdbool.h>

typedef struct
{
    Model_Agent super;
    bool alive;
}ConwayCell;

void ConwaysGameOfLife_Init(Model_Map* map);
void ConwaysGameOfLife_Next(void);
//TODO: Should be static?
void ConwayCell_Init(ConwayCell* this);
#endif
