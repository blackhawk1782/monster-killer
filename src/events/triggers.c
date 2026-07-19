#include "events/triggers.h"
#include "events/event_system.h"
#include <math.h>

void Trigger_CheckAll(const Vec2* playerPos) {
    if (!playerPos) return;
    EventSystem* es = EventSystem_Get();
    for (u32 i = 0; i < es->count; i++) {
        GameEvent* e = &es->events[i];
        if (e->triggered) continue;
        f32 dx = e->pos.x - playerPos->x;
        f32 dy = e->pos.y - playerPos->y;
        if (dx*dx + dy*dy < 2.0f) {
            EventSystem_Trigger((s32)i);
        }
    }
}
