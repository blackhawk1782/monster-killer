#ifndef MK_CAMERA_H
#define MK_CAMERA_H

#include "types.h"

typedef struct CameraSystem {
    Vec2 pos;           /* Logical position for rail path / collision */
    Vec2 renderPos;     /* pos + shakeOffset, used by renderer only */
    Vec2 dir;
    Vec2 plane;
    f32 pitch;
    f32 fov;
    f32 bobPhase;
    f32 bobAmount;
    f32 shakeIntensity;
    f32 shakeDuration;
    f32 shakeOffsetX;
    f32 shakeOffsetY;
    f32 zoom;
} CameraSystem;

bool CameraSystem_Init(void);
void CameraSystem_Shutdown(void);
CameraSystem* CameraSystem_Get(void);

/* Update per frame */
void CameraSystem_Update(void);

/* Transformations */
void CameraSystem_SetPosition(f32 x, f32 y);
void CameraSystem_SetDirection(f32 angle);
void CameraSystem_SetPitch(f32 pitch);
void CameraSystem_SetFOV(f32 fov);

/* Effects */
void CameraSystem_ApplyShake(f32 intensity, f32 duration);
void CameraSystem_SetBob(f32 amount);
void CameraSystem_SetZoom(f32 zoom);

/* Utilities */
void CameraSystem_BuildPlane(void);

#endif
