#include "ConwayCell.h"

static void Agenda(Model_Agent* this, const Model_Map* map)
{

}

static void MessageHandler(Model_Agent* this, const char* message)
{

}

void ConwayCell_Init(ConwayCell* this)
{
    Model_Agent_Init(this, "Conway Cell");
    this->agenda = Agenda;
    this->messageHandler = MessageHandler;
}
