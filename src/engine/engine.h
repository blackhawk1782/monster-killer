#ifndef MK_ENGINE_H
#define MK_ENGINE_H

#include "types.h"
#include <raylib.h>

typedef enum {
    GS_BOOT,
    GS_TITLE,
    GS_MENU,
    GS_LOADING,
    GS_PLAYING,
    GS_PAUSED,
    GS_GAMEOVER,
    GS_MISSION_COMPLETE,
    GS_CREDITS
} GameState;

typedef struct {
    GameState state;
    bool running;
    bool paused;
    f32 deltaTime;
    f32 timeScale;
    u32 frameCount;
    u32 screenW;
    u32 screenH;
    RenderTexture2D screenTarget;
    bool showDebug;
    f32 targetFps;
} Engine;

/* Global engine instance accessor */
Engine* Engine_Get(void);

/* Lifecycle */
bool Engine_Init(s32 screenW, s32 screenH, const char* title);
void Engine_Shutdown(void);
void Engine_Run(void);

/* Frame control */
void Engine_BeginFrame(void);
void Engine_EndFrame(void);
void Engine_SetState(GameState state);
GameState Engine_GetState(void);

/* Time */
f32  Engine_GetDeltaTime(void);
f32  Engine_GetTime(void);
void Engine_SetTimeScale(f32 scale);

/* Debug */
void Engine_ToggleDebug(void);
void Engine_DrawDebug(void);

#endif
