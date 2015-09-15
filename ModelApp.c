#include "ModelApp.h"
#include "Model/Model.h"
#include "Configuration.h" //for agent defs
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct
{
    Model_Agent super;
    bool alive;
}ConwayCell;

static int liveCells = 0;
static int deadCells = 0;

ConwayCell* conwayCells;

static void TransitionToAlive(ConwayCell* this)
{
    liveCells++;
    deadCells--;

    this->alive = true;
    this->super.class = CONWAY_CELL_CLASS;
}

static void TransitionToDead(ConwayCell* this)
{
    liveCells--;
    deadCells++;

    this->alive = false;
    this->super.class = DEAD_CELL_CLASS;
}

static void Agenda(Model_Agent* this, const Model_Map* map)
{
    const int x = this->x;
    const int y = this->y;
    int left, right, up, down;
    ConwayCell *cell = (ConwayCell*)this;
    ConwayCell *neighbour = NULL;
    //Count cells
    int cellCount = 0;
    Model_Message msg;

    msg.effected = this;

    if ((x-1) < 0)
        left = map->cols - 1;
    else
        left = x-1;

    if ((x+1) == map->cols)
        right = 0;
    else
        right = x+1;

    if ((y-1) < 0)
        up = map->rows - 1;
    else
        up = y - 1;

    if ((y+1) == map->rows)
        down = 0;
    else
        down = y + 1;


    //left
    neighbour = (ConwayCell *) Model_Map_GetAgent(map, left, y);
    if(neighbour->alive)
        cellCount++;
    //right
    neighbour = (ConwayCell *) Model_Map_GetAgent(map, right, y);
    if(neighbour->alive)
        cellCount++;

    //down
    neighbour = (ConwayCell *) Model_Map_GetAgent(map, x, down);
    if(neighbour->alive)
        cellCount++;
    //up
    neighbour = (ConwayCell *) Model_Map_GetAgent(map, x, up);
    if(neighbour->alive)
        cellCount++;

    //left down
    neighbour = (ConwayCell *) Model_Map_GetAgent(map, left, down);
    if(neighbour->alive)
        cellCount++;
    //right down
    neighbour = (ConwayCell *) Model_Map_GetAgent(map, right, down);
    if(neighbour->alive)
        cellCount++;

    //left up
    neighbour = (ConwayCell *) Model_Map_GetAgent(map, left, up);
    if(neighbour->alive)
        cellCount++;
    //right up
    neighbour = (ConwayCell *) Model_Map_GetAgent(map, right, up);
    if(neighbour->alive)
        cellCount++;


    //Any live cell with two or three live neighbours lives on to the next generation.
    if (cell->alive && ((cellCount == 2) || (cellCount == 3)))
        return;
    else if (((cellCount < 2) || (cellCount > 3)) && cell->alive)
    {
        //Any live cell with fewer than two live neighbours dies, as if caused by   under-population.
        //Any live cell with more than three live neighbours dies, as if by     overcrowding.
        msg.message = "die";
        Model_QueueMessage(&msg);
    }
    else if ((cellCount == 3) && (!cell->alive))
    {
        //Any dead cell with exactly three live neighbours becomes a live cell, as  if by reproduction.
        msg.message = "live";
        Model_QueueMessage(&msg);
    }
}

static void MessageHandler(Model_Agent* this, const char* message)
{
    ConwayCell *cell = (ConwayCell*) this;
    if (strcmp(message, "live") == 0)
    {
        TransitionToAlive(cell);
    }
    else if (strcmp(message, "die") == 0)
    {
        TransitionToDead(cell);
    }
    else
    {
        printf("logic error %s\n", message);
    }
}



void ConwayCell_Init(ConwayCell* this, bool alive)
{
    const char* class;
    if (alive)
    {
        class = CONWAY_CELL_CLASS;
    }
    else
    {
        class = DEAD_CELL_CLASS;
    }
    this->alive = alive;
    Model_Agent* super = &this->super;
    Model_Agent_Init(super, class);
    super->agenda = Agenda;
    super->messageHandler = MessageHandler;
}


void AppInit(int mapHeight, int mapWidth)
{
    const int MAX_CELLS = mapHeight * mapWidth;
    int i;
    int x = 0;
    int y = 0;
    conwayCells = calloc(MAX_CELLS, sizeof(ConwayCell));
    for (i = 0; i < MAX_CELLS; ++i)
    {
        ConwayCell_Init(&conwayCells[i], Model_Random_Between(1, 100) > 60);

        if (!Model_PlaceAgent(&conwayCells[i].super, x, y))
        {
            //TODO: make a function for this
            fprintf(stderr, "Error at %s:%d in %s\n%p could not be placed at %d,%d\n", __FILE__, __LINE__, __FUNCTION__, &conwayCells[i], x, y);
            abort();
        }

        if (++y == mapHeight)
        {
            y = 0;
            x++;
        }
    }
}

void AppNext(void)
{
    //TODO: delete me?
}

void AppEnd()
{
    free(conwayCells);
    conwayCells = NULL;
}
