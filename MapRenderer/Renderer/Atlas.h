/**
    @file Atlas
    @author Scott Geigel

    Implements a Texture Atlas for storing named graphical resources.
*/
#ifndef __ATLAS_H__
#define __ATLAS_H__

typedef void texture_t;

/**
    Temporary limitation
    @TODO  make a super cool dynamic array
*/
#define MAX_TEXTURES 8
#define MAX_IDENTIFIER 16
typedef struct
{
    texture_t* texture;
    char name[MAX_IDENTIFIER];
}Atlas_Entry;

typedef struct
{
    Atlas_Entry entries[MAX_TEXTURES];
}Atlas;

typedef unsigned Atlas_index_t;
#define Atlas_index_npos (0u - 1)

void Atlas_Init(Atlas* this);
void Atlas_Free(Atlas* this);

Atlas_index_t Atlas_FindByName(Atlas* this, const char* name);
const Atlas_Entry* Atlas_Get(Atlas* this, Atlas_index_t idx);
Atlas_index_t Atlas_Insert(Atlas* this, const char* name, texture_t* surface);
Atlas_Entry* Atlas_Remove(Atlas* this, Atlas_index_t idx, Atlas_Entry* out);

Atlas_index_t Atlas_Iterator_Start(Atlas* this);
Atlas_index_t Atlas_Iterator_Next(Atlas* this, Atlas_index_t from);
#endif
