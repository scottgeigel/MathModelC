#ifndef __INPUT_HANDLER_H__
#define __INPUT_HANDLER_H__
#include <stdbool.h>
typedef enum
{
    EXIT_REQUESTED = 0,
    NUM_EVENTS,
}InputHandler_Events;

bool InputHandler_ExitRequested();
#endif
