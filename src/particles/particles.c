#include "particles/particles.h"
#include "utilities/math_utils.h"
#include "engine/engine.h"
#include <raylib.h>
#include <math.h>
#include <string.h>

static ParticleSystem g_particles = {0};

bool Particles_Init(void) {
    memset(&g_particles, 0, sizeof(g_particles));
    return true;
}

void Particles_Shutdown(void) {
    memset(&g_particles, 0, sizeof(g_particles));
}

ParticleSystem* Particles_GetSystem(void) {
    return &g_particles;
}

void Particles_Spawn(const Vec2* pos, f32 z, const Vec2* vel, f32 life, f32 size, const Color32* color) {
    if (!pos || !vel || !color) return;

    for (u32 i = 0; i < MK_MAX_PARTICLES; i++) {
        Particle* p = &g_particles.particles[i];
        if (!p->active) {
            p->pos = *pos;
            p->z = z;
            p->vel = *vel;
            p->life = life;
            p->maxLife = life;
            p->size = size;
            p->color = *color;
            p->active = true;
            g_particles.activeCount++;
            return;
        }
    }
}

void Particles_SpawnExplosion(const Vec2* pos, f32 z, s32 count, f32 radius) {
    for (s32 i = 0; i < count; i++) {
        f32 angle = Math_RandomFloat() * MK_TWO_PI;
        f32 speed = Math_RandomFloat() * radius;
        Vec2 vel = { cosf(angle) * speed, sinf(angle) * speed };
        Color32 c = { 255, (u8)(Math_RandomFloat() * 128), 0, 255 };
        Particles_Spawn(pos, z, &vel, 0.5f + Math_RandomFloat() * 0.5f, 2.0f, &c);
    }
}

void Particles_SpawnSpark(const Vec2* pos, f32 z, s32 count) {
    for (s32 i = 0; i < count; i++) {
        f32 angle = Math_RandomFloat() * MK_TWO_PI;
        f32 speed = 2.0f + Math_RandomFloat() * 4.0f;
        Vec2 vel = { cosf(angle) * speed, sinf(angle) * speed };
        Color32 c = { 255, 255, 200, 255 };
        Particles_Spawn(pos, z, &vel, 0.2f + Math_RandomFloat() * 0.3f, 1.0f, &c);
    }
}

void Particles_Update(f32 dt) {
    for (u32 i = 0; i < MK_MAX_PARTICLES; i++) {
        Particle* p = &g_particles.particles[i];
        if (!p->active) continue;

        p->pos.x += p->vel.x * dt;
        p->pos.y += p->vel.y * dt;
        p->life -= dt;

        if (p->life <= 0.0f) {
            p->active = false;
            g_particles.activeCount--;
        }
    }
}

void Particles_Draw(const CameraSystem* cam) {
    if (!cam) return;

    for (u32 i = 0; i < MK_MAX_PARTICLES; i++) {
        Particle* p = &g_particles.particles[i];
        if (!p->active) continue;

        f32 alpha = p->life / p->maxLife;
        if (alpha < 0.0f) alpha = 0.0f;
        if (alpha > 1.0f) alpha = 1.0f;

        /* 3D-to-2D projection with vertical height */
        f32 dx = p->pos.x - cam->renderPos.x;
        f32 dy = p->pos.y - cam->renderPos.y;

        f32 invDet = 1.0f / (cam->plane.x * cam->dir.y - cam->dir.x * cam->plane.y);
        f32 transformX = invDet * (cam->dir.y * dx - cam->dir.x * dy);
        f32 transformY = invDet * (-cam->plane.y * dx + cam->plane.x * dy);

        if (transformY <= 0.01f) continue;

        s32 screenX = (s32)((MK_SCREEN_WIDTH / 2.0f) * (1.0f + transformX / transformY));
        /* Vertical projection: z=0 floor, z=0.5 eye, z=1 ceiling */
        s32 screenY = (s32)(MK_SCREEN_HEIGHT / 2.0f + (p->z - 0.5f) / transformY * MK_SCREEN_HEIGHT);
        s32 size = (s32)(p->size / transformY);
        if (size < 1) size = 1;

        Color c = {
            p->color.r,
            p->color.g,
            p->color.b,
            (u8)(p->color.a * alpha)
        };
        DrawRectangle(screenX - size/2, screenY - size/2, size, size, c);
    }
}
