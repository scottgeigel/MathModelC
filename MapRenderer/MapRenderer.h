/**
    @file MapRenderer.h"
    @author Scott Geigel
    @brief Glue between the Model_Map and Renderer
*/
#ifndef __MAP_RENDERER_H__
#define __MAP_RENDERER_H__
#include "Model/Model.h"
/**
    @brief Initializes MapRenderer with first pass compilation

    There is a one to one relationship between MapRenderer and Model_Map
*/
void MapRenderer_StartScene(const Model_Map* map, int xres, int yres);
/**
    @brief Draws the map to the screen

    Updates map compilation if necessary
*/
void MapRenderer_DrawMap(void);
void MapRenderer_EndScene();
#endif
