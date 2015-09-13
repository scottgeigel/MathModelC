#include "ConwayCell.h"
#include "Configuration.h" //for agent defs
#include <stdio.h>

#include <assert.h>

static int liveCells = 0;
static int deadCells = 0;

#define MAX_CELLS (10*12)
static ConwayCell debug[MAX_CELLS];//TODO: fix this
static Model_Map* debug2;

//TODO: This will cause a bug. transitions need to be queued as they will alter events on a bias for who had their turn first
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

    printf("%s at %d,%d has %d neighbours\n", this->class, this->x, this->y, cellCount);

    //Any live cell with fewer than two live neighbours dies, as if caused by under-population.
    //Any live cell with more than three live neighbours dies, as if by overcrowding.
    if ((cellCount < 2) || (cellCount > 3))
    {
        msg.message = "die";
        Model_QueueMessage(&msg);
    }
    //Any live cell with two or three live neighbours lives on to the next generation.
    if ((cellCount == 2) || (cellCount == 3))
        ;//printf("survive\n");//survive
    //Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
    if ((cellCount == 3) && (!cell->alive))
    {
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


void ConwaysGameOfLife_Init(Model_Map* map)
{
    int i;
    int x = 0;
    int y = 0;
    debug2 = map;
    for (i = 0; i < MAX_CELLS; ++i)
    {
        ConwayCell_Init(&debug[i], Model_Random_Between(1, 100) > 60);
        Model_PlaceAgent(&debug[i].super, x, y++);

        if (y == map->rows)
        {
            y = 0;
            x++;
        }
        //debug[i].super.x = 2 + i;
        //debug[i].super.y = 1 + (i/2);
        //map->tiles[debug[i].super.x][debug[i].super.y].agent = &debug[i].super;
    }
}

void ConwaysGameOfLife_Next(void)
{
    int i;
    Model_Message* msg;

    printf("\n\nBEGIN DEBUG\n");
    {
        int i,j;
        for (j = 0; j < debug2->rows; ++j)
        {
            for (i = 0; i < debug2->cols; ++i)
            {
                printf("%c ", debug2->tiles[i][j].agent->class[0]);
            }
            printf("\n");
        }
    }

    printf("\nEND DEBUG\n\n");
    for (i = 0; i < MAX_CELLS; ++i)
    {
        debug[i].super.agenda((Model_Agent*) &debug[i], debug2);
    }

    while((msg = Model_GetNextMessage()) != NULL)
    {
        printf("%d,%d Proccessing %s to %p\n",msg->effected->x,msg->effected->y, msg->message, msg->effected);
        msg->effected->messageHandler(msg->effected, msg->message);
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
