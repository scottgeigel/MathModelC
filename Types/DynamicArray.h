#ifndef __DYNAMIC_ARRAY_H__
#define __DYNAMIC_ARRAY_H__
typedef struct
{
    void* data;
    int count;
    size_t size;
    size_t elementSize;
}Array;

void Array_Init(Array* this, size_t initialSize, size_t elementSize);
const void* Array_Get(Array* this, int index);
const void Array_Insert(Array* this, void* item);
const void* Array_Remove(Array* this, int index, void* buffer);
#endif
