#include "Renderer.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

static SDL_Window* window;
static SDL_Surface* screen;
static int SCREEN_HEIGHT;
static int SCREEN_WIDTH;

static char lastError[80];
static Atlas atlas;

static SDL_Surface* LoadBMP(const char* path)
{
    SDL_Surface* optimizedSurface = NULL;
    SDL_Surface* loadedSurface = SDL_LoadBMP(path);

    if (loadedSurface == NULL)
    {
        snprintf(lastError, sizeof(lastError), "Could not load %s\nSDL Error: %s", path, SDL_GetError());
    }
    else
    {
        ///TODO:move to Optimize function
        optimizedSurface = SDL_ConvertSurface(loadedSurface, screen->format, 0);

        if (optimizedSurface == NULL)
        {
            snprintf(lastError, sizeof(lastError), "Could load, but could not   optimize %s\nSDL Error: %s", path, SDL_GetError());
        }
        SDL_FreeSurface(loadedSurface);
    }

    return optimizedSurface;
}

Renderer_Error Renderer_Init(int xres, int yres)
{
    SCREEN_HEIGHT = yres;
    SCREEN_WIDTH  = xres;
    Renderer_Error ret = Renderer_SUCCESS;

    Atlas_Init(&atlas);
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        snprintf(lastError, sizeof(lastError), "SDL Could not initialize. %s", SDL_GetError());
        ret = Renderer_INIT_FAILED;
    }
    else
    {
        window = SDL_CreateWindow("Zombies vs Humans", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                            SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if ( window == NULL)
        {
            snprintf(lastError, sizeof(lastError), "SDL could not create window. %s", SDL_GetError());
            ret = Renderer_WINDOW_FAILED;
        }
        screen = SDL_GetWindowSurface(window);
        ///TODO: make this background color customizable
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
    }
    return ret;
}

/// Adds a resource to the pre-cached list for use later.
/// TODO: make this return bool or something.
Atlas_index_t Renderer_AddResource(const char* path, const char* name)
{
    Atlas_index_t idx = Atlas_FindByName(&atlas, name);
    if (idx != Atlas_index_npos)
    {
        snprintf(lastError, sizeof(lastError), "Attempted to Collide %s:%s with existing entry. Please use RemoveResource(name) first!", path, name);
    }
    else
    {
        SDL_Surface* temp = LoadBMP(path);
        idx = Atlas_Insert(&atlas, name, temp);
    }
    return idx;
}

void Renderer_Quit()
{
    Atlas_Entry tmp;
    Atlas_index_t idx = Atlas_Iterator_Start(&atlas);
    while(idx != Atlas_index_npos)
    {
        Atlas_Remove(&atlas, idx, &tmp);
        SDL_FreeSurface(tmp.texture);
        idx = Atlas_Iterator_Next(&atlas, idx);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Renderer_Update()
{
    SDL_UpdateWindowSurface(window);
}

void Renderer_DrawResource(Atlas_index_t idx, int x, int y, int scaleFactorX, int scaleFactorY)
{
    if (idx != Atlas_index_npos)
    {
        const Atlas_Entry *entry = Atlas_Get(&atlas, idx);
        SDL_Surface* surface = entry->texture;
        SDL_Rect screenDimensions;
        screenDimensions.x = x;
        screenDimensions.y = y;
        screenDimensions.w = screen->w / scaleFactorX;
        screenDimensions.h = screen->h / scaleFactorY;
        //printf("Blitting texture #%d %s(%p) at %d,%d size %dx%d\n",
        //        idx, entry->name, (void*) entry->texture,
        //         x, y, screenDimensions.w, screenDimensions.h);
        if (SDL_BlitScaled(surface, NULL, screen, &screenDimensions))
        {
            snprintf(lastError, sizeof(lastError), "Scaling failed: %s", SDL_GetError());
        }
    }
    else
    {
        snprintf(lastError, sizeof(lastError), "Could not find %s. Pleas use AddResource(name, path) first.", SDL_GetError());
    }
}

const char* Renderer_GetLastError()
{
    return lastError;
}

Atlas* Renderer_GetAtlas()
{
    return &atlas;
}
