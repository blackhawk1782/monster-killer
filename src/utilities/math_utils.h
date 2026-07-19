#ifndef MK_MATH_UTILS_H
#define MK_MATH_UTILS_H

#include "types.h"

/* Angle utilities */
f32 Math_WrapAngle(f32 angle);
f32 Math_AngleDifference(f32 a, f32 b);
f32 Math_Lerp(f32 a, f32 b, f32 t);
f32 Math_Clamp(f32 v, f32 min, f32 max);
s32 Math_ClampInt(s32 v, s32 min, s32 max);
f32 Math_Distance(f32 x1, f32 y1, f32 x2, f32 y2);
f32 Math_DistanceSq(f32 x1, f32 y1, f32 x2, f32 y2);

/* Random */
void Math_SeedRandom(u32 seed);
f32  Math_RandomFloat(void);
s32  Math_RandomRange(s32 min, s32 max);

/* Vector2 ops */
Vec2 Math_Vec2Add(const Vec2* a, const Vec2* b);
Vec2 Math_Vec2Sub(const Vec2* a, const Vec2* b);
Vec2 Math_Vec2Scale(const Vec2* v, f32 s);
f32  Math_Vec2Len(const Vec2* v);
Vec2 Math_Vec2Normalize(const Vec2* v);

#endif
