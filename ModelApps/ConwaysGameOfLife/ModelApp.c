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
    Model_Message msg;
    ConwayCell *cell = (ConwayCell*)this;
    ConwayCell *neighbour = NULL;
    int cell_count = 0;
    const Model_Tile tiles[9]; //used to store 9 tiles encompassed in a 1 tile radius of the point
    size_t tiles_count = Model_Map_ScanArea(map, tiles, this->x, this->y, 1);

    //Count cells
    for (size_t i = 0; i < tiles_count; i++)
    {
        neighbour = (ConwayCell*) tiles[i].agent;
        if (neighbour->alive && (neighbour->super.x != this->x && neighbour->super.y != this->y))
        {
            cell_count++;
        }
    }


    msg.effected = this;
    //Any live cell with two or three live neighbours lives on to the next generation.
    if (cell->alive && ((cell_count == 2) || (cell_count == 3)))
        return;
    else if (((cell_count < 2) || (cell_count > 3)) && cell->alive)
    {
        //Any live cell with fewer than two live neighbours dies, as if caused by   under-population.
        //Any live cell with more than three live neighbours dies, as if by     overcrowding.
        msg.message = "die";
        Model_QueueMessage(&msg);
    }
    else if ((cell_count == 3) && (!cell->alive))
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



static void ConwayCell_Init(ConwayCell* this, bool alive)
{
    const char* class;
    if (alive)
    {
        liveCells++;
        class = CONWAY_CELL_CLASS;
    }
    else
    {
        deadCells++;
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
    Model_Grapher_AddColDef("live");
    Model_Grapher_AddColDef("dead");
}

void AppNext(void)
{
    //TODO: implement if necessary
}

void AppGraphIteration()
{
    char buf[80];
    snprintf(buf, 80, "%d", liveCells);
    Model_Grapher_AddRowToCol("live", buf);
    snprintf(buf, 80, "%d", deadCells);
    Model_Grapher_AddRowToCol("dead", buf);
}

void AppEnd()
{
    free(conwayCells);
    conwayCells = NULL;
}
