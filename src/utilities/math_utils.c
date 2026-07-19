#include "utilities/math_utils.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

static u32 g_randState = 12345;

f32 Math_WrapAngle(f32 angle) {
    while (angle < -MK_PI) angle += MK_TWO_PI;
    while (angle >= MK_PI) angle -= MK_TWO_PI;
    return angle;
}

f32 Math_AngleDifference(f32 a, f32 b) {
    f32 diff = b - a;
    while (diff < -MK_PI) diff += MK_TWO_PI;
    while (diff > MK_PI) diff -= MK_TWO_PI;
    return diff;
}

f32 Math_Lerp(f32 a, f32 b, f32 t) {
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    return a + (b - a) * t;
}

f32 Math_Clamp(f32 v, f32 min, f32 max) {
    if (v < min) return min;
    if (v > max) return max;
    return v;
}

s32 Math_ClampInt(s32 v, s32 min, s32 max) {
    if (v < min) return min;
    if (v > max) return max;
    return v;
}

f32 Math_Distance(f32 x1, f32 y1, f32 x2, f32 y2) {
    f32 dx = x2 - x1;
    f32 dy = y2 - y1;
    return sqrtf(dx*dx + dy*dy);
}

f32 Math_DistanceSq(f32 x1, f32 y1, f32 x2, f32 y2) {
    f32 dx = x2 - x1;
    f32 dy = y2 - y1;
    return dx*dx + dy*dy;
}

void Math_SeedRandom(u32 seed) {
    g_randState = seed ? seed : (u32)time(NULL);
    srand(g_randState);
}

f32 Math_RandomFloat(void) {
    return (f32)rand() / (f32)RAND_MAX;
}

s32 Math_RandomRange(s32 min, s32 max) {
    if (min >= max) return min;
    return min + rand() % (max - min + 1);
}

Vec2 Math_Vec2Add(const Vec2* a, const Vec2* b) {
    return (Vec2){ a->x + b->x, a->y + b->y };
}

Vec2 Math_Vec2Sub(const Vec2* a, const Vec2* b) {
    return (Vec2){ a->x - b->x, a->y - b->y };
}

Vec2 Math_Vec2Scale(const Vec2* v, f32 s) {
    return (Vec2){ v->x * s, v->y * s };
}

f32 Math_Vec2Len(const Vec2* v) {
    return sqrtf(v->x * v->x + v->y * v->y);
}

Vec2 Math_Vec2Normalize(const Vec2* v) {
    f32 len = Math_Vec2Len(v);
    if (len < MK_EPSILON) return (Vec2){0,0};
    return (Vec2){ v->x / len, v->y / len };
}
