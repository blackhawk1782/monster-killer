#include "player/player.h"
#include "player/aiming.h"
#include "weapons/weapon.h"
#include "engine/engine.h"
#include "camera/camera.h"
#include "utilities/math_utils.h"
#include <raylib.h>
#include <string.h>

static Player g_player = {0};

bool Player_Init(void) {
    memset(&g_player, 0, sizeof(g_player));
    g_player.maxHealth = 100;
    g_player.health = 100;
    g_player.lives = 3;
    g_player.alive = true;
    Aiming_Init();
    Weapon_Init();
    return true;
}

void Player_Shutdown(void) {
    Weapon_Shutdown();
    Aiming_Shutdown();
    memset(&g_player, 0, sizeof(g_player));
}

Player* Player_Get(void) {
    return &g_player;
}

void Player_Update(void) {
    if (!g_player.alive) return;

    f32 dt = Engine_GetDeltaTime();

    if (g_player.invincibleTimer > 0.0f) {
        g_player.invincibleTimer -= dt;
        if (g_player.invincibleTimer < 0.0f) g_player.invincibleTimer = 0.0f;
    }

    Aiming_Update();
    Weapon_Update(dt);

    if (IsKeyPressed(KEY_R)) {
        Weapon_Reload();
    }
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) || IsKeyDown(KEY_Z)) {
        Weapon_Fire();
    }

    /* Weapon switching — number keys */
    if (IsKeyPressed(KEY_ONE))   Weapon_Switch(WEAPON_PISTOL);
    if (IsKeyPressed(KEY_TWO))   Weapon_Switch(WEAPON_SHOTGUN);
    if (IsKeyPressed(KEY_THREE)) Weapon_Switch(WEAPON_SMG);
    if (IsKeyPressed(KEY_FOUR))  Weapon_Switch(WEAPON_MACHINEGUN);
    if (IsKeyPressed(KEY_FIVE))  Weapon_Switch(WEAPON_MAGNUM);
    if (IsKeyPressed(KEY_SIX))   Weapon_Switch(WEAPON_ROCKETLAUNCHER);

    /* Mouse wheel weapon switching */
    f32 wheel = GetMouseWheelMove();
    if (wheel > 0.0f) {
        WeaponType current = Weapon_GetCurrentType();
        WeaponType next = (current + 1) % WEAPON_COUNT;
        Weapon_Switch(next);
    } else if (wheel < 0.0f) {
        WeaponType current = Weapon_GetCurrentType();
        WeaponType prev = (current - 1 + WEAPON_COUNT) % WEAPON_COUNT;
        Weapon_Switch(prev);
    }

    /* Combo decay */
    if (g_player.combo > 0) {
        static f32 comboTimer = 0.0f;
        comboTimer += dt;
        if (comboTimer >= 3.0f) {
            g_player.combo = 0;
            comboTimer = 0.0f;
        }
    }
}

void Player_TakeDamage(s32 damage) {
    if (!g_player.alive || g_player.invincibleTimer > 0.0f) return;
    g_player.health -= damage;
    if (g_player.health <= 0) {
        g_player.health = 0;
        g_player.lives--;
        if (g_player.lives <= 0) {
            g_player.alive = false;
            Engine_SetState(GS_GAMEOVER);
        } else {
            g_player.health = g_player.maxHealth;
            g_player.invincibleTimer = 2.0f;
        }
    }
    CameraSystem_ApplyShake(0.3f, 0.3f);
}

void Player_AddScore(s32 points) {
    g_player.score += points;
    g_player.combo++;
}

void Player_Reset(void) {
    g_player.health = g_player.maxHealth;
    g_player.score = 0;
    g_player.combo = 0;
    g_player.accuracyHits = 0;
    g_player.accuracyShots = 0;
    g_player.accuracy = 0.0f;
    g_player.alive = true;
    g_player.invincibleTimer = 0.0f;
}
