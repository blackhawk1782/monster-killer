#include "enemy/enemy_states.h"
#include <string.h>

void EnemyState_Enter(Enemy* enemy, EnemyState newState) {
    if (!enemy) return;
    enemy->state = newState;
    /* Reset timers or play entry animations */
    switch (newState) {
        case ES_IDLE:
            enemy->reactionTimer = 0.0f;
            break;
        case ES_ATTACK:
            enemy->reactionTimer = 0.5f;
            break;
        case ES_SHOOT:
            enemy->cooldownTimer = 1.0f;
            break;
        default:
            break;
    }
}

void EnemyState_Update(Enemy* enemy, f32 dt) {
    if (!enemy) return;
    (void)dt;
    /* Per-state updates handled in enemy_ai.c */
}
