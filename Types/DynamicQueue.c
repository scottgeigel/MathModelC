#include "DynamicQueue.h"
#include <stdlib.h>
void Queue_Init(Queue* this)
{
    this->head = NULL;
    this->tail = NULL;
}
bool Queue_Empty(Queue* this)
{
    return this->head == NULL;
}
void Queue_InsertP(Queue* this, void *ptr)
{
    QueueNode* newNode = malloc(sizeof(QueueNode));
    newNode->data = ptr;
    newNode->next = NULL;
    if (this->head == NULL)
    {
        this->head = this->tail = newNode;
    }
    else
    {
        this->tail->next = newNode;
        this->tail = newNode;
    }
}

void* Queue_Next(Queue* this)
{
    void * ret = NULL;
    QueueNode *tmp = this->head;
    if (tmp == NULL)
    {
        return NULL;
    }
    ret = tmp->data;
    this->head = tmp->next;
    tmp->next = NULL;
    tmp->data = NULL;
    free(tmp);
    return ret;
}
void Queue_Destroy(Queue* this)
{
    while(Queue_Next(this) != NULL);
    this->head = NULL;
    this->tail = NULL;
    if(!Queue_Empty(this)) abort();
}
