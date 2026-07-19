#ifndef MK_TRIGGERS_H
#define MK_TRIGGERS_H

#include "types.h"

/* Trigger volumes that fire events */
typedef struct {
    Vec2 pos;
    f32 radius;
    s32 eventId;
    bool active;
} Trigger;

void Trigger_CheckAll(const Vec2* playerPos);

#endif
