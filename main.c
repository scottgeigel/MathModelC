#include "Model/Model.h"
#include "InputHandler/InputHandler.h"
#include "MapRenderer.h"
void main()
{
    const int screenHeight  = 600;
    const int screenWidth   = 800;
    Model_Map map;
    Renderer_Init(screenWidth, screenHeight);
    Model_Map_Init(&map, 30, 25);

    StartScene(&map, screenWidth, screenHeight);
    while(!InputHandler_ExitRequested())
    {
        DrawMap();
    }
    //World_delete(world);
    Renderer_Quit();
}
/*
Renderer_resource_t tileIdx = Renderer_AddResource("res/tile.bmp", "tile");
bool changed[col][row];
memset(changed, true, sizeof(bool) * col * row);

int i,j;
unsigned x,y;
unsigned skipx = 0,skipy = 0, tileSizex, tileSizey;
bool changeFrame = false;
int remx,remy;
int nextXThreshold = 0, nextYThreshold = 0;
tileSizex = (SCREEN_WIDTH / col);
remx = (SCREEN_WIDTH % col);
tileSizey = (SCREEN_HEIGHT / row);
remy = (SCREEN_HEIGHT % row);

if (remx)
{
    printf("%d x pixels left over\n", remx);
    skipx = SCREEN_WIDTH / remx;
}
if (remy)
{
    printf("%d y pixels left over\n", remy);
    skipy = SCREEN_HEIGHT/ remy;
}
x = remx/2;

for (i = 0; i < col; ++i)
{
    if (0 && skipx && (x > nextXThreshold))
    {
        x++;
        nextXThreshold += skipx;
    }
    y = remy/2;
    nextYThreshold = 0;
    for (j = 0; j < row; ++j)
    {
        if(0 && skipy && (y>nextYThreshold))
        {
            nextYThreshold += skipy;
            ++y;
        }

        if (changed[i][j])
        {
            Renderer_DrawResource("tile", x, y, col, row);
            changed[i][j] = false;
            changeFrame = true;
        }
        y+=tileSizey;
    }
    x+=tileSizex;
}

if (changeFrame)
    Renderer_Update();*/
