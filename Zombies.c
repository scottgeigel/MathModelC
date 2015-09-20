#include "Zombies.h"
#include "Model/Model.h"
#include "Configuration.h"
#include <stdlib.h>

/**
    Zombie Agent

    A Zombie has xx attributes
        * alive All zombies can only be killed with a HeadShot
        * sight (3) Zombies can see 3 blocks around them
        * hearing TODO implement?
        * speed (1-2) how many actions a zombie can perform per iteration. This can be
          moving or attacking their target.
        * Deadliness (0-100%) how effective they are with their weapon. This is based on a
          bell curve, so realistically no one has 0% or 100% accuracy.
    Zombie Rules
        if nothing near by
            set heading to random as if searching for humans
        if human near by
            set heading for nearest human

    Transitions
        if hit, stay in current compartment and temporarily substract 1 from speed for stopping power of weapon
        if headshot, transition to dead
*/
typedef struct
{
    Model_Agent super;
    bool alive
    int deadliness;
    int speed;

}Zombie;

static Zombie* zombies;

static int zombiePopulation = 0;

static void TransitionToDead(Zombie* this)
{
    //implement
}

static void Agenda(Model_Agent* this, const Model_Map* map)
{
    //build line of sight. Maps do not wrap
}

static void MessageHandler(Model_Agent* this, const char* message)
{

}

static void Zombie_Init(Zombie* this)
{
    this->super.class = ZOMBIE_CLASS;
    this->super.agenda = Agenda;
    this->super.messageHandler = MessageHandler;
    this->alive = true;
    this->speed = Model_Random_Between(1, 2);
}

void ZombiesInit(int howMany)
{
    const int ZOMBIE_COUNT = howMany; // todo make parameter
    int i;
    zombies = calloc(ZOMBIE_COUNT, sizeof(Zombie));
    for (i = 0; i < ZOMBIE_COUNT; i++)
    {
        Zombie_Init(&zombies[i]);
        if(!Model_PlaceAgentRandom(&zombies[i].super))
        {
            //TODO: make a function for this
            fprintf(stderr, "Error at %s:%d in %s\n%p could not be placed\n", __FILE__, __LINE__, __FUNCTION__, &zombies[i]);
            abort();
        }
    }
    Model_Grapher_AddColDef("Zombies");
}

void ZombiesEnd()
{
    free(zombies);
    zombies = NULL;
}
