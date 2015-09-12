#include "InputHandler.h"
#include <SDL2/SDL.h>

static bool statusFlags[NUM_EVENTS] = {false};

static void ScanInputs()
{
    SDL_Event e;

    while(SDL_PollEvent(&e) != 0)
    {
        //check for our exit
        if (e.type == SDL_QUIT)
        {
            statusFlags[EXIT_REQUESTED] = true; //cannot be cleared
        }
        else if (e.type == SDL_KEYDOWN) //key presses
        {
            if (e.key.repeat > 0)//ignore???
                continue;
            switch (e.key.keysym.sym)
            {
                case SDLK_SPACE:
                    statusFlags[NEXTSTEP_REQUESTED] = true;
            }
        }
    }
}

bool InputHandler_ExitRequested()
{
    ScanInputs();
    return statusFlags[EXIT_REQUESTED];
}

bool InputHandler_NextStepRequested()
{
    ScanInputs();
    bool ret = statusFlags[NEXTSTEP_REQUESTED];
    statusFlags[NEXTSTEP_REQUESTED] = false;
    return ret;
}
