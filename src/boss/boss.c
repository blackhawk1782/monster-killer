#include "boss/boss.h"
#include "boss/boss_ai.h"
#include "sprites/sprites.h"
#include "utilities/math_utils.h"
#include <string.h>

static BossManager g_bossMgr = {0};

bool Boss_Init(void) {
    memset(&g_bossMgr, 0, sizeof(g_bossMgr));
    return true;
}

void Boss_Shutdown(void) {
    memset(&g_bossMgr, 0, sizeof(g_bossMgr));
}

BossManager* Boss_GetManager(void) {
    return &g_bossMgr;
}

s32 Boss_Spawn(const Vec2* pos, s32 type) {
    if (g_bossMgr.count >= MK_MAX_BOSSES) return -1;
    s32 id = (s32)g_bossMgr.count;
    Boss* b = &g_bossMgr.bosses[id];
    memset(b, 0, sizeof(Boss));
    b->pos = *pos;
    b->z = 0.0f;
    b->health = 500;
    b->maxHealth = 500;
    b->phase = 1;
    b->maxPhases = 3;
    b->state = BS_IDLE;
    b->alive = true;
    b->active = true;
    b->scoreValue = 5000;
    b->attackCooldown = 2.0f;
    (void)type;

    Animation_Init(&b->anim, 0, 3, 6.0f, true);
    Animation_Play(&b->anim);

    b->spriteId = Sprite_Spawn(pos, b->z, 0);
    g_bossMgr.count++;
    return id;
}

void Boss_Destroy(s32 id) {
    if (id < 0 || id >= (s32)g_bossMgr.count) return;
    Boss* b = &g_bossMgr.bosses[id];
    b->active = false;
    b->alive = false;
    if (b->spriteId >= 0) Sprite_Destroy(b->spriteId);
}

void Boss_UpdateAll(f32 dt) {
    for (u32 i = 0; i < g_bossMgr.count; i++) {
        Boss* b = &g_bossMgr.bosses[i];
        if (!b->active || !b->alive) continue;
        Animation_Update(&b->anim, dt);
        BossAI_Update(b, dt);
    }
}

void Boss_TakeDamage(s32 id, s32 damage) {
    if (id < 0 || id >= (s32)g_bossMgr.count) return;
    Boss* b = &g_bossMgr.bosses[id];
    if (!b->alive) return;

    b->health -= damage;
    if (b->health <= 0) {
        b->phase++;
        if (b->phase > b->maxPhases) {
            b->health = 0;
            b->alive = false;
            b->state = BS_DEATH;
            Animation_Init(&b->anim, 8, 15, 5.0f, false);
            Animation_Play(&b->anim);
        } else {
            b->health = b->maxHealth / 2;
            b->state = BS_VULNERABLE;
            b->timer = 3.0f;
        }
    }
}

bool Boss_IsAlive(s32 id) {
    if (id < 0 || id >= (s32)g_bossMgr.count) return false;
    return g_bossMgr.bosses[id].alive;
}
