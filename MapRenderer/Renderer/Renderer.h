/**
    @file Renderer.h
    Introduces public interface to the underlying Renderer implementation.
*/
#ifndef __RENDERER_H__
#define __RENDERER_H__
#include "Atlas.h"
/**
    Enum descriptors of errors for the Renderer Module
*/
typedef enum
{
    Renderer_SUCCESS,
    ///init errors
    Renderer_INIT_FAILED, ///SDL failed to init video
    Renderer_WINDOW_FAILED, //Failed to create a window
    Renderer_NO_TEXTURE,//cache lookup didn't contain a texture
}Renderer_Error;

/**
    Initializes the SDL renderer for the program.

    @returns
*/
Renderer_Error Renderer_Init(int xres, int yres);
Atlas_index_t Renderer_AddResource(const char* path, const char* name);
void Renderer_Quit(void);
void Renderer_Update(void);
const char* Renderer_GetLastError(void);
void Renderer_DrawResource(Atlas_index_t idx, int x, int y, int scaleFactorX, int scaleFactorY);
const char* Renderer_GetLastError();
Atlas* Renderer_GetAtlas();
#endif
