#include "ui/ui.h"
#include "ui/hud.h"
#include "player/player.h"
#include "player/aiming.h"
#include "weapons/weapon.h"
#include "camera/camera.h"
#include "engine/engine.h"
#include <raylib.h>
#include <string.h>
#include <stdio.h>

static bool g_uiInitialized = false;

bool UI_Init(void) {
    g_uiInitialized = true;
    return true;
}

void UI_Shutdown(void) {
    g_uiInitialized = false;
}

void UI_UpdateTitle(void) {
    if (IsKeyPressed(KEY_ENTER)) {
        Engine_SetState(GS_MENU);
    }
}

void UI_DrawTitle(void) {
    ClearBackground(BLACK);
    const char* title = "MONSTER KILLER";
    s32 tw = MeasureText(title, 30);
    DrawText(title, (MK_SCREEN_WIDTH - tw) / 2, MK_SCREEN_HEIGHT / 3, 30, RED);

    const char* press = "PRESS ENTER";
    s32 pw = MeasureText(press, 10);
    DrawText(press, (MK_SCREEN_WIDTH - pw) / 2, MK_SCREEN_HEIGHT / 2, 10, WHITE);
}

void UI_UpdateMenu(void) {
    if (IsKeyPressed(KEY_ENTER)) {
        Engine_SetState(GS_PLAYING);
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        Engine_SetState(GS_TITLE);
    }
}

void UI_DrawMenu(void) {
    ClearBackground(BLACK);
    const char* title = "MAIN MENU";
    s32 tw = MeasureText(title, 20);
    DrawText(title, (MK_SCREEN_WIDTH - tw) / 2, 40, 20, WHITE);

    const char* items[] = { "START MISSION", "OPTIONS", "QUIT" };
    for (s32 i = 0; i < 3; i++) {
        s32 iw = MeasureText(items[i], 10);
        DrawText(items[i], (MK_SCREEN_WIDTH - iw) / 2, 80 + i * 20, 10, YELLOW);
    }
}

void UI_DrawHUD(void) {
    if (!g_uiInitialized) return;
    Player* p = Player_Get();
    Weapon* w = Weapon_GetCurrent();

    UI_DrawCrosshair();

    if (p) {
        UI_DrawHealth(p->health, p->maxHealth);
        UI_DrawScore(p->score);
        UI_DrawCombo(p->combo);
    }
    if (w) {
        UI_DrawAmmo(w->currentMag, w->ammo);
        UI_DrawWeaponName(w->name);
        if (w->reloading) {
            const char* txt = "RELOADING...";
            s32 tw = MeasureText(txt, 10);
            DrawText(txt, (MK_SCREEN_WIDTH - tw) / 2, MK_SCREEN_HEIGHT - 50, 10, ORANGE);
        }
    }
    if (p && p->accuracyShots > 0) {
        f32 acc = (f32)p->accuracyHits / (f32)p->accuracyShots;
        UI_DrawAccuracy(acc);
    }

    /* Laser sight from crosshair */
    UI_DrawLaserSight(CameraSystem_Get());
}

void UI_DrawPause(void) {
    DrawRectangle(0, 0, MK_SCREEN_WIDTH, MK_SCREEN_HEIGHT, (Color){0, 0, 0, 180});
    const char* txt = "PAUSED";
    s32 tw = MeasureText(txt, 20);
    DrawText(txt, (MK_SCREEN_WIDTH - tw) / 2, MK_SCREEN_HEIGHT / 2 - 10, 20, WHITE);
}

void UI_UpdateGameOver(void) {
    if (IsKeyPressed(KEY_ENTER)) {
        Player_Reset();
        Engine_SetState(GS_MENU);
    }
}

void UI_DrawGameOver(void) {
    DrawRectangle(0, 0, MK_SCREEN_WIDTH, MK_SCREEN_HEIGHT, (Color){0, 0, 0, 200});
    const char* txt = "GAME OVER";
    s32 tw = MeasureText(txt, 20);
    DrawText(txt, (MK_SCREEN_WIDTH - tw) / 2, MK_SCREEN_HEIGHT / 3, 20, RED);

    const char* cont = "PRESS ENTER TO CONTINUE";
    s32 cw = MeasureText(cont, 10);
    DrawText(cont, (MK_SCREEN_WIDTH - cw) / 2, MK_SCREEN_HEIGHT / 2, 10, WHITE);
}

void UI_UpdateMissionComplete(void) {
    if (IsKeyPressed(KEY_ENTER)) {
        Engine_SetState(GS_MENU);
    }
}

