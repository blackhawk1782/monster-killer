#include "enemy/enemy.h"
#include "enemy/enemy_ai.h"
#include "sprites/sprites.h"
#include "utilities/math_utils.h"
#include "engine/engine.h"
#include <string.h>

static EnemyManager g_enemyMgr = {0};

bool Enemy_Init(void) {
    memset(&g_enemyMgr, 0, sizeof(g_enemyMgr));
    return true;
}

void Enemy_Shutdown(void) {
    memset(&g_enemyMgr, 0, sizeof(g_enemyMgr));
}

EnemyManager* Enemy_GetManager(void) {
    return &g_enemyMgr;
}

s32 Enemy_Spawn(const Vec2* pos, s32 type) {
    if (g_enemyMgr.count >= MK_MAX_ENEMIES) return -1;
    s32 id = (s32)g_enemyMgr.count;
    Enemy* e = &g_enemyMgr.enemies[id];
    memset(e, 0, sizeof(Enemy));
    e->pos = *pos;
    e->z = 0.5f;  /* Center of body at eye level */
    e->radius = 0.4f;  /* Hitbox radius */
    e->health = 50;
    e->maxHealth = 50;
    e->armor = 0;
    e->state = ES_IDLE;
    e->alive = true;
    e->active = true;
    e->accuracy = 0.7f;
    e->speed = 1.0f;
    e->hearingRadius = 8.0f;
    e->sightRadius = 16.0f;
    e->scoreValue = 100;
    e->weakSpotMultiplier = 2.0f;
    (void)type;

    Animation_Init(&e->anim, 0, 3, 8.0f, true);
    Animation_Play(&e->anim);

    e->spriteId = Sprite_Spawn(pos, e->z, 0);
    if (e->spriteId >= 0) {
        /* Set enemy sprite to bright red for visibility */
        SpriteManager* sm = Sprite_GetManager();
        if (sm && (u32)e->spriteId < sm->count) {
            sm->sprites[e->spriteId].tint = (Color32){255, 50, 50, 255};
            sm->sprites[e->spriteId].scale = 0.8f;
        }
    }
    g_enemyMgr.count++;
    g_enemyMgr.activeCount++;
    return id;
}

void Enemy_Destroy(s32 id) {
    if (id < 0 || id >= (s32)g_enemyMgr.count) return;
    Enemy* e = &g_enemyMgr.enemies[id];
    e->active = false;
    e->alive = false;
    if (e->spriteId >= 0) Sprite_Destroy(e->spriteId);
    g_enemyMgr.activeCount--;
}

void Enemy_UpdateAll(f32 dt) {
    for (u32 i = 0; i < g_enemyMgr.count; i++) {
        Enemy* e = &g_enemyMgr.enemies[i];
        if (!e->active || !e->alive) continue;
        Animation_Update(&e->anim, dt);
        EnemyAI_Update(e, dt);
    }
}

void Enemy_DrawAll(void) {
    /* Sprites are drawn by Sprite_DrawAll */
}

void Enemy_TakeDamage(s32 id, s32 damage, bool headshot) {
    if (id < 0 || id >= (s32)g_enemyMgr.count) return;
    Enemy* e = &g_enemyMgr.enemies[id];
    if (!e->alive) return;

    s32 actual = damage;
    if (headshot && e->weakSpot) actual = (s32)(damage * e->weakSpotMultiplier);
    if (e->armor > 0) {
        s32 absorbed = actual / 2;
        if (absorbed > e->armor) absorbed = e->armor;
        e->armor -= absorbed;
        actual -= absorbed;
    }

    e->health -= actual;
    e->state = ES_PAIN;
    Animation_Init(&e->anim, 4, 5, 10.0f, false);
    Animation_Play(&e->anim);

    if (e->health <= 0) {
        e->health = 0;
        e->alive = false;
        e->state = ES_DEATH;
        Animation_Init(&e->anim, 6, 9, 8.0f, false);
        Animation_Play(&e->anim);
    }
}

bool Enemy_IsAlive(s32 id) {
    if (id < 0 || id >= (s32)g_enemyMgr.count) return false;
    return g_enemyMgr.enemies[id].alive;
}
