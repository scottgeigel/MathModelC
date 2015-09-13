#include "Atlas.h"
#include <string.h>
void Atlas_Init(Atlas* this)
{
    memset(&this->entries[0], 0, sizeof(Atlas));
}

void Atlas_Free(Atlas* this)
{
    //nothing to free
}

//TODO:would be better if it just returned a pointer
Atlas_index_t Atlas_FindByName(Atlas* this, const char* name)
{
    Atlas_index_t i;
    for (i = 0; i < MAX_TEXTURES; ++i)
    {
        if(strncmp(&this->entries[i].name[0], name, MAX_IDENTIFIER) == 0)
            return i;
    }
    return Atlas_index_npos;
}

const Atlas_Entry* Atlas_Get(Atlas* this, Atlas_index_t idx)
{
    if (idx < MAX_TEXTURES)
    {
        return &this->entries[idx];
    }
    else
    {
        return NULL;
    }
}

Atlas_index_t Atlas_Insert(Atlas* this, const char* name, texture_t* surface)
{
    //TODO: more efficient search
    Atlas_index_t i;
    for (i = 0; i < MAX_TEXTURES; ++i)
    {
        if (this->entries[i].name[0] == 0
            && this->entries[i].texture == NULL)
        {
            strcpy(&this->entries[i].name[0], name);
            this->entries[i].texture = surface;
            return i;
        }
    }
    return Atlas_index_npos;
}

Atlas_Entry* Atlas_Remove(Atlas* this, Atlas_index_t idx, Atlas_Entry* out)
{
    Atlas_Entry* ret = out;
    if (idx < MAX_TEXTURES)
    {
        memcpy(out, &this->entries[idx], sizeof(Atlas_Entry));
        this->entries[idx].texture = NULL;
        this->entries[idx].name[0] = 0;
    }
    else
    {
        ret = NULL;
    }
    return ret;
}


Atlas_index_t Atlas_Iterator_Start(Atlas* this)
{
    Atlas_Iterator_Next(this, 0);
}

Atlas_index_t Atlas_Iterator_Next(Atlas* this, Atlas_index_t from)
{
    for (from; from < MAX_TEXTURES; ++from)
    {
        if (this->entries[from].texture != NULL)
            return from;
    }
    return Atlas_index_npos;
}
