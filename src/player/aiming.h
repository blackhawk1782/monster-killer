#ifndef MK_AIMING_H
#define MK_AIMING_H

#include "types.h"

typedef struct {
    Vec2 screenPos;
    Vec2 worldDir;
    f32 pitch;          /* Vertical aim angle in radians */
    f32 sensitivity;
    bool locked;
} AimingSystem;

bool Aiming_Init(void);
void Aiming_Shutdown(void);
AimingSystem* Aiming_Get(void);

void Aiming_Update(void);
Vec2 Aiming_GetCrosshairPos(void);
Vec2 Aiming_GetWorldDirection(const Vec2* playerPos);
f32  Aiming_GetPitch(void);

#endif
