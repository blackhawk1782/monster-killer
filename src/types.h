#ifndef MK_TYPES_H
#define MK_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Fixed-width scalar aliases for engine consistency */
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;
typedef float    f32;
typedef double   f64;

/* 2D vector */
typedef struct { f32 x, y; } Vec2;

/* 3D vector */
typedef struct { f32 x, y, z; } Vec3;

/* Integer 2D point */
typedef struct { s32 x, y; } Point2;

/* Rectangle */
typedef struct { s32 x, y, w, h; } Rect;

/* Color packed as RGBA */
typedef struct { u8 r, g, b, a; } Color32;

/* Engine constants */
#define MK_MAX_TEXTURES        256
#define MK_MAX_SPRITES         1024
#define MK_MAX_ENEMIES         64
#define MK_MAX_PARTICLES       2048
#define MK_MAX_SOUNDS          128
#define MK_MAX_MAP_WIDTH       64
#define MK_MAX_MAP_HEIGHT      64
#define MK_MAX_RAIL_NODES      256
#define MK_MAX_EVENTS          128
#define MK_MAX_WEAPONS         16
#define MK_MAX_BOSSES          4
#define MK_MAX_ANIMATIONS      64
#define MK_MAX_SPRITE_FRAMES   32
#define MK_MAX_ATLASES         32
#define MK_SCREEN_WIDTH        320
#define MK_SCREEN_HEIGHT       200
#define MK_FOV                 60.0f
#define MK_MAX_RAYCAST_DEPTH   32
#define MK_MAX_SPRITE_DRAWS    512
#define MK_PI                  3.14159265358979323846f
#define MK_TWO_PI              6.28318530717958647692f
#define MK_HALF_PI             1.57079632679489661923f
#define MK_DEG2RAD             0.01745329251994329577f
#define MK_RAD2DEG             57.2957795130823208768f
#define MK_EPSILON             0.0001f

#endif
