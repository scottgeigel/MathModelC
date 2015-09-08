#include "MapRenderer.h"
#include "Renderer/Renderer.h"
#include <stdbool.h>

static struct
{
    int resolutionHeight;
    int resolutionWidth;
    int tileSizeX;
    int tileSizeY;
    int startX;
    int startY;
    const Model_Map* map;
    Atlas_index_t **lastDrawn;
}ScreenInfo;

Atlas_index_t debug;

static Model_Map* map = NULL;

void StartScene(const Model_Map* map, int xres, int yres)
{
    debug = Renderer_AddResource("res/tile.bmp", "tile");

    int remy,remx; //remainders if tiles to not fit perfectly
    int i;

    remx = xres % map->cols;
    remy = yres % map->rows;
    ScreenInfo.resolutionHeight = yres;
    ScreenInfo.resolutionWidth  = xres;
    ScreenInfo.tileSizeX = xres / map->cols;
    ScreenInfo.tileSizeY = yres / map->rows;
    ScreenInfo.startX = remx / 2; //start halfway over
    ScreenInfo.startY = remy / 2;
    ScreenInfo.map = map;

    ScreenInfo.lastDrawn = calloc(map->cols, sizeof(Atlas_index_t *));
    for (i = 0; i < map->cols; ++i)
    {
        ScreenInfo.lastDrawn[i] = calloc(map->rows, sizeof(Atlas_index_t));
        memset(ScreenInfo.lastDrawn[i], Atlas_index_npos, map->rows * sizeof(Atlas_index_t));
    }
}

void DrawMap(void)
{

    int x,y,c,r;//x position, y position, collumn index, row index
    const Model_Map * const map = ScreenInfo.map;
    x = ScreenInfo.startX;
    bool frameChanged = false;
    for (c = 0; c < ScreenInfo.map->cols; ++c)
    {
        y = ScreenInfo.startY;
        for (r = 0; r < ScreenInfo.map->rows; ++r)
        {
            if (ScreenInfo.lastDrawn[c][r] != debug)
            {
                frameChanged = true;
                ScreenInfo.lastDrawn[c][r] = debug;
                Renderer_DrawResource(debug, x, y, map->cols, map->rows);
            }
            y += ScreenInfo.tileSizeY;
        }
        x += ScreenInfo.tileSizeX;
    }
    if (frameChanged)
        Renderer_Update();
}
