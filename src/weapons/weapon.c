#include "weapons/weapon.h"
#include "weapons/weapon_data.h"
#include "player/player.h"
#include "player/aiming.h"
#include "camera/camera.h"
#include "renderer/renderer.h"
#include "audio/audio_manager.h"
#include "enemy/enemy.h"
#include "particles/particles.h"
#include "particles/blood.h"
#include "physics/collision.h"
#include "map/map.h"
#include "utilities/math_utils.h"
#include "engine/engine.h"
#include <math.h>
#include <string.h>

static Weapon g_weapons[WEAPON_COUNT];
static WeaponType g_currentWeapon = WEAPON_PISTOL;

/* Hitscan: cast ray and hit first enemy, wall, floor, or ceiling in path */
static void Weapon_Hitscan(const Vec2* origin, const Vec2* dir, f32 pitch, s32 damage, f32 spread, f32 maxDist) {
    const Map* map = Map_Get();
    EnemyManager* em = Enemy_GetManager();

    /* Apply horizontal spread */
    f32 angle = atan2f(dir->y, dir->x);
    angle += (Math_RandomFloat() * 2.0f - 1.0f) * spread;
    Vec2 rayDir = { cosf(angle), sinf(angle) };

    /* Eye height in world units (0 = floor, 1 = ceiling) */
    const f32 eyeHeight = 0.5f;

    f32 closestDist = maxDist;
    s32 hitEnemy = -1;
    bool headshot = false;

    /* Check each active enemy for intersection with ray */
    for (u32 i = 0; i < em->count; i++) {
        Enemy* e = &em->enemies[i];
        if (!e->active || !e->alive) continue;

        f32 ex = e->pos.x - origin->x;
        f32 ey = e->pos.y - origin->y;

        f32 a = rayDir.x * rayDir.x + rayDir.y * rayDir.y;
        f32 b = 2.0f * (ex * rayDir.x + ey * rayDir.y);
        f32 c = ex * ex + ey * ey - e->radius * e->radius;

        f32 disc = b * b - 4.0f * a * c;
        if (disc < 0.0f) continue;

        f32 sqrtDisc = sqrtf(disc);
        f32 t1 = (-b - sqrtDisc) / (2.0f * a);
        f32 t2 = (-b + sqrtDisc) / (2.0f * a);
        f32 t = (t1 > 0.0f) ? t1 : t2;
        if (t <= 0.0f) continue;

        /* Check wall occlusion */
        f32 hitX = origin->x + rayDir.x * t;
        f32 hitY = origin->y + rayDir.y * t;
        if (!Collision_LineOfSight(map, origin->x, origin->y, hitX, hitY)) continue;

        if (t < closestDist) {
            closestDist = t;
            hitEnemy = (s32)i;
            headshot = (Math_RandomFloat() < 0.15f);
        }
    }

    /* Check wall hit distance */
    f32 wallDist = maxDist;
    f32 wallX = 0.0f, wallY = 0.0f;
    if (Collision_RayWall(map, origin->x, origin->y,
                          origin->x + rayDir.x * maxDist,
                          origin->y + rayDir.y * maxDist,
                          &wallX, &wallY)) {
        f32 dx = wallX - origin->x;
        f32 dy = wallY - origin->y;
        wallDist = sqrtf(dx*dx + dy*dy);
    }

    /* Check floor/ceiling hit based on vertical pitch */
    f32 floorDist = maxDist;
    f32 ceilDist = maxDist;
    if (fabsf(pitch) > 0.001f) {
        f32 tanPitch = tanf(pitch);
        if (pitch < 0.0f) {
            /* Looking down — check floor */
            floorDist = eyeHeight / (-tanPitch);
            if (floorDist < 0.0f) floorDist = maxDist;
        } else {
            /* Looking up — check ceiling */
            ceilDist = (1.0f - eyeHeight) / tanPitch;
            if (ceilDist < 0.0f) ceilDist = maxDist;
        }
    }

    /* Determine what was hit first */
    if (floorDist < closestDist && floorDist < wallDist && floorDist < ceilDist) {
        /* Hit floor */
        Vec2 hitPos = { origin->x + rayDir.x * floorDist, origin->y + rayDir.y * floorDist };
        Vec2 crosshair = Aiming_GetCrosshairPos();
        Particles_SpawnSpark(&hitPos, 0.0f, 4);
        Renderer_AddTracer(&crosshair, &hitPos, 0.0f);
        Renderer_AddMark(&hitPos, 0.0f);
        return;
    }

    if (ceilDist < closestDist && ceilDist < wallDist) {
        /* Hit ceiling */
        Vec2 hitPos = { origin->x + rayDir.x * ceilDist, origin->y + rayDir.y * ceilDist };
        Vec2 crosshair = Aiming_GetCrosshairPos();
        Particles_SpawnSpark(&hitPos, 1.0f, 4);
        Renderer_AddTracer(&crosshair, &hitPos, 1.0f);
        Renderer_AddMark(&hitPos, 1.0f);
        return;
    }

    if (wallDist < closestDist) {
        /* Hit wall — compute actual vertical hit position from pitch */
        f32 tanPitch = tanf(pitch);
        f32 wallZ = eyeHeight + wallDist * tanPitch;
        if (wallZ < 0.0f) wallZ = 0.0f;
        if (wallZ > 1.0f) wallZ = 1.0f;

        Vec2 wallPos = { wallX, wallY };
        Vec2 crosshair = Aiming_GetCrosshairPos();
        Particles_SpawnSpark(&wallPos, wallZ, 4);
        Renderer_AddTracer(&crosshair, &wallPos, wallZ);
        Renderer_AddMark(&wallPos, wallZ);
        return;
    }

    if (hitEnemy >= 0) {
        /* Hit enemy */
        Enemy_TakeDamage(hitEnemy, damage, headshot);
        Player* p = Player_Get();
        if (p) {
            p->accuracyHits++;
            s32 points = em->enemies[hitEnemy].scoreValue;
            if (headshot) points = (s32)(points * 1.5f);
            p->score += points;
            p->combo++;
        }

        Vec2 hitPos = {
            origin->x + rayDir.x * closestDist,
            origin->y + rayDir.y * closestDist
        };
        Vec2 crosshair = Aiming_GetCrosshairPos();
        Blood_Spawn(&hitPos, 8);
        AudioManager_PlaySound("hit");
        Renderer_AddTracer(&crosshair, &hitPos, 0.5f);
        Renderer_AddMark(&hitPos, 0.5f);

        if (!Enemy_IsAlive(hitEnemy)) {
            AudioManager_PlaySound("enemy_death");
            Particles_SpawnExplosion(&hitPos, 0.5f, 10, 2.0f);
        }
    }
}

