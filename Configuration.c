#include "Configuration.h"
#include "MapRenderer/Renderer/Renderer.h"
void LoadConfiguration()
{
    Renderer_AddResource(PATH_TO_CONWAY_CELL_IMG, CONWAY_CELL_CLASS);
    Renderer_AddResource(PATH_TO_DEAD_CELL_IMG, DEAD_CELL_CLASS);
    Renderer_AddResource(PATH_TO_DEFAULT_TILE_IMG, "tile");
}