void UI_DrawMissionComplete(void) {
    ClearBackground(BLACK);
    const char* txt = "MISSION COMPLETE";
    s32 tw = MeasureText(txt, 20);
    DrawText(txt, (MK_SCREEN_WIDTH - tw) / 2, MK_SCREEN_HEIGHT / 3, 20, GREEN);

    Player* p = Player_Get();
    if (p) {
        char buf[64];
        snprintf(buf, sizeof(buf), "SCORE: %d", p->score);
        s32 sw = MeasureText(buf, 10);
        DrawText(buf, (MK_SCREEN_WIDTH - sw) / 2, MK_SCREEN_HEIGHT / 2, 10, WHITE);
    }
}

void UI_DrawCrosshair(void) {
    Vec2 pos = Aiming_GetCrosshairPos();
    s32 cx = (s32)pos.x;
    s32 cy = (s32)pos.y;

    /* Outer brackets */
    DrawLine(cx - 8, cy - 8, cx - 3, cy - 3, GREEN);
    DrawLine(cx + 3, cy - 3, cx + 8, cy - 8, GREEN);
    DrawLine(cx - 8, cy + 8, cx - 3, cy + 3, GREEN);
    DrawLine(cx + 3, cy + 3, cx + 8, cy + 8, GREEN);

    /* Center dot */
    DrawPixel(cx, cy, WHITE);
}

void UI_DrawLaserSight(const struct CameraSystem* cam) {
    if (!cam) return;
    Vec2 aimDir = Aiming_GetWorldDirection(NULL);
    Vec2 start = Aiming_GetCrosshairPos();

    /* Project a short line from crosshair in aim direction (screen-space approx) */
    f32 len = 20.0f;
    s32 x0 = (s32)start.x;
    s32 y0 = (s32)start.y;
    s32 x1 = (s32)(start.x + aimDir.x * len);
    s32 y1 = (s32)(start.y + aimDir.y * len);
    DrawLine(x0, y0, x1, y1, (Color){0, 255, 0, 128});
}

void UI_DrawHealth(s32 health, s32 maxHealth) {
    char buf[32];
    snprintf(buf, sizeof(buf), "HP %d/%d", health, maxHealth);
    DrawText(buf, 4, MK_SCREEN_HEIGHT - 20, 10, RED);
}

void UI_DrawAmmo(s32 current, s32 total) {
    char buf[32];
    if (current <= 0 && total > 0) {
        snprintf(buf, sizeof(buf), "RELOAD!");
        DrawText(buf, MK_SCREEN_WIDTH - 60, MK_SCREEN_HEIGHT - 20, 10, RED);
    } else {
        snprintf(buf, sizeof(buf), "%d / %d", current, total);
        DrawText(buf, MK_SCREEN_WIDTH - 60, MK_SCREEN_HEIGHT - 20, 10, YELLOW);
    }
}

void UI_DrawScore(s32 score) {
    char buf[32];
    snprintf(buf, sizeof(buf), "SCORE %d", score);
    DrawText(buf, 4, 4, 10, WHITE);
}

void UI_DrawCombo(s32 combo) {
    if (combo < 2) return;
    char buf[32];
    snprintf(buf, sizeof(buf), "COMBO x%d", combo);
    s32 tw = MeasureText(buf, 10);
    DrawText(buf, (MK_SCREEN_WIDTH - tw) / 2, 20, 10, ORANGE);
}

void UI_DrawWeaponName(const char* name) {
    if (!name) return;

    /* Weapon background bar */
    s32 barW = 80;
    s32 barH = 14;
    s32 barX = (MK_SCREEN_WIDTH - barW) / 2;
    s32 barY = MK_SCREEN_HEIGHT - 36;
    DrawRectangle(barX, barY, barW, barH, (Color){40, 40, 50, 200});
    DrawRectangleLines(barX, barY, barW, barH, SKYBLUE);

    s32 nw = MeasureText(name, 10);
    DrawText(name, (MK_SCREEN_WIDTH - nw) / 2, barY + 3, 10, WHITE);
}

void UI_DrawAccuracy(f32 accuracy) {
    char buf[32];
    snprintf(buf, sizeof(buf), "ACC %.0f%%", accuracy * 100.0f);
    DrawText(buf, 4, 16, 10, GREEN);
}

void UI_DrawBossHealth(const char* name, s32 health, s32 maxHealth) {
    if (!name || maxHealth <= 0) return;
    s32 barW = 100;
    s32 barH = 8;
    s32 x = (MK_SCREEN_WIDTH - barW) / 2;
    s32 y = 10;
    s32 fill = (s32)((f32)health / maxHealth * barW);
    if (fill < 0) fill = 0;
    if (fill > barW) fill = barW;

    DrawRectangle(x, y, barW, barH, DARKGRAY);
    DrawRectangle(x, y, fill, barH, RED);
    s32 nw = MeasureText(name, 8);
    DrawText(name, (MK_SCREEN_WIDTH - nw) / 2, y + barH + 2, 8, WHITE);
}
