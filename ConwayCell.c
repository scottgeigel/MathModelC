#include "ConwayCell.h"
#include <stdio.h>

static void Agenda(Model_Agent* this, const Model_Map* map)
{
    ConwayCell *cell = (ConwayCell*)this;
    //Count cells
    int cellCount = 0;
    const int x = this->x;
    const int y = this->y;

    //left
    if(Model_Map_HasAgent(map, x - 1, y))
        cellCount++;
    //left down
    if(Model_Map_HasAgent(map, x - 1, y + 1))
        cellCount++;
    //left up
    if(Model_Map_HasAgent(map, x - 1, y - 1))
        cellCount++;
    //down
    if(Model_Map_HasAgent(map, x, y + 1))
        cellCount++;
    //right down
    if(Model_Map_HasAgent(map, x + 1, y + 1))
        cellCount++;
    //right
    if(Model_Map_HasAgent(map, x + 1, y))
        cellCount++;
    //right up
    if(Model_Map_HasAgent(map, x + 1, y - 1))
        cellCount++;


    //Any live cell with fewer than two live neighbours dies, as if caused by under-population.
    //Any live cell with more than three live neighbours dies, as if by overcrowding.
    if ((cellCount < 2) || (cellCount > 3))
        printf("die\n");//die
    //Any live cell with two or three live neighbours lives on to the next generation.
    if ((cellCount == 2) || (cellCount == 3))
        printf("survive\n");//survive
    //Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
    if ((cellCount == 3) && (!cell->alive))
        printf("reproduce\n");//reproduce
}

static void MessageHandler(Model_Agent* this, const char* message)
{

}

void ConwayCell_Init(ConwayCell* this)
{
    Model_Agent* super = &this->super;
    Model_Agent_Init(super, "Conway Cell");
    super->agenda = Agenda;
    super->messageHandler = MessageHandler;
}