bool Weapon_Init(void) {
    memset(g_weapons, 0, sizeof(g_weapons));
    WeaponData_LoadAll(g_weapons);
    g_currentWeapon = WEAPON_PISTOL;
    return true;
}

void Weapon_Shutdown(void) {
    memset(g_weapons, 0, sizeof(g_weapons));
}

void Weapon_Update(f32 dt) {
    Weapon* w = &g_weapons[g_currentWeapon];
    if (w->reloading) {
        w->reloadTimer -= dt;
        if (w->reloadTimer <= 0.0f) {
            w->reloading = false;
            s32 need = w->magazine - w->currentMag;
            s32 take = need < w->ammo ? need : w->ammo;
            w->currentMag += take;
            w->ammo -= take;
        }
    }
}

void Weapon_Fire(void) {
    Weapon* w = &g_weapons[g_currentWeapon];
    f32 now = Engine_GetTime();
    if (w->reloading) return;
    if (w->currentMag <= 0) {
        Weapon_Reload();
        return;
    }
    if (now - w->lastFireTime < w->fireRate) return;

    w->lastFireTime = now;
    w->currentMag--;

    Player* p = Player_Get();
    if (p) p->accuracyShots++;

    /* Effects */
    CameraSystem_ApplyShake(w->recoil * 0.5f, 0.1f);
    Renderer_ApplyFlash(0.4f);
    AudioManager_PlaySound("shoot");

    /* Hitscan from camera position in aim direction */
    CameraSystem* cam = CameraSystem_Get();
    Vec2 origin = cam->pos;
    Vec2 dir = Aiming_GetWorldDirection(&origin);
    f32 pitch = Aiming_GetPitch();

    if (w->type == WEAPON_SHOTGUN) {
        /* Shotgun: 8 pellets */
        for (s32 i = 0; i < 8; i++) {
            Weapon_Hitscan(&origin, &dir, pitch, w->damage, w->spread, 32.0f);
        }
    } else if (w->type == WEAPON_ROCKETLAUNCHER) {
        /* Rocket: area damage (simplified as wide hitscan for now) */
        Weapon_Hitscan(&origin, &dir, pitch, w->damage, w->spread, 24.0f);
        /* TODO: explosion radius damage */
    } else {
        /* Single projectile hitscan */
        Weapon_Hitscan(&origin, &dir, pitch, w->damage, w->spread, 32.0f);
    }
}

void Weapon_Reload(void) {
    Weapon* w = &g_weapons[g_currentWeapon];
    if (w->reloading || w->currentMag >= w->magazine || w->ammo <= 0) return;
    w->reloading = true;
    w->reloadTimer = w->reloadTime;
    AudioManager_PlaySound("reload");
}

void Weapon_Switch(WeaponType type) {
    if (type >= 0 && type < WEAPON_COUNT) {
        /* Cancel reload on switch */
        g_weapons[g_currentWeapon].reloading = false;
        g_currentWeapon = type;
    }
}

Weapon* Weapon_GetCurrent(void) {
    return &g_weapons[g_currentWeapon];
}

WeaponType Weapon_GetCurrentType(void) {
    return g_currentWeapon;
}
