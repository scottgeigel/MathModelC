#include "Renderer.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

static SDL_Window* window;
static SDL_Surface* screen;

//TODO: make a super cool dynamic array
#define MAX_TEXTURES 8
#define MAX_IDENTIFIER 16

static struct
{
    SDL_Surface *textureMap[MAX_TEXTURES];
    char identifiers[MAX_TEXTURES][MAX_IDENTIFIER];
}Atlas;

static char lastError[80];

static void DEBUG_DumpAtlas()
{
    Renderer_resource_t i;
    printf("[\n");
    for (i = 0; i < MAX_TEXTURES; ++i)
    {
        printf("\t%d: {%p, %s}\n", i, Atlas.textureMap[i], Atlas.identifiers[i]);
    }
}
//TODO:would be better if it just returned a pointer
static Renderer_resource_t FindInTextureMap(const char* name)
{
    Renderer_resource_t i;
    for (i = 0; i < MAX_TEXTURES; ++i)
    {
        if(strncmp(&Atlas.identifiers[i][0], name, MAX_IDENTIFIER) == 0)
            return i;
    }
    return Renderer_resource_ERR;
}

static bool InTextureMap(const char* name)
{
    return FindInTextureMap(name) != Renderer_resource_ERR;
}

static Renderer_resource_t InsertIntoTextureMap(const char* name, SDL_Surface* surface)
{
    //TODO: more efficient search
    Renderer_resource_t i;
    for (i = 0; i < MAX_TEXTURES; ++i)
    {
        if (Atlas.identifiers[i][0] == 0
            && Atlas.textureMap[i] == NULL)
        {
            strcpy(&Atlas.identifiers[i][0], name);
            Atlas.textureMap[i] = surface;
            printf("Inserting %s, %p into %d\n", name, (void*)surface, i);
            return i;
        }
    }
    return Renderer_resource_ERR;
}

static SDL_Surface* RemoveFromTextureMap(Renderer_resource_t idx, const char* name)
{
    SDL_Surface* ret = NULL;
    if (name != NULL)//if extra error checking requested
    {
        if (strcmp(&Atlas.identifiers[idx][0], name))
        {
            return NULL;
        }
    }
    ret = Atlas.textureMap[idx];
    Atlas.textureMap[idx] = NULL;
    Atlas.identifiers[idx][0] = 0;
    return ret;
}

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

Renderer_Error Renderer_Init()
{
    memset(&Atlas.identifiers[0], 0, MAX_TEXTURES*MAX_IDENTIFIER);
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        snprintf(lastError, sizeof(lastError), "SDL Could not initialize. %s", SDL_GetError());
        return Renderer_INIT_FAILED;
    }

    window = SDL_CreateWindow("Zombies vs Humans", SDL_WINDOWPOS_UNDEFINED,
                        SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                        SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if ( window == NULL)
    {
        snprintf(lastError, sizeof(lastError), "SDL could not create window. %s", SDL_GetError());
    }
    screen = SDL_GetWindowSurface(window);
    ///TODO: make this background color customizable
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
}

void Renderer_Quit()
{
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Renderer_Update()
{
    SDL_UpdateWindowSurface(window);
}

/// Adds a resource to the pre-cached list for use later.
/// TODO: make this return bool or something.
Renderer_resource_t Renderer_AddResource(const char* path, const char* name)
{
    if (InTextureMap(name))
    {
        snprintf(lastError, sizeof(lastError), "Attempted to Collide %s:%s with existing entry. Please use RemoveResource(name) first!", path, name);
    }

    SDL_Surface* temp = LoadBMP(path);
    InsertIntoTextureMap(name, temp);
}

void Renderer_DrawResource(const char* what, int x, int y, int scaleFactorX, int scaleFactorY)
{
    //DEBUG_DumpAtlas();
    Renderer_resource_t surfaceIdx = FindInTextureMap(what);
    if (surfaceIdx != Renderer_resource_ERR)
    {
        SDL_Surface* surface = Atlas.textureMap[surfaceIdx];
        SDL_Rect screenDimensions;
        screenDimensions.x = x;
        screenDimensions.y = y;
        screenDimensions.w = screen->w / scaleFactorX;
        screenDimensions.h = screen->h / scaleFactorY;
        printf("Blitting texture #%d %s(%p) at %d,%d size %dx%d\n",surfaceIdx, what, (void*) surface, x, y, screenDimensions.w, screenDimensions.h);
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
