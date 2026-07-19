#ifndef MK_BOSS_H
#define MK_BOSS_H

#include "types.h"
#include "animation/animation.h"

typedef enum {
    BS_IDLE,
    BS_PHASE1,
    BS_PHASE2,
    BS_PHASE3,
    BS_SPECIAL,
    BS_VULNERABLE,
    BS_DEATH
} BossState;

typedef struct {
    Vec2 pos;
    f32 z;
    s32 health;
    s32 maxHealth;
    s32 phase;
    s32 maxPhases;
    BossState state;
    Animation anim;
    f32 timer;
    f32 attackCooldown;
    bool alive;
    bool active;
    s32 spriteId;
    s32 scoreValue;
    char name[32];
} Boss;

typedef struct {
    Boss bosses[MK_MAX_BOSSES];
    u32 count;
} BossManager;

bool Boss_Init(void);
void Boss_Shutdown(void);
BossManager* Boss_GetManager(void);

s32  Boss_Spawn(const Vec2* pos, s32 type);
void Boss_Destroy(s32 id);
void Boss_UpdateAll(f32 dt);
void Boss_TakeDamage(s32 id, s32 damage);
bool Boss_IsAlive(s32 id);

#endif
