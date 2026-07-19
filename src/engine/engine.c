#include "engine/engine.h"
#include "renderer/renderer.h"
#include "renderer/texture_manager.h"
#include "camera/camera.h"
#include "map/map.h"
#include "player/player.h"
#include "ui/ui.h"
#include "audio/audio_manager.h"
#include "enemy/enemy.h"
#include "sprites/sprites.h"
#include "particles/particles.h"
#include "events/event_system.h"
#include "utilities/math_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Engine g_engine = {0};

Engine* Engine_Get(void) { return &g_engine; }

bool Engine_Init(s32 screenW, s32 screenH, const char* title) {
    memset(&g_engine, 0, sizeof(g_engine));
    g_engine.running = true;
    g_engine.state = GS_BOOT;
    g_engine.timeScale = 1.0f;
    g_engine.targetFps = 60.0f;
    g_engine.screenW = screenW;
    g_engine.screenH = screenH;

    InitWindow(screenW, screenH, title);
    if (!IsWindowReady()) {
        fprintf(stderr, "Failed to create window\n");
        return false;
    }

    SetTargetFPS((s32)g_engine.targetFps);
    SetExitKey(KEY_NULL);
    HideCursor();

    g_engine.screenTarget = LoadRenderTexture(MK_SCREEN_WIDTH, MK_SCREEN_HEIGHT);
    if (g_engine.screenTarget.id == 0) {
        fprintf(stderr, "Failed to create render target\n");
        CloseWindow();
        return false;
    }

    if (!TextureManager_Init()) {
        fprintf(stderr, "TextureManager init failed\n");
        UnloadRenderTexture(g_engine.screenTarget);
        CloseWindow();
        return false;
    }

    if (!AudioManager_Init()) {
        fprintf(stderr, "AudioManager init failed\n");
        TextureManager_Shutdown();
        UnloadRenderTexture(g_engine.screenTarget);
        CloseWindow();
        return false;
    }

    if (!CameraSystem_Init()) {
        fprintf(stderr, "CameraSystem init failed\n");
        AudioManager_Shutdown();
        TextureManager_Shutdown();
        UnloadRenderTexture(g_engine.screenTarget);
        CloseWindow();
        return false;
    }

    if (!Map_Init()) {
        fprintf(stderr, "Map init failed\n");
        CameraSystem_Shutdown();
        AudioManager_Shutdown();
        TextureManager_Shutdown();
        UnloadRenderTexture(g_engine.screenTarget);
        CloseWindow();
        return false;
    }

    if (!Player_Init()) {
        fprintf(stderr, "Player init failed\n");
        Map_Shutdown();
        CameraSystem_Shutdown();
        AudioManager_Shutdown();
        TextureManager_Shutdown();
        UnloadRenderTexture(g_engine.screenTarget);
        CloseWindow();
        return false;
    }

    if (!UI_Init()) {
        fprintf(stderr, "UI init failed\n");
        Player_Shutdown();
        Map_Shutdown();
        CameraSystem_Shutdown();
        AudioManager_Shutdown();
        TextureManager_Shutdown();
        UnloadRenderTexture(g_engine.screenTarget);
        CloseWindow();
        return false;
    }

    if (!Renderer_Init()) {
        fprintf(stderr, "Renderer init failed\n");
        UI_Shutdown();
        Player_Shutdown();
        Map_Shutdown();
        CameraSystem_Shutdown();
        AudioManager_Shutdown();
        TextureManager_Shutdown();
        UnloadRenderTexture(g_engine.screenTarget);
        CloseWindow();
        return false;
    }

    if (!Sprite_Init()) {
        fprintf(stderr, "Sprite init failed\n");
        Renderer_Shutdown();
        UI_Shutdown();
        Player_Shutdown();
        Map_Shutdown();
        CameraSystem_Shutdown();
        AudioManager_Shutdown();
        TextureManager_Shutdown();
        UnloadRenderTexture(g_engine.screenTarget);
        CloseWindow();
        return false;
    }

    if (!Enemy_Init()) {
        fprintf(stderr, "Enemy init failed\n");
        Sprite_Shutdown();
        Renderer_Shutdown();
        UI_Shutdown();
        Player_Shutdown();
        Map_Shutdown();
        CameraSystem_Shutdown();
        AudioManager_Shutdown();
        TextureManager_Shutdown();
        UnloadRenderTexture(g_engine.screenTarget);
        CloseWindow();
        return false;
    }

    if (!Particles_Init()) {
        fprintf(stderr, "Particles init failed\n");
        Enemy_Shutdown();
        Sprite_Shutdown();
        Renderer_Shutdown();
        UI_Shutdown();
        Player_Shutdown();
        Map_Shutdown();
        CameraSystem_Shutdown();
        AudioManager_Shutdown();
        TextureManager_Shutdown();
        UnloadRenderTexture(g_engine.screenTarget);
        CloseWindow();
        return false;
    }

    if (!EventSystem_Init()) {
        fprintf(stderr, "EventSystem init failed\n");
        Particles_Shutdown();
        Enemy_Shutdown();
        Sprite_Shutdown();
        Renderer_Shutdown();
        UI_Shutdown();
        Player_Shutdown();
        Map_Shutdown();
        CameraSystem_Shutdown();
        AudioManager_Shutdown();
        TextureManager_Shutdown();
        UnloadRenderTexture(g_engine.screenTarget);
        CloseWindow();
        return false;
    }

    g_engine.state = GS_TITLE;
    return true;
}

