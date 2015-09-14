#include "Model.h"
#include "Random.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static Model_Map* map = NULL;
static Model_Message msgqueue[MAX_MESSAGES];
int q_head;
int q_tail;
int q_count;

//TODO: Message queues may need to be a per agent basis. Example: 1 message tells the agent to remove itself, the other tells the free'd data to do something else
#define COMPARTMENT_SIZE 256 //TODO: fix
static Model_Agent* agents[COMPARTMENT_SIZE];
static Model_Agent** pAgentsEnd = agents;

void Model_Init(Model_Map* _map)
{
    map = _map;
    Model_Random_Init();
    memset(msgqueue, 0, sizeof(msgqueue));
    q_head = 0;
    q_tail = 0;
    q_count = 0;
    memset(agents, 0, sizeof(agents));
}

void Model_Free()
{
    map = NULL;
    memset(msgqueue, 0, sizeof(msgqueue));
    memset(agents, 0, sizeof(agents));
}

static void UpdatePositions()
{
    Model_Agent** pAgent = agents;
    while (pAgent != pAgentsEnd)
    {
        (*pAgent)->agenda(*pAgent, map);
        pAgent++;
    }
}

static void HandleMessages()
{
    Model_Message* msg;
    while((msg = Model_GetNextMessage()) != NULL)
    {
        msg->effected->messageHandler(msg->effected, msg->message);
    }
}

void Model_Next()
{
    UpdatePositions();
    HandleMessages();
}

bool Model_PlaceAgent(Model_Agent* agent, int x, int y)
{
    Model_Agent** target = &map->tiles[x][y].agent;
    if ((*target) == NULL)
    {
        *target = agent;
        agent->x = x;
        agent->y = y;

        *pAgentsEnd++ = agent;
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
    if (q_count < MAX_MESSAGES)
    {
        memcpy(&msgqueue[q_tail++], msg, sizeof(Model_Message));
        q_count++;
        if (q_tail >= MAX_MESSAGES)
            q_tail = 0;

    }
}

Model_Message* Model_GetNextMessage()
{
    Model_Message* ret = NULL;
    if (q_count > 0)
    {
        ret = &msgqueue[q_head++];
        q_count--;
        if (q_head >= MAX_MESSAGES)
            q_head = 0;
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
