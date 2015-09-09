#ifndef __AGENT_H__
#define __AGENT_H__

struct Model_Map;
typedef struct Model_Map Model_Map;

struct Model_Agent;
typedef struct Model_Agent Model_Agent;

typedef void (*Model_Agent_Agenda)(Model_Agent* this, const Model_Map* map);
typedef void (*Model_Agent_MessageHandle)(Model_Agent* this, const char* message);
struct Model_Agent
{
    int x;
    int y;
    const char *class;
    Model_Agent_Agenda agenda;
    Model_Agent_MessageHandle messageHandler;
};

void Model_Agent_Init(Model_Agent* this, const char* class);
#endif
