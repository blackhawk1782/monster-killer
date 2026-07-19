#ifndef MK_PLAYER_H
#define MK_PLAYER_H

#include "types.h"

typedef struct {
    s32 health;
    s32 maxHealth;
    s32 lives;
    s32 score;
    s32 combo;
    s32 accuracyHits;
    s32 accuracyShots;
    f32 accuracy;
    bool alive;
    f32 invincibleTimer;
} Player;

bool Player_Init(void);
void Player_Shutdown(void);
Player* Player_Get(void);

void Player_Update(void);
void Player_TakeDamage(s32 damage);
void Player_AddScore(s32 points);
void Player_Reset(void);

#endif
