#ifndef MK_COLLISION_H
#define MK_COLLISION_H

#include "types.h"
#include "map/map.h"

bool Collision_PointInWall(const Map* map, f32 x, f32 y);
bool Collision_CircleWall(const Map* map, f32 x, f32 y, f32 radius);
bool Collision_RayWall(const Map* map, f32 x0, f32 y0, f32 x1, f32 y1, f32* outX, f32* outY);
bool Collision_LineOfSight(const Map* map, f32 x0, f32 y0, f32 x1, f32 y1);

#endif
