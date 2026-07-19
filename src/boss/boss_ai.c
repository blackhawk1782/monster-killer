#include "boss/boss_ai.h"
#include "camera/camera.h"
#include "player/player.h"
#include "utilities/math_utils.h"
#include "engine/engine.h"
#include <math.h>

void BossAI_Update(Boss* boss, f32 dt) {
    if (!boss || !boss->active || !boss->alive) return;

    CameraSystem* cam = CameraSystem_Get();
    f32 dx = cam->pos.x - boss->pos.x;
    f32 dy = cam->pos.y - boss->pos.y;
    f32 dist = sqrtf(dx*dx + dy*dy);

    boss->attackCooldown -= dt;
    boss->timer -= dt;

    switch (boss->state) {
        case BS_IDLE:
            if (dist < 20.0f) {
                boss->state = BS_PHASE1;
                boss->timer = 5.0f;
            }
            break;

        case BS_PHASE1:
            if (boss->attackCooldown <= 0.0f) {
                /* Attack pattern: shoot at player */
                Player* p = Player_Get();
                if (p && p->alive && dist < 15.0f) {
                    Player_TakeDamage(15);
                }
                boss->attackCooldown = 1.5f;
            }
            if (boss->health < boss->maxHealth / 2) {
                boss->state = BS_PHASE2;
                boss->timer = 5.0f;
            }
            break;

        case BS_PHASE2:
            if (boss->attackCooldown <= 0.0f) {
                /* Faster attacks */
                Player* p = Player_Get();
                if (p && p->alive && dist < 15.0f) {
                    Player_TakeDamage(20);
                }
                boss->attackCooldown = 1.0f;
            }
            if (boss->health < boss->maxHealth / 4) {
                boss->state = BS_PHASE3;
                boss->timer = 5.0f;
            }
            break;

        case BS_PHASE3:
            if (boss->attackCooldown <= 0.0f) {
                /* Rapid fire */
                Player* p = Player_Get();
                if (p && p->alive && dist < 15.0f) {
                    Player_TakeDamage(25);
                }
                boss->attackCooldown = 0.5f;
            }
            break;

        case BS_VULNERABLE:
            if (boss->timer <= 0.0f) {
                boss->state = BS_PHASE1;
            }
            break;

        case BS_DEATH:
            if (Animation_IsFinished(&boss->anim)) {
                boss->active = false;
            }
            break;

        default:
            break;
    }
}