void Engine_Shutdown(void) {
    EventSystem_Shutdown();
    Particles_Shutdown();
    Enemy_Shutdown();
    Sprite_Shutdown();
    Renderer_Shutdown();
    UI_Shutdown();
    Player_Shutdown();
    Map_Shutdown();
    CameraSystem_Shutdown();
    AudioManager_Shutdown();
    TextureManager_Shutdown();
    UnloadRenderTexture(g_engine.screenTarget);
    CloseWindow();
    memset(&g_engine, 0, sizeof(g_engine));
}

void Engine_BeginFrame(void) {
    g_engine.deltaTime = GetFrameTime() * g_engine.timeScale;
    g_engine.frameCount++;
}

void Engine_EndFrame(void) {
    /* Render low-res screen target scaled to window */
    BeginDrawing();
    ClearBackground(BLACK);

    Rectangle src = { 0.0f, 0.0f,
                      (f32)g_engine.screenTarget.texture.width,
                     -(f32)g_engine.screenTarget.texture.height };
    Rectangle dst = { 0.0f, 0.0f,
                      (f32)GetScreenWidth(),
                      (f32)GetScreenHeight() };
    DrawTexturePro(g_engine.screenTarget.texture, src, dst, (Vector2){0,0}, 0.0f, WHITE);

    if (g_engine.showDebug) Engine_DrawDebug();
    EndDrawing();

    if (WindowShouldClose()) g_engine.running = false;
}

void Engine_Run(void) {
    while (g_engine.running) {
        Engine_BeginFrame();

        switch (g_engine.state) {
            case GS_TITLE:
                UI_UpdateTitle();
                if (IsKeyPressed(KEY_ENTER)) Engine_SetState(GS_MENU);
                break;
            case GS_MENU:
                UI_UpdateMenu();
                break;
            case GS_PLAYING:
                CameraSystem_Update();
                Player_Update();
                Enemy_UpdateAll(g_engine.deltaTime);
                Particles_Update(g_engine.deltaTime);
                Sprite_UpdateAll(g_engine.deltaTime);
                Map_Update();
                break;
            case GS_PAUSED:
                if (IsKeyPressed(KEY_ESCAPE)) Engine_SetState(GS_PLAYING);
                break;
            case GS_GAMEOVER:
                UI_UpdateGameOver();
                break;
            case GS_MISSION_COMPLETE:
                UI_UpdateMissionComplete();
                break;
            default:
                break;
        }

        /* Render to low-res target */
        BeginTextureMode(g_engine.screenTarget);
        ClearBackground(BLACK);

        switch (g_engine.state) {
            case GS_TITLE:
                UI_DrawTitle();
                break;
            case GS_MENU:
                UI_DrawMenu();
                break;
            case GS_PLAYING:
            case GS_PAUSED:
                Renderer_DrawFrame();
                UI_DrawHUD();
                if (g_engine.state == GS_PAUSED) UI_DrawPause();
                break;
            case GS_GAMEOVER:
                Renderer_DrawFrame();
                UI_DrawGameOver();
                break;
            case GS_MISSION_COMPLETE:
                UI_DrawMissionComplete();
                break;
            default:
                break;
        }
        EndTextureMode();

        Engine_EndFrame();
    }
}

void Engine_SetState(GameState state) {
    if (state == GS_PLAYING && g_engine.state != GS_PLAYING) {
        /* Spawn test enemies in the horizontal corridor at y=4 */
        Vec2 e1 = { 5.0f, 4.0f };
        Vec2 e2 = { 8.0f, 4.0f };
        Vec2 e3 = { 11.0f, 4.0f };
        Enemy_Spawn(&e1, 0);
        Enemy_Spawn(&e2, 0);
        Enemy_Spawn(&e3, 0);
    }
    g_engine.state = state;
}

GameState Engine_GetState(void) {
    return g_engine.state;
}

f32 Engine_GetDeltaTime(void) {
    return g_engine.deltaTime;
}

f32 Engine_GetTime(void) {
    return (f32)GetTime();
}

void Engine_SetTimeScale(f32 scale) {
    g_engine.timeScale = scale;
}

void Engine_ToggleDebug(void) {
    g_engine.showDebug = !g_engine.showDebug;
}

void Engine_DrawDebug(void) {
    DrawFPS(10, 10);
    DrawText(TextFormat("State: %d", g_engine.state), 10, 30, 20, GREEN);
    DrawText(TextFormat("Delta: %.4f", g_engine.deltaTime), 10, 50, 20, GREEN);
}
