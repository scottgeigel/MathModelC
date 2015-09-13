#include "Model.h"
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
void Model_Map_Init(Model_Map* this, int cols, int rows)
{
    int i,j;
    this->rows = rows;
    this->cols  = cols;
    this->tiles = calloc(cols, sizeof(Model_Tile*));
    for (i = 0; i < cols; ++i)
    {
        this->tiles[i] = malloc(rows * sizeof(Model_Tile));
        for (j = 0; j < rows; ++j)
        {
            this->tiles[i][j].type = Model_tile_default;
            this->tiles[i][j].agent = NULL;
        }
    }
}

bool Model_Map_HasAgent(const Model_Map* this, int x, int y)
{
    //should this have an exception/setjmp of some kind instead?
    if ((x < 0) || (x >= this->cols) || (y < 0) || (y >= this->rows) ||
        this->tiles[x][y].agent == NULL)
    {
        return false;
    }
    else
    {
        return true;
    }
}

Model_Agent* Model_Map_GetAgent(const Model_Map* this, int x, int y)
{
    //should this have an exception/setjmp of some kind instead?
    if ((x < 0) || (x >= this->cols) || (y < 0) || (y >= this->rows))
    {
        //printf("%d, %d invalid\n", x, y);
        return NULL;
    }
    else
    {
        //printf("Returning %p\n", this->tiles[x][y].agent);
        return this->tiles[x][y].agent;
    }
}
