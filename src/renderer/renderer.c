#include "renderer/renderer.h"
#include "renderer/texture_manager.h"
#include "raycaster/raycaster.h"
#include "camera/camera.h"
#include "map/map.h"
#include "engine/engine.h"
#include "sprites/sprites.h"
#include "particles/particles.h"
#include "utilities/math_utils.h"
#include <raylib.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static f32 g_flashIntensity = 0.0f;
static f32 g_fadeAmount = 0.0f;
static u8  g_paletteShift = 0;

/* Bullet tracers */
#define MAX_TRACERS 16
typedef struct {
    Vec2 startScreen;   /* Crosshair position in screen space at fire time */
    Vec2 end;           /* World-space hit position */
    f32 endZ;           /* 0 = floor, 0.5 = wall, 1 = ceiling */
    f32 life;
    f32 maxLife;
    bool active;
} BulletTracer;
static BulletTracer g_tracers[MAX_TRACERS] = {0};

/* Bullet impact marks (decals) */
#define MAX_MARKS 64
typedef struct {
    Vec2 pos;           /* World-space hit position */
    f32 z;              /* 0 = floor, 0.5 = wall, 1 = ceiling */
    f32 life;
    f32 maxLife;
    bool active;
} BulletMark;
static BulletMark g_marks[MAX_MARKS] = {0};

void Renderer_AddMark(const Vec2* pos, f32 z) {
    for (s32 i = 0; i < MAX_MARKS; i++) {
        if (!g_marks[i].active) {
            g_marks[i].pos = *pos;
            g_marks[i].z = z;
            g_marks[i].life = 3.0f;      /* Visible for 3 seconds */
            g_marks[i].maxLife = 3.0f;
            g_marks[i].active = true;
            return;
        }
    }
}

static void Renderer_DrawMarks(const CameraSystem* cam) {
    f32 dt = Engine_GetDeltaTime();
    for (s32 i = 0; i < MAX_MARKS; i++) {
        BulletMark* m = &g_marks[i];
        if (!m->active) continue;

        m->life -= dt;
        if (m->life <= 0.0f) {
            m->active = false;
            continue;
        }

        /* Project mark to screen */
        f32 invDet = 1.0f / (cam->plane.x * cam->dir.y - cam->dir.x * cam->plane.y);

        f32 sx = m->pos.x - cam->renderPos.x;
        f32 sy = m->pos.y - cam->renderPos.y;
        f32 tx = invDet * (cam->dir.y * sx - cam->dir.x * sy);
        f32 tz = invDet * (-cam->plane.y * sx + cam->plane.x * sy);

        if (tz <= 0.01f) continue;

        s32 screenX = (s32)((MK_SCREEN_WIDTH / 2.0f) * (1.0f + tx / tz));
        s32 screenY = (s32)(MK_SCREEN_HEIGHT / 2.0f + (m->z - 0.5f) / tz * MK_SCREEN_HEIGHT);

        f32 alpha = m->life / m->maxLife;
        s32 size = (s32)(4.0f / tz);
        if (size < 2) size = 2;
        if (size > 12) size = 12;

        /* Dark bullet hole / scorch mark */
        u8 a = (u8)(180 * alpha);
        Color c = { 40, 30, 20, a };
        DrawRectangle(screenX - size/2, screenY - size/2, size, size, c);

        /* Inner darker core */
        s32 core = size / 2;
        if (core >= 1) {
            Color coreC = { 20, 15, 10, a };
            DrawRectangle(screenX - core/2, screenY - core/2, core, core, coreC);
        }
    }
}

void Renderer_AddTracer(const Vec2* startScreen, const Vec2* end, f32 endZ) {
    for (s32 i = 0; i < MAX_TRACERS; i++) {
        if (!g_tracers[i].active) {
            g_tracers[i].startScreen = *startScreen;
            g_tracers[i].end = *end;
            g_tracers[i].endZ = endZ;
            g_tracers[i].life = 0.08f;
            g_tracers[i].maxLife = 0.08f;
            g_tracers[i].active = true;
            return;
        }
    }
}

static void Renderer_DrawTracers(const CameraSystem* cam) {
    f32 dt = Engine_GetDeltaTime();
    for (s32 i = 0; i < MAX_TRACERS; i++) {
        BulletTracer* t = &g_tracers[i];
        if (!t->active) continue;

        t->life -= dt;
        if (t->life <= 0.0f) {
            t->active = false;
            continue;
        }

        /* Project end point to screen */
        f32 invDet = 1.0f / (cam->plane.x * cam->dir.y - cam->dir.x * cam->plane.y);

        f32 sx = t->end.x - cam->renderPos.x;
        f32 sy = t->end.y - cam->renderPos.y;
        f32 tx = invDet * (cam->dir.y * sx - cam->dir.x * sy);
        f32 tz = invDet * (-cam->plane.y * sx + cam->plane.x * sy);

        if (tz <= 0.01f) continue;

        s32 x1 = (s32)((MK_SCREEN_WIDTH / 2.0f) * (1.0f + tx / tz));
        s32 y1 = (s32)(MK_SCREEN_HEIGHT / 2.0f + (t->endZ - 0.5f) / tz * MK_SCREEN_HEIGHT);

        s32 x0 = (s32)t->startScreen.x;
        s32 y0 = (s32)t->startScreen.y;

        f32 alpha = t->life / t->maxLife;
        Color c = { 255, 255, 0, (u8)(200 * alpha) };
        DrawLine(x0, y0, x1, y1, c);
    }
}

bool Renderer_Init(void) {
    return true;
}

void Renderer_Shutdown(void) {
}

void Renderer_DrawFrame(void) {
    const CameraSystem* cam = CameraSystem_Get();
    const Map* map = Map_Get();

    /* 1. Raycast walls, floor, ceiling into z-buffer */
    Raycaster_Render(cam, map);

    /* 2. Collect and depth-sort sprites */
    Sprite_DrawAll(cam, map);

    /* 3. Particles */
    Particles_Draw(cam);

    /* 3.5 Bullet tracers */
    Renderer_DrawTracers(cam);

    /* 3.6 Bullet impact marks */
    Renderer_DrawMarks(cam);

    /* 4. Post-processing overlays */
    if (g_flashIntensity > 0.01f) {
        u8 alpha = (u8)(g_flashIntensity * 255.0f);
        DrawRectangle(0, 0, MK_SCREEN_WIDTH, MK_SCREEN_HEIGHT,
                      (Color){255, 255, 200, alpha});
        g_flashIntensity *= 0.85f;
        if (g_flashIntensity < 0.01f) g_flashIntensity = 0.0f;
    }

    if (g_fadeAmount > 0.01f) {
        u8 alpha = (u8)(g_fadeAmount * 255.0f);
        DrawRectangle(0, 0, MK_SCREEN_WIDTH, MK_SCREEN_HEIGHT,
                      (Color){0, 0, 0, alpha});
    }

    if (g_paletteShift > 0) {
        /* Simple palette shift via overlay tint */
        DrawRectangle(0, 0, MK_SCREEN_WIDTH, MK_SCREEN_HEIGHT,
                      (Color){g_paletteShift, 0, 255 - g_paletteShift, 30});
    }
}

void Renderer_ApplyFlash(f32 intensity) {
    g_flashIntensity = intensity;
}

void Renderer_ApplyFade(f32 amount) {
    g_fadeAmount = amount;
}

void Renderer_SetPaletteShift(u8 shift) {
    g_paletteShift = shift;
}
