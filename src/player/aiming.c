#include "player/aiming.h"
#include "engine/engine.h"
#include "camera/camera.h"
#include <raylib.h>
#include <math.h>
#include <string.h>

static AimingSystem g_aim = {0};

bool Aiming_Init(void) {
    memset(&g_aim, 0, sizeof(g_aim));
    g_aim.sensitivity = 1.0f;
    g_aim.screenPos = (Vec2){ MK_SCREEN_WIDTH / 2.0f, MK_SCREEN_HEIGHT / 2.0f };
    return true;
}

void Aiming_Shutdown(void) {
    memset(&g_aim, 0, sizeof(g_aim));
}

AimingSystem* Aiming_Get(void) {
    return &g_aim;
}

void Aiming_Update(void) {
    if (g_aim.locked) return;

    /* Absolute mouse position scaled to virtual screen */
    Vector2 mousePos = GetMousePosition();
    f32 scaleX = (f32)MK_SCREEN_WIDTH / (f32)GetScreenWidth();
    f32 scaleY = (f32)MK_SCREEN_HEIGHT / (f32)GetScreenHeight();

    g_aim.screenPos.x = mousePos.x * scaleX;
    g_aim.screenPos.y = mousePos.y * scaleY;

    /* Clamp to screen */
    if (g_aim.screenPos.x < 0) g_aim.screenPos.x = 0;
    if (g_aim.screenPos.x >= MK_SCREEN_WIDTH) g_aim.screenPos.x = MK_SCREEN_WIDTH - 1;
    if (g_aim.screenPos.y < 0) g_aim.screenPos.y = 0;
    if (g_aim.screenPos.y >= MK_SCREEN_HEIGHT) g_aim.screenPos.y = MK_SCREEN_HEIGHT - 1;

    /* Compute world direction from screen offset */
    CameraSystem* cam = CameraSystem_Get();
    f32 ndcX = (g_aim.screenPos.x / MK_SCREEN_WIDTH) * 2.0f - 1.0f;
    f32 ndcY = (g_aim.screenPos.y / MK_SCREEN_HEIGHT) * 2.0f - 1.0f;

    f32 rayDirX = cam->dir.x + cam->plane.x * ndcX;
    f32 rayDirY = cam->dir.y + cam->plane.y * ndcX;

    f32 len = sqrtf(rayDirX*rayDirX + rayDirY*rayDirY);
    if (len > 0.0f) {
        g_aim.worldDir.x = rayDirX / len;
        g_aim.worldDir.y = rayDirY / len;
    }

    /* Vertical pitch from crosshair Y offset */
    g_aim.pitch = ndcY * 0.5f;
}

Vec2 Aiming_GetCrosshairPos(void) {
    return g_aim.screenPos;
}

Vec2 Aiming_GetWorldDirection(const Vec2* playerPos) {
    (void)playerPos;
    return g_aim.worldDir;
}

f32 Aiming_GetPitch(void) {
    return g_aim.pitch;
}
