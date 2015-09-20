#include "Humans.h"
#include "Model/Model.h"
#include "Configuration.h"
#include <stdlib.h>

/**
    Human Agent

    A Human has xx attributes
        * health (-100 to 100) All humans start with 100. At zero, the human is dead
        * sight (5) Humans can see 5 blocks around them
        * speed (1-3) how many actions a human can perform per iteration. This can be
          moving or firing their weapon.
        * Accuracy (0-100%) how effective they are with their weapon. This is based on a
          bell curve, so realistically no one has 0% or 100% accuracy.
            - there will be 2 numbers: HeadShot Accuracy and General Accuracy
        * Courage (0-100%) represents how likely the person is to stand and fight or, if when infected,
          the human decides to be noble and avoid others to protect them. expressed
          as:
            Bravery(): (courage * fudge * (humans per 5 block radius))
                            - ((1 - courage) * (zombies per 5 block radius))
            where fudge is a function not to exceed 100%
                disabling fudge would be a constant of 1
        * infection timer (-1 to 5) if not infected, the timer will read -1. If infected the timer
          will initialize to a random number from 1 to 5. It will then substract after every model
          itteration (not every move since humans can have up to 3) until 0. At 0, they will transition
          to zombie.
    Human Rules
        if nothing near by
            set heading to random as if searching for supplies
        if human near by
            if infected and Bravery > 2, set heading to path with least humans
            if infected and Bravery < 2, or not infected, set heading to nearest human
        if zombie near by, calculate will fight
            if will Bravery > 1 fire round
            if will Bravery < 1 set heading to path with least zombies

    Transitions
        if hit, stay in current compartment and subtract from health a random number between 5 and 25
        if bit, transition to infected, initialize timer to a number between 1 and 5, and substract
         25 health. Note the chance of a zombie biting vs inflicting other damage is less than 1
        if health reaches 0 or less before infection timer equals 0, transition straight to dead.
*/
typedef struct
{
    Model_Agent super;
    int health;
    int infectionTimer;
    int speed;

}Human;

static Human* humans;

static int humanPopulation = 0;

static void TransitionToInfected(Human* this)
{
    //implement
}

static void TransitionToZombie(Human* this)
{
    //implement
}

static void TransitionToDead(Human* this)
{
    //implement
}

static void Agenda(Model_Agent* this, const Model_Map* map)
{
}

static void MessageHandler(Model_Agent* this, const char* message)
{

}

static void Human_Init(Human* this)
{
    this->super.class = HUMAN_CLASS;
    this->super.agenda = Agenda;
    this->super.messageHandler = MessageHandler;
    this->infectionTimer = -1;
    this->speed = Model_Random_Between(1, 3);
}

void HumansInit(int howMany)
{
    const int HUMAN_COUNT = howMany; // todo make parameter
    int i;
    humans = calloc(HUMAN_COUNT, sizeof(Human));
    for (i = 0; i < HUMAN_COUNT; i++)
    {
        Human_Init(&humans[i]);
        if(!Model_PlaceAgentRandom(&humans[i].super))
        {
            //TODO: make a function for this
            fprintf(stderr, "Error at %s:%d in %s\n%p could not be placed\n", __FILE__, __LINE__, __FUNCTION__, &humans[i]);
            abort();
        }
    }
    Model_Grapher_AddColDef("Humans");
}

void HumansEnd()
{
    free(humans);
    humans = NULL;
}
