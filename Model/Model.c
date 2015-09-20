#include "Model.h"
#include "Random.h"
#include "ModelApp.h"
#include "Types/DynamicQueue.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static Model_Map* map = NULL;
static FILE* graphOutputFile = NULL;
static unsigned long iterationCount;
/*static Model_Message *msgqueue;
int q_max_size;
int q_head;
int q_tail;
int q_count;*/

static Queue msgqueue;

//TODO: Message queues may need to be a per agent basis. Example: 1 message tells the agent to remove itself, the other tells the free'd data to do something else
static Model_Agent** agents;
static Model_Agent** pAgentsEnd;
static Model_Agent** agent_Boundry;
static int max_agents;

void Model_Init(Model_Map* _map)
{
    iterationCount = 0;
    map = _map;
    Model_Random_Init();

    //q_max_size = map->cols * map->rows;//128;
    //msgqueue = calloc(q_max_size, sizeof(Model_Message));
    Queue_Init(&msgqueue);
    //q_head = 0;
    //q_tail = 0;
    //q_count = 0;

    max_agents = 64;//map->cols * map->rows;
    agents = calloc(max_agents, sizeof(Model_Agent*));
    pAgentsEnd = agents;
    agent_Boundry = &agents[max_agents];

    graphOutputFile = fopen("outgraph.csv", "w");
    Model_Grapher_Start(graphOutputFile);
    Model_Grapher_AddColDef("iteration");
    AppInit(map->rows, map->cols);
}

void Model_Free()
{
    map = NULL;
    Queue_Destroy(&msgqueue);
    //free(msgqueue);
    //msgqueue = NULL;
    free(agents);
    agents = NULL;
    Model_Grapher_End();
    fclose(graphOutputFile);
    graphOutputFile = NULL;
    AppEnd();
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
    bool somethingHappened = !Queue_Empty(&msgqueue);
    Model_Message* msg;
    while((msg = Model_GetNextMessage()) != NULL)
    {
        msg->effected->messageHandler(msg->effected, msg->message);
        free(msg);
    }
    if (somethingHappened)
        iterationCount++;
}

void Model_Next()
{
    UpdatePositions();
    HandleMessages();
    AppNext();
}

void Model_GraphIteration()
{
    static unsigned long lastIterationCount = 0 - 1;
    if (lastIterationCount != iterationCount)
    {
        lastIterationCount = iterationCount;
        char buf[80];
        snprintf(buf, 80, "%lu", iterationCount);
        Model_Grapher_AddRowToCol("iteration", buf);
        AppGraphIteration();
        Model_Grapher_NextRow();
    }
}

bool Model_PlaceAgent(Model_Agent* agent, int x, int y)
{
    Model_Agent** target = &map->tiles[x][y].agent;
    if ((*target) == NULL)
    {
        if(pAgentsEnd == agent_Boundry)
        {
            agents = realloc(agents, sizeof(Model_Agent*) * (max_agents << 1));
            if (agents == NULL)
            {
                printf("Failed to grow agents from %d to %d\n", max_agents, max_agents*2);
                abort();
            }
            //our pointer may have changed, so move to the new position
            pAgentsEnd = agents + max_agents;
            max_agents <<= 1;
            agent_Boundry = agents + max_agents;
        }

        *target = agent;
        agent->x = x;
        agent->y = y;

        *pAgentsEnd++ = agent;
        return true;
    }
    else
    {
        printf("%p collided with %p at %d, %d\n", agent, *target, x, y);
        return false;
    }
}

bool Model_MoveAgent(Model_Agent* agent, int x, int y)
{
    Model_Agent** from   = &map->tiles[agent->x][agent->y].agent;
    Model_Agent** to     = &map->tiles[x][y].agent;
    if ((*from) == agent && (*to) == NULL)
    {
        agent->x = x;
        agent->y = y;
        *from = NULL;
        *to = agent;
    }
    else
    {
        printf("agent %p from.agent %p to.agent %p\n", agent, *from, *to);
        abort();
    }
}

bool Model_PlaceAgentRandom(Model_Agent* agent)
{
    static int x = 0;
    static int y = 0;
    if(Model_PlaceAgent(agent, x, y++))
    {
        if (y == map->rows)
        {
            y = 0;
            x++;
        }
        if (x == map->cols)
        {
            return false;
        }
        return true;
    }
    return false;
}

//void Model_QueueMessage(Model_Message* msg)
//{
//    /*if (q_count == q_max_size)
//    {
//        DumpQueue();
//        msgqueue = realloc(msgqueue, sizeof(Model_Message) * (q_max_size << 2));
//        if (msgqueue == NULL)
//        {
//            printf("Failed to grow msgqueue from %d messages to %d\n", //q_max_size, q_max_size << 1);
//            abort();
//        }
//        q_max_size <<= 1;
//        q_tail = (q_tail -1 ) % q_max_size;
//        printf("grew msgqueue %p->%p\n", msgqueue, &msgqueue[q_max_size+1]);
//    }*/
//
//    if (q_tail == q_max_size)
//    {
//        printf ("memory violation -- head %d tail %d count %d max %d\n", q_head, q_tail, q_count, q_max_size);
//        abort();
//    }
//
//    memcpy(&msgqueue[q_tail], msg, sizeof(Model_Message));
//    q_tail = (q_tail + 1) % q_max_size;
//    q_count++;
//
//}
void Model_QueueMessage(Model_Message* msg)
{
    Model_Message *in = malloc(sizeof(Model_Message));
    memcpy(in, msg, sizeof(Model_Message));
    Queue_InsertP(&msgqueue, in);
}
/*
Model_Message* Model_GetNextMessage()
{
    Model_Message* ret = NULL;
    if (q_count > 0)
    {
        if (q_head == q_max_size)
        {
            printf ("memory violation -- head %d tail %d count %d max %d\n", q_head, q_tail, q_count, q_max_size);
            abort();
        }
        ret = &msgqueue[q_head];
        q_count--;
        q_head = (q_head + 1) % q_max_size;
    }
    return ret;
}*/

Model_Message* Model_GetNextMessage()
{
    return Queue_Next(&msgqueue);
}
