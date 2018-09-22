#include "Configuration.h"
#include "MapRenderer/Renderer/Renderer.h"
void LoadConfiguration()
{
    Renderer_AddResource(PATH_TO_ZOMBIE_IMG, ZOMBIE_CLASS);
    Renderer_AddResource(PATH_TO_HUMAN_IMG, HUMAN_CLASS);
    Renderer_AddResource(PATH_TO_DEFAULT_TILE_IMG, "tile");
}
