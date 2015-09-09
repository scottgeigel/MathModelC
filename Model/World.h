#ifndef __WORLD_H__
#define __WORLD_H__
struct Model_Agent;
typedef struct Model_Agent Model_Agent;

typedef unsigned int Model_tile_t;
#define Model_tile_default ((Model_tile_t) 0)

struct Model_Tile;
typedef struct Model_Tile Model_Tile;
struct Model_Tile
{
    Model_tile_t type;
    Model_Agent* agent;
};

struct Model_Map;
typedef struct Model_Map Model_Map;
struct Model_Map
{
    int rows;
    int cols;
    Model_Tile **tiles;
};

/**
    @TODO find a way to have this map generated by a file
*/
void Model_Map_Init(Model_Map* this, int cols, int rows);
#endif
