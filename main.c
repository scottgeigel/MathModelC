#include "Renderer/Renderer.h"
#include "Model/World.h"
#include "InputHandler/InputHandler.h"
#include <stdbool.h>
void main()
{
    Renderer_Init();
    World* world = World_new(100, 50);
    Renderer_resource_t tileIdx = Renderer_AddResource("res/tile.bmp", "tile");
    bool[100][50] changed;
    memset(changed, true, sizeof(bool) * 100 * 50);

    While(!InputHandler_ExitRequested())
    {
        bool changeFrame = false;
        for (unsigned x = 0; x < world->width; ++x)
        {
            for (unsigned y = 0; y < world->height; ++y)
            {
                if (changed[x][y])
                {
                    Renderer_DrawResource(tileIdx, x * (SCREEN_WIDTH / 100), y * (SCREEN_HEIGHT / 50), 100, 50);
                    changed[x][y] = false;
                    changeFrame = true;
                }
            }
        }

        if (changeFrame)
            Renderer_Update();
    }
    World_delete(world);
    Renderer_Quit();
}
