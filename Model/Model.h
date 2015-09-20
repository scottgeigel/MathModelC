#ifndef __MODEL_H__
#define __MODEL_H__
#include "Agent.h"
#include "World.h"
#include "Random.h"
#include "Grapher.h"
#define MAX_MESSAGES 256 //TODO: fix this
typedef struct
{
    const char* message;
    Model_Agent* effected;
}Model_Message;

void Model_Init(Model_Map* _map);
void Model_Free();
void Model_Next();
void Model_GraphIteration();
bool Model_PlaceAgent(Model_Agent* agent, int x, int y);
bool Model_PlaceAgentRandom(Model_Agent* agent);
bool Model_MoveAgent(Model_Agent* agent, int x, int y);
void Model_QueueMessage(Model_Message* msg);
Model_Message* Model_GetNextMessage();
#endif
