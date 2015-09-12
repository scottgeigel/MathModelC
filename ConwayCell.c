#include "ConwayCell.h"
#include <stdio.h>

#define MAX_CELLS 20
static ConwayCell debug[MAX_CELLS];
static Model_Map* debug2;

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

    printf("%s at %d,%d ", this->class, this->x, this->y);
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


void ConwaysGameOfLife_Init(Model_Map* map)
{
    int i;
    debug2 = map;
    for (i = 0; i < MAX_CELLS; ++i)
    {
        ConwayCell_Init(&debug[i]);
        debug[i].super.x = 2 + i;
        debug[i].super.y = 1 + (i/2);
        map->tiles[debug[i].super.x][debug[i].super.y].agent = &debug[i].super;
    }
}

void ConwaysGameOfLife_Next(void)
{
    int i;
    for (i = 0; i < MAX_CELLS; ++i)
    {
        debug[i].super.agenda((Model_Agent*) &debug[i], debug2);
    }
}

void ConwayCell_Init(ConwayCell* this)
{
    Model_Agent* super = &this->super;
    Model_Agent_Init(super, "Conway Cell");
    super->agenda = Agenda;
    super->messageHandler = MessageHandler;
}
