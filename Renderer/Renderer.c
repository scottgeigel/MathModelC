#include <SDL2/SDL.h>
#include <stdio.h>
static SDL_Window* window;
static SDL_Surface* screen;

//TODO: make a super cool dynamic array
#define MAX_TEXTURES 8
#define MAX_IDENTIFIER 16

static struct
{
    SDL_Surface *textureMap[MAX_TEXTURES];
    char identifiers[MAX_TEXTURES][MAX_IDENTIFIER];
    Renderer_resource_t count;
}Atlas;

static char lastError[80];

static bool InTextureMap(const char* name)
{
    int i;
    for (i = 0; i < Atlas.count; ++i)
    {
        if(strcmp(Atlas[i], name))
            return true;
    }
    return false;
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
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        snprintf(lastError, sizeof(lastError), "SDL Could not initialize. %s", SDL_GetError());
        return Renderer_INIT_FAILED
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
    SDL_FillRect(screen, null, SDL_MapRGB(screen.format, 0xFF, 0xFF, 0xFF));
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
    textureMap[name] = temp;
}

    public void DrawResource(in string what, int x, int y, int scaleFactorX = 1, int scaleFactorY = 1)
    {
        writeln("Blitting " ~ what);
        if (what in textureMap)
        {
            ///TODO:Find out wil you can't scale
            SDL_Surface* surface = textureMap[what];
            writeln(format("%#X", surface));
            SDL_Rect screenDimensions;
            screenDimensions.x = x;
            screenDimensions.y = y;
            screenDimensions.w = screen.w / scaleFactorX;
            screenDimensions.h = screen.h / scaleFactorY;

            if (SDL_BlitScaled(surface, null, screen, &screenDimensions))
            {
                throw new Exception(format("Scaling failed: %s", SDL_GetError()));
            }
        }
        else
        {
            throw new Exception("Could not find " ~ what ~ ". Please use RemoveResource(name) first!");
        }
    }
}
