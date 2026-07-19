#ifndef MK_RENDERER_H
#define MK_RENDERER_H

#include "types.h"

/* Frame renderer: orchestrates raycasting, floor/ceiling, sprites, effects */
bool Renderer_Init(void);
void Renderer_Shutdown(void);
void Renderer_DrawFrame(void);

/* Post-processing effects */
void Renderer_ApplyFlash(f32 intensity);
void Renderer_ApplyFade(f32 amount);
void Renderer_SetPaletteShift(u8 shift);

/* Bullet tracer */
void Renderer_AddTracer(const Vec2* startScreen, const Vec2* end, f32 endZ);

/* Bullet impact mark (decal) */
void Renderer_AddMark(const Vec2* pos, f32 z);

#endif
