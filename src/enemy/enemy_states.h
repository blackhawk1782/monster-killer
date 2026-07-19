#ifndef MK_ENEMY_STATES_H
#define MK_ENEMY_STATES_H

#include "enemy/enemy.h"

void EnemyState_Enter(Enemy* enemy, EnemyState newState);
void EnemyState_Update(Enemy* enemy, f32 dt);

#endif
