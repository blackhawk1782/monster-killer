#ifndef MK_BLOOD_H
#define MK_BLOOD_H

#include "types.h"
#include "camera/camera.h"

void Blood_Spawn(const Vec2* pos, s32 amount);
void Blood_Draw(const CameraSystem* cam);

#endif
