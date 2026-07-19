#include "ui/hud.h"
#include <raylib.h>
#include <stdio.h>

void HUD_DrawRank(const char* rank) {
    if (!rank) return;
    s32 rw = MeasureText(rank, 12);
    DrawText(rank, MK_SCREEN_WIDTH - rw - 4, 4, 12, GOLD);
}

void HUD_DrawTimer(f32 time) {
    char buf[32];
    s32 mins = (s32)(time / 60.0f);
    s32 secs = (s32)(time) % 60;
    snprintf(buf, sizeof(buf), "%02d:%02d", mins, secs);
    s32 tw = MeasureText(buf, 10);
    DrawText(buf, (MK_SCREEN_WIDTH - tw) / 2, 4, 10, WHITE);
}

void HUD_DrawAccuracy(f32 accuracy) {
    char buf[32];
    snprintf(buf, sizeof(buf), "ACC %.0f%%", accuracy * 100.0f);
    DrawText(buf, 4, 16, 10, GREEN);
}

void HUD_DrawLives(s32 lives) {
    char buf[32];
    snprintf(buf, sizeof(buf), "LIVES x%d", lives);
    DrawText(buf, MK_SCREEN_WIDTH - 60, 4, 10, RED);
}
