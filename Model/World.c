#include "Model.h"
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
void Model_Map_Init(Model_Map* this, int cols, int rows, bool wraps)
{
    int i,j;
    this->wraps = wraps;
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

void Model_Map_Free(Model_Map* this)
{
    int i;
    for (i = 0; i < this->cols; ++i)
    {
        free(this->tiles[i]);
        this->tiles[i] = NULL;
    }
    free(this->tiles);
    this->tiles = NULL;
}

size_t Model_Map_ScanArea(const Model_Map* this, const Model_Tile* tiles, int x, int y, int radius)
{
    int start_x, start_y, stop_x, stop_y;
    size_t tiles_pushed = 0;

    start_x = x - radius;
    stop_x = x + radius + 1;

    start_y = y - radius;
    stop_y = y + radius + 1;

    if (this->wraps)
    {
        start_x = ((start_x % this->cols) + this->cols) % this->cols;
        stop_x = ((stop_x % this->cols) + this->cols) % this->cols;
        start_y = ((start_y % this->rows) + this->rows) % this->rows;
        stop_y = ((stop_y % this->rows) + this->rows) % this->rows;
    }
    else
    {
        start_x = (start_x > 0)? start_x : 0;
        stop_x = (stop_x > this->cols)? stop_x : this->cols;
        start_y = (start_y > 0)? start_y : 0;
        stop_y = (stop_y > this->rows)? stop_y : this->rows;
    }

    for (int target_x = start_x; target_x != stop_x; target_x = (target_x+1) % this->cols)
    {
        for (int target_y = start_y; target_y != stop_y; target_y = (target_y+1)%this->rows)
        {
            //push the agent onto the result lists
            Model_Tile* coerced_tiles = &tiles[tiles_pushed];
            const Model_Tile* target_tile = &this->tiles[target_x][target_y];

            memcpy(coerced_tiles, target_tile, sizeof(Model_Tile));
            tiles_pushed++;
        }
    }

    return tiles_pushed;
}
