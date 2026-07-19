#include "map/map.h"
#include "map/map_loader.h"
#include "utilities/math_utils.h"
#include <string.h>
#include <stdio.h>

static Map g_map = {0};

bool Map_Init(void) {
    memset(&g_map, 0, sizeof(g_map));
    g_map.width = 16;
    g_map.height = 16;
    strncpy(g_map.name, "default", sizeof(g_map.name) - 1);

    /* Build a simple straight horizontal corridor */
    for (s32 y = 0; y < g_map.height; y++) {
        for (s32 x = 0; x < g_map.width; x++) {
            /* Outer walls */
            if (x == 0 || y == 0 || x == g_map.width - 1 || y == g_map.height - 1) {
                g_map.grid[y * g_map.width + x] = 1;
            }
            /* Top and bottom walls of corridor at y=4 */
            else if (y == 3 && x > 0 && x < 15) {
                g_map.grid[y * g_map.width + x] = 1;
            }
            else if (y == 5 && x > 0 && x < 15) {
                g_map.grid[y * g_map.width + x] = 1;
            }
            else {
                g_map.grid[y * g_map.width + x] = 0;
            }
            g_map.floorGrid[y * g_map.width + x] = 2;
            g_map.ceilGrid[y * g_map.width + x] = 3;
        }
    }
    return true;
}

void Map_Shutdown(void) {
    memset(&g_map, 0, sizeof(g_map));
}

Map* Map_Get(void) {
    return &g_map;
}

bool Map_Load(const char* filename) {
    return MapLoader_LoadFromFile(&g_map, filename);
}

bool Map_LoadFromData(const u8* wallData, const u8* floorData, const u8* ceilData,
                       s32 width, s32 height) {
    if (width <= 0 || height <= 0 || width > MK_MAX_MAP_WIDTH || height > MK_MAX_MAP_HEIGHT)
        return false;

    g_map.width = width;
    g_map.height = height;
    memcpy(g_map.grid, wallData, width * height);
    if (floorData) memcpy(g_map.floorGrid, floorData, width * height);
    if (ceilData)  memcpy(g_map.ceilGrid, ceilData, width * height);
    return true;
}

u8 Map_GetCell(const Map* map, s32 x, s32 y) {
    if (!map || x < 0 || x >= map->width || y < 0 || y >= map->height) return 1;
    return map->grid[y * map->width + x];
}

void Map_SetCell(Map* map, s32 x, s32 y, u8 value) {
    if (!map || x < 0 || x >= map->width || y < 0 || y >= map->height) return;
    map->grid[y * map->width + x] = value;
}

bool Map_IsSolid(const Map* map, s32 x, s32 y) {
    u8 cell = Map_GetCell(map, x, y);
    return cell == CELL_WALL || cell == CELL_DOOR || cell == CELL_SECRET || cell == CELL_WINDOW;
}

bool Map_IsDoor(const Map* map, s32 x, s32 y) {
    return Map_GetCell(map, x, y) == CELL_DOOR;
}

void Map_OpenDoor(Map* map, s32 x, s32 y) {
    if (Map_IsDoor(map, x, y)) {
        Map_SetCell(map, x, y, CELL_EMPTY);
    }
}

void Map_Update(void) {
    /* Animate doors, update lights, etc. */
}
