#ifndef MK_PARTICLES_H
#define MK_PARTICLES_H

#include "types.h"
#include "camera/camera.h"

typedef struct {
    Vec2 pos;
    f32 z;              /* Vertical position: 0 = floor, 1 = ceiling */
    Vec2 vel;
    f32 life;
    f32 maxLife;
    f32 size;
    Color32 color;
    bool active;
} Particle;

typedef struct {
    Particle particles[MK_MAX_PARTICLES];
    u32 activeCount;
} ParticleSystem;

bool Particles_Init(void);
void Particles_Shutdown(void);
ParticleSystem* Particles_GetSystem(void);

void Particles_Spawn(const Vec2* pos, f32 z, const Vec2* vel, f32 life, f32 size, const Color32* color);
void Particles_SpawnExplosion(const Vec2* pos, f32 z, s32 count, f32 radius);
void Particles_SpawnSpark(const Vec2* pos, f32 z, s32 count);
void Particles_Update(f32 dt);
void Particles_Draw(const CameraSystem* cam);

#endif
