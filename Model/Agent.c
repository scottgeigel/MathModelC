#include "Model.h"
#include <stdlib.h>
void Model_Agent_Init(Model_Agent* this, const char* class)
{
    this->x                 = -1;
    this->y                 = -1;
    this->class             = class;
    this->agenda            = NULL;
    this->messageHandler    = NULL;
}
