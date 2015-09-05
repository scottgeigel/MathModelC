/**
    @file Renderer.h
    Introduces public interface to the underlying Renderer implementation.
*/
#ifndef __RENDERER_H__
#define __RENDERER_H__
#define SCREEN_HEIGHT   (1080u)
#define SCREEN_WIDTH    (1920u)

/**
    Enum descriptors of errors for the Renderer Module
*/
typedef enum
{
    ///init errors
    Renderer_INIT_FAILED, ///SDL failed to init video
    Renderer_WINDOW_FAILED, //Failed to create a window
}Renderer_Error;

typedef unsigned Renderer_resource_t;
/**
    Initializes the SDL renderer for the program.

    @returns
*/
Renderer_Error Renderer_Init(void);
Renderer_resource_t Renderer_AddResource(const char* path, const char* name);
void Renderer_Quit(void);
void Renderer_Update(void);
const char* Renderer_GetLastError(void);
#endif
