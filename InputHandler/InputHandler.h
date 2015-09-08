#ifndef __INPUT_HANDLER_H__
#define __INPUT_HANDLER_H__
#include <stdbool.h>
/*
    @defgroup InputHandler Input Handler
    @ingroup InputHandler
    @{
    @file InputHandler.h
    @brief Handles input and window events
*/
typedef enum
{
    EXIT_REQUESTED = 0,
    NUM_EVENTS,
}InputHandler_Events;

/**
    @brief Determines whether application was requested to be terminated
    @returns bool
    @retval true - Application termination requested
    @retval false - Termination not requested
*/
bool InputHandler_ExitRequested();
/**
@}
*/
#endif
