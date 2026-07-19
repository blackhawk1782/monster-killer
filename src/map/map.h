#ifndef MK_MAP_H
#define MK_MAP_H

#include "types.h"

typedef enum {
    CELL_EMPTY = 0,
    CELL_WALL = 1,
    CELL_DOOR = 2,
    CELL_SECRET = 3,
    CELL_WINDOW = 4,
    CELL_SPAWN = 5,
    CELL_ENEMY = 6,
    CELL_BOSS = 7,
    CELL_PICKUP = 8,
    CELL_DECORATION = 9,
    CELL_LIGHT = 10
} CellType;

typedef struct {
    u8 grid[MK_MAX_MAP_WIDTH * MK_MAX_MAP_HEIGHT];
    u8 floorGrid[MK_MAX_MAP_WIDTH * MK_MAX_MAP_HEIGHT];
    u8 ceilGrid[MK_MAX_MAP_WIDTH * MK_MAX_MAP_HEIGHT];
    u8 lightGrid[MK_MAX_MAP_WIDTH * MK_MAX_MAP_HEIGHT];
    s32 width;
    s32 height;
    char name[64];
} Map;

bool Map_Init(void);
void Map_Shutdown(void);
Map* Map_Get(void);

/* Loading */
bool Map_Load(const char* filename);
bool Map_LoadFromData(const u8* wallData, const u8* floorData, const u8* ceilData,
                       s32 width, s32 height);

/* Queries */
u8   Map_GetCell(const Map* map, s32 x, s32 y);
void Map_SetCell(Map* map, s32 x, s32 y, u8 value);
bool Map_IsSolid(const Map* map, s32 x, s32 y);
bool Map_IsDoor(const Map* map, s32 x, s32 y);
void Map_OpenDoor(Map* map, s32 x, s32 y);

/* Update */
void Map_Update(void);

#endif
