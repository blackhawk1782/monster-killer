#ifndef MK_MAP_LOADER_H
#define MK_MAP_LOADER_H

#include "map/map.h"

bool MapLoader_LoadFromFile(Map* map, const char* filename);
bool MapLoader_LoadFromString(Map* map, const char* text);

#endif
