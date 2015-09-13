#include "Model.h"
#include "Random.h"
#include <stdio.h>

static Model_Map* map = NULL;
static Model_Message msgqueue[MAX_MESSAGES];
int q_head;
int q_tail;

void Model_Init(Model_Map* _map)
{
    map = _map;
    Model_Random_Init();
    memset(msgqueue, 0, sizeof(msgqueue));
    q_head = 0;
    q_tail = 0;
}

void Model_Next()
{
    //HandleMessages();
    //UpdatePositions();
}

bool Model_PlaceAgent(Model_Agent* agent, int x, int y)
{
    Model_Agent** target = &map->tiles[x][y].agent;
    if ((*target) == NULL)
    {
        *target = agent;
        agent->x = x;
        agent->y = y;
        printf("Placed %p at %d, %d\n", agent, x, y);
        return true;
    }
    else
    {
        printf("%p collided with %#X at %d, %d\n", agent, *target, x, y);
        return false;
    }
}

void Model_QueueMessage(Model_Message* msg)
{
    memcpy(&msgqueue[q_tail++], msg, sizeof(Model_Message));
    if (q_tail >= MAX_MESSAGES)
        q_tail = 0;
}

Model_Message* Model_GetNextMessage()
{
    Model_Message* ret = NULL;
    if (q_tail != q_head)
    {
        ret = &msgqueue[q_head++];
        if (q_head >= MAX_MESSAGES)
        {
            q_head = 0;
        }
    }
    return ret;
}
//TODO: this will cause an infinite loop if the map is full
//void Model_PlaceAgentRandom(Model_Agent* agent)
//{
//    int x;
//    int y;
//    do
//    {
//        x = RandomBetween(0, map->cols);
//        y = RandomBetween(0, map->rows);
//    }while (!Model_PlaceAgent(agent, x, y));
//}
