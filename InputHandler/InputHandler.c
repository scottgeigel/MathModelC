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
    }
}

bool InputHandler_ExitRequested()
{
    ScanInputs();
    return statusFlags[EXIT_REQUESTED];
}
