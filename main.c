#include "Configuration.h"
#include "Model/Model.h"
#include "InputHandler/InputHandler.h"
#include "MapRenderer/Renderer/Renderer.h" //TODO: move this up a level
#include "MapRenderer/MapRenderer.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct
{
    bool printUsage;
    bool singleStep;
    bool graph;
    int mapHeight;
    int mapWidth;
}Options;

typedef enum
{
    SUCCESS,
    INVALID_PARAM,
    MISSING_PARAM
}OptionError;

struct Option
{
    const char* longOption;
    const char* shortOption;
    const char* about;
    OptionError (*callback)(int* , const char **);
};

static bool ValidateBase10(const char* num)
{
    if (*num == '\0') return false;
    while(*num != '\0')
    {
        if ((*num > '9') || (*num < '0'))
        {
            printf("%c is not a number\n", *num);
            return false;
        }
        num++;
    }
    return true;
}

static OptionError HelpCallback(int* argsLeft, const char ** args)
{
    printf("HelpCallback\n");
    Options.printUsage = true;
    return SUCCESS;
}

static OptionError SingleStepCallback(int* argsLeft, const char ** args)
{
    printf("SingleStepCallback\n");
    Options.singleStep = true;
    return SUCCESS;
}

static OptionError HeightCallback(int* argsLeft, const char ** args)
{
    printf("HeightCallback %d\n", *argsLeft);
    if ((*argsLeft) == 0)
        return MISSING_PARAM;
    (*argsLeft)++;
    printf("Parsing %s\n", args[*argsLeft]);
    Options.mapHeight = atoi(args[*argsLeft]);
    if(ValidateBase10(args[*argsLeft]))
        return SUCCESS;
    else
        return INVALID_PARAM;
}

static OptionError WidthCallback(int* argsLeft, const char ** args)
{
    printf("WidthCallback\n");
    if (*argsLeft == 0)
        return MISSING_PARAM;
    (*argsLeft)++;
    Options.mapWidth = atoi(args[*argsLeft]);
    if(ValidateBase10(args[*argsLeft]))
        return SUCCESS;
    else
        return INVALID_PARAM;
}

static OptionError GraphCallback(int* argsLeft, const char ** args)
{
    Options.graph = true;
    return SUCCESS;
}
struct Option options[] =
{
        {"--help", "-h", "Print this message", HelpCallback},
        {"--singleStep", "-s", "Wait for the space key before running the next iteration of the model. Default is False", SingleStepCallback},
        {"--height", "-H", "[--height|-y] y   where y is the number of vertical tiles. Default is 10", HeightCallback},
        {"--width", "-W", "[--width|-x] x    where x is the number of horizontal tiles. Default is 10", WidthCallback},
        {"--graph", "-g", "[--graph|-g] Enables graphing in CSV format.", GraphCallback}
};
bool ParseArgv(int argc, const char *argv[])
{
    const int stop = sizeof(options) / sizeof(struct Option);
    int argidx;
    bool error = false;
    Options.printUsage    = false;
    Options.singleStep    = false;
    Options.graph         = false;
    Options.mapHeight     = 10;
    Options.mapWidth      = 10;

    for(argidx = 1; argidx < argc; argidx++)
    {
        //find option
        int i;
        OptionError e;
        for (i = 0; i < stop; i++)
        {
            if ((strcmp(options[i].longOption, argv[argidx]) == 0)
                || (strcmp(options[i].shortOption, argv[argidx]) == 0))
            {
                e = options[i].callback(&argidx, argv);
                break;
            }
        }
        if (e != SUCCESS)
        {
            switch(e)
            {
                case INVALID_PARAM:
                    printf("Invalid parameter %s for %s, %s\n", argv[argidx-1], options[i].longOption, options[i].shortOption);
                break;
                case MISSING_PARAM:
                    printf("Missing parameter for %s, %s\n", options[i].longOption, options[i].shortOption);
                break;
                default:
                    printf("Undefined error occurred\n");
                break;
            }
            error = true;
            break;
        }
        if (i == stop)
        {
            printf("Unrecognized option: %s\n", argv[argidx]);
            error = true;
            break;
        }
    }
    return error;
}

static __attribute__((noreturn)) void PrintUsage(const char* programName)
{
    const int stop = sizeof(options) / sizeof(struct Option);
    char fmt[32];
    int i;

    if (strstr(programName, "./") != NULL)
        programName += sizeof("./") - 1;

    int longest = 0;
    for (i = 0; i < stop; i++)
    {
        int t = strlen(options[i].longOption);
        if (t > longest)
            longest = t;
    }

    printf("%s\n", programName);
    for (i = 0; i < stop; i++)
    {
        snprintf(fmt, sizeof(fmt), "    %%-%ds  %%s    %%s\n", longest);

        printf(fmt, options[i].longOption, options[i].shortOption, options[i].about);
    }
    exit(0);
}

static __attribute__((noreturn)) void StartModel()
{
    const int screenHeight  = 768;
    const int screenWidth   = 1024;
    const bool singleStep   = Options.singleStep;
    Model_Map map;

    if (Renderer_Init(screenWidth, screenHeight) != Renderer_SUCCESS) //TODO: move this up a level
    {
        printf(Renderer_GetLastError());
        exit(1);
    }
    LoadConfiguration();

    Model_Map_Init(&map, Options.mapWidth, Options.mapHeight);
    Model_Init(&map);

    MapRenderer_StartScene(&map, screenWidth, screenHeight);

    while(!InputHandler_ExitRequested())
    {
        if(!singleStep || InputHandler_NextStepRequested())
        {
            Model_Next();
            if (Options.graph)
                Model_GraphIteration();
        }
        MapRenderer_DrawMap();
    }
    MapRenderer_EndScene();
    Model_Map_Free(&map);
    Model_Free();
    Renderer_Quit();//TODO: move this up a level
    exit(0);
}

void main(int argc, const char *argv[])
{
    if (ParseArgv(argc, argv))
        exit(1);
    if (Options.printUsage)
        PrintUsage(argv[0]);
    else
        StartModel();
    exit(0);
}
