#include "enemy/enemy_ai.h"
#include "enemy/enemy_states.h"
#include "camera/camera.h"
#include "player/player.h"
#include "utilities/math_utils.h"
#include "engine/engine.h"
#include <math.h>

void EnemyAI_Update(Enemy* enemy, f32 dt) {
    if (!enemy || !enemy->active || !enemy->alive) return;

    CameraSystem* cam = CameraSystem_Get();
    f32 dx = cam->pos.x - enemy->pos.x;
    f32 dy = cam->pos.y - enemy->pos.y;
    f32 distSq = dx*dx + dy*dy;
    f32 dist = sqrtf(distSq);

    /* State machine transitions */
    switch (enemy->state) {
        case ES_IDLE:
            if (dist < enemy->hearingRadius * enemy->hearingRadius) {
                enemy->state = ES_WALK;
            }
            break;

        case ES_WALK:
            if (dist < enemy->sightRadius * enemy->sightRadius) {
                enemy->state = ES_ATTACK;
                enemy->reactionTimer = 0.5f;
            }
            /* Move toward player */
            if (dist > 1.5f) {
                f32 move = enemy->speed * dt;
                enemy->pos.x += (dx / dist) * move;
                enemy->pos.y += (dy / dist) * move;
                enemy->angle = atan2f(dy, dx);
            }
            break;

        case ES_ATTACK:
            enemy->reactionTimer -= dt;
            if (enemy->reactionTimer <= 0.0f) {
                enemy->state = ES_SHOOT;
                enemy->cooldownTimer = 1.0f;
            }
            break;

        case ES_SHOOT:
            enemy->cooldownTimer -= dt;
            if (enemy->cooldownTimer <= 0.0f) {
                /* Fire at player */
                Player* p = Player_Get();
                if (p && p->alive) {
                    f32 hitChance = enemy->accuracy / (1.0f + dist * 0.1f);
                    if (Math_RandomFloat() < hitChance) {
                        Player_TakeDamage(10);
                    }
                }
                enemy->state = ES_WALK;
            }
            break;

        case ES_PAIN:
            if (Animation_IsFinished(&enemy->anim)) {
                enemy->state = ES_WALK;
            }
            break;

        case ES_DEATH:
            if (Animation_IsFinished(&enemy->anim)) {
                enemy->active = false;
            }
            break;

        default:
            break;
    }
}
