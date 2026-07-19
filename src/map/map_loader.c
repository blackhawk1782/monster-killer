#include "map/map_loader.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bool MapLoader_LoadFromFile(Map* map, const char* filename) {
    if (!map || !filename) return false;

    char path[256];
    snprintf(path, sizeof(path), "assets/maps/%s", filename);

    FILE* f = fopen(path, "rb");
    if (!f) {
        fprintf(stderr, "Map file not found: %s\n", path);
        return false;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* text = (char*)malloc(size + 1);
    if (!text) {
        fclose(f);
        return false;
    }

    fread(text, 1, size, f);
    text[size] = '\0';
    fclose(f);

    bool result = MapLoader_LoadFromString(map, text);
    free(text);
    return result;
}

bool MapLoader_LoadFromString(Map* map, const char* text) {
    if (!map || !text) return false;

    memset(map, 0, sizeof(Map));

    s32 width = 0, height = 0;
    const char* p = text;

    /* First pass: determine dimensions */
    s32 lineLen = 0;
    while (*p) {
        if (*p == '\n') {
            if (lineLen > width) width = lineLen;
            lineLen = 0;
            height++;
        } else if (*p != '\r') {
            lineLen++;
        }
        p++;
    }
    if (lineLen > 0) height++;

    if (width <= 0 || height <= 0 || width > MK_MAX_MAP_WIDTH || height > MK_MAX_MAP_HEIGHT) {
        fprintf(stderr, "Invalid map dimensions: %dx%d\n", width, height);
        return false;
    }

    map->width = width;
    map->height = height;

    /* Second pass: fill grid */
    p = text;
    s32 y = 0, x = 0;
    while (*p && y < height) {
        if (*p == '\n') {
            y++;
            x = 0;
        } else if (*p == '\r') {
            /* skip */
        } else {
            if (x < width) {
                char c = *p;
                u8 cell = 0;
                switch (c) {
                    case '#': cell = CELL_WALL; break;
                    case 'D': cell = CELL_DOOR; break;
                    case 'S': cell = CELL_SECRET; break;
                    case 'W': cell = CELL_WINDOW; break;
                    case 'P': cell = CELL_SPAWN; break;
                    case 'E': cell = CELL_ENEMY; break;
                    case 'B': cell = CELL_BOSS; break;
                    case 'I': cell = CELL_PICKUP; break;
                    case '^': cell = CELL_DECORATION; break;
                    case '*': cell = CELL_LIGHT; break;
                    default:  cell = CELL_EMPTY; break;
                }
                map->grid[y * width + x] = cell;
                map->floorGrid[y * width + x] = 2; /* default floor */
                map->ceilGrid[y * width + x] = 3;  /* default ceiling */
            }
            x++;
        }
        p++;
    }

    return true;
}
