#include "physics/collision.h"
#include <math.h>

bool Collision_PointInWall(const Map* map, f32 x, f32 y) {
    if (!map) return true;
    s32 mx = (s32)x;
    s32 my = (s32)y;
    return Map_IsSolid(map, mx, my);
}

bool Collision_CircleWall(const Map* map, f32 x, f32 y, f32 radius) {
    if (!map) return true;
    s32 minX = (s32)(x - radius);
    s32 maxX = (s32)(x + radius);
    s32 minY = (s32)(y - radius);
    s32 maxY = (s32)(y + radius);

    for (s32 iy = minY; iy <= maxY; iy++) {
        for (s32 ix = minX; ix <= maxX; ix++) {
            if (Map_IsSolid(map, ix, iy)) {
                f32 dx = x - (ix + 0.5f);
                f32 dy = y - (iy + 0.5f);
                if (dx*dx + dy*dy < radius*radius) return true;
            }
        }
    }
    return false;
}

bool Collision_RayWall(const Map* map, f32 x0, f32 y0, f32 x1, f32 y1, f32* outX, f32* outY) {
    if (!map) return false;

    f32 dx = x1 - x0;
    f32 dy = y1 - y0;
    f32 dist = sqrtf(dx*dx + dy*dy);
    if (dist < 0.001f) return false;

    f32 stepX = dx / dist * 0.1f;
    f32 stepY = dy / dist * 0.1f;
    f32 steps = dist / 0.1f;

    for (f32 i = 0; i < steps; i++) {
        f32 cx = x0 + stepX * i;
        f32 cy = y0 + stepY * i;
        if (Map_IsSolid(map, (s32)cx, (s32)cy)) {
            if (outX) *outX = cx;
            if (outY) *outY = cy;
            return true;
        }
    }
    return false;
}

bool Collision_LineOfSight(const Map* map, f32 x0, f32 y0, f32 x1, f32 y1) {
    f32 dummyX, dummyY;
    return !Collision_RayWall(map, x0, y0, x1, y1, &dummyX, &dummyY);
}
