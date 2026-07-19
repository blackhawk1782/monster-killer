#include "events/event_system.h"
#include "enemy/enemy.h"
#include "map/map.h"
#include "audio/audio_manager.h"
#include "camera/camera.h"
#include "renderer/renderer.h"
#include "particles/particles.h"
#include <string.h>
#include <stdio.h>

static EventSystem g_events = {0};

bool EventSystem_Init(void) {
    memset(&g_events, 0, sizeof(g_events));
    return true;
}

void EventSystem_Shutdown(void) {
    memset(&g_events, 0, sizeof(g_events));
}

EventSystem* EventSystem_Get(void) {
    return &g_events;
}

s32 EventSystem_Register(EventType type, const Vec2* pos, s32 p1, s32 p2, const char* text) {
    if (g_events.count >= MK_MAX_EVENTS) return -1;
    s32 id = (s32)g_events.count;
    GameEvent* e = &g_events.events[id];
    e->type = type;
    if (pos) e->pos = *pos;
    e->param1 = p1;
    e->param2 = p2;
    e->triggered = false;
    e->once = true;
    if (text) {
        strncpy(e->text, text, sizeof(e->text) - 1);
        e->text[sizeof(e->text) - 1] = '\0';
    }
    g_events.count++;
    return id;
}

void EventSystem_Trigger(s32 id) {
    if (id < 0 || id >= (s32)g_events.count) return;
    GameEvent* e = &g_events.events[id];
    if (e->triggered && e->once) return;
    e->triggered = true;

    switch (e->type) {
        case EVT_SPAWN_ENEMY:
            Enemy_Spawn(&e->pos, e->param1);
            break;
        case EVT_OPEN_DOOR:
            Map_OpenDoor(Map_Get(), e->param1, e->param2);
            break;
        case EVT_PLAY_SOUND:
            AudioManager_PlaySound(e->text);
            break;
        case EVT_EXPLODE:
            Particles_SpawnExplosion(&e->pos, 0.5f, 20, 5.0f);
            CameraSystem_ApplyShake(0.5f, 0.5f);
            break;
        case EVT_SWITCH_MUSIC:
            AudioManager_PlayMusic(e->text);
            break;
        case EVT_CAMERA_SHAKE:
            CameraSystem_ApplyShake((f32)e->param1 / 100.0f, (f32)e->param2 / 100.0f);
            break;
        case EVT_BOSS_INTRO:
            /* TODO: boss intro sequence */
            break;
        case EVT_CHANGE_WEATHER:
            /* TODO: weather system */
            break;
        case EVT_SHOW_MESSAGE:
            /* TODO: message display */
            break;
        default:
            break;
    }
}

void EventSystem_Update(f32 dt) {
    (void)dt;
    /* Check trigger conditions */
}
