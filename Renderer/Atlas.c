#include "Atlas.h"

void Atlas_Init(Atlas* this)
{
    memset(&this->entries[0], 0, sizeof(Atlas));
}

void Atlas_Dump(Atlas* this)
{
    Atlas_index_t i;
    printf("[\n");
    for (i = 0; i < MAX_TEXTURES; ++i)
    {
        Atlas_Entry *entry = &this->entries[i];
        printf("\t%d: {%p, %s}\n", i, entry->texture, entry->name);
    }
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
        return NULL;//return something else? setjmp?
    }
}

Atlas_index_t Atlas_Insert(Atlas* this, const char* name, SDL_Surface* surface)
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
            printf("Inserting %s, %p into %d\n", name, (void*)surface, i);
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
