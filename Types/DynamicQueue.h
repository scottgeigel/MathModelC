#ifndef __QUEUE_H__
#define __QUEUE_H__
#include <stdbool.h>
struct QueueNode;
typedef struct QueueNode QueueNode;
struct QueueNode
{
    void *data;
    QueueNode *next;
};
typedef struct
{
    QueueNode *head;
    QueueNode *tail;
}Queue;

void Queue_Init(Queue* this);
bool Queue_Empty(Queue* this);
void Queue_InsertP(Queue* this, void *ptr);
void* Queue_Next(Queue* this);
void Queue_Destroy(Queue* this);

#endif
