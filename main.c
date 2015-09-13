#include "Model/Model.h"
#include "InputHandler/InputHandler.h"
#include "MapRenderer.h"
void main()
{
    const int screenHeight  = 600;
    const int screenWidth   = 800;
    bool singleStep = true;
    Model_Map map;
    Renderer_Init(screenWidth, screenHeight);
    LoadConfiguration();

    Model_Map_Init(&map, 10, 12);
    Model_Init(&map);

    StartScene(&map, screenWidth, screenHeight);
    ConwaysGameOfLife_Init(&map);
    while(!InputHandler_ExitRequested())
    {
        if(!singleStep || InputHandler_NextStepRequested())
            Model_Next();
        DrawMap();
    }
    ConwayCell_Free();
    EndScene();
    Model_Map_Free(&map);
    Model_Free();
    Renderer_Quit();
}
