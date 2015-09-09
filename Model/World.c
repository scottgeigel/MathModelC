#include "Model.h"
#include <stdlib.h>
#include <string.h>
void Model_Map_Init(Model_Map* this, int cols, int rows)
{
    int i,j;
    this->rows = rows;
    this->cols  = cols;
    this->tiles = calloc(cols, sizeof(Model_tile_t*));
    for (i = 0; i < cols; ++i)
    {
        this->tiles[i] = malloc(rows * sizeof(Model_tile_t));
        for (j = 0; j < rows; ++j)
        {
            this->tiles[i][j] = (Model_Tile){.type = Model_tile_default, .agent = NULL};
        }
    }
}
