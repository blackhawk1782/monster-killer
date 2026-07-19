#ifndef MK_ENEMY_H
#define MK_ENEMY_H

#include "types.h"
#include "animation/animation.h"

typedef enum {
    ES_IDLE,
    ES_WALK,
    ES_RUN,
    ES_ATTACK,
    ES_SHOOT,
    ES_PAIN,
    ES_DEATH,
    ES_SPECIAL
} EnemyState;

typedef struct {
    Vec2 pos;
    f32 z;
    f32 angle;
    f32 radius;       /* Collision/hit radius */
    s32 health;
    s32 maxHealth;
    s32 armor;
    EnemyState state;
    Animation anim;
    f32 reactionTimer;
    f32 cooldownTimer;
    f32 accuracy;
    bool alive;
    bool active;
    s32 spriteId;
    s32 weaponType;
    f32 speed;
    f32 hearingRadius;
    f32 sightRadius;
    s32 scoreValue;
    bool weakSpot;
    f32 weakSpotMultiplier;
} Enemy;

typedef struct {
    Enemy enemies[MK_MAX_ENEMIES];
    u32 count;
    u32 activeCount;
} EnemyManager;

bool Enemy_Init(void);
void Enemy_Shutdown(void);
EnemyManager* Enemy_GetManager(void);

s32  Enemy_Spawn(const Vec2* pos, s32 type);
void Enemy_Destroy(s32 id);
void Enemy_UpdateAll(f32 dt);
void Enemy_DrawAll(void);

void Enemy_TakeDamage(s32 id, s32 damage, bool headshot);
bool Enemy_IsAlive(s32 id);

#endif
