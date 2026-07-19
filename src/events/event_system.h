#ifndef MK_EVENT_SYSTEM_H
#define MK_EVENT_SYSTEM_H

#include "types.h"

typedef enum {
    EVT_SPAWN_ENEMY,
    EVT_OPEN_DOOR,
    EVT_PLAY_SOUND,
    EVT_EXPLODE,
    EVT_SWITCH_MUSIC,
    EVT_CAMERA_SHAKE,
    EVT_BOSS_INTRO,
    EVT_CHANGE_WEATHER,
    EVT_SHOW_MESSAGE,
    EVT_TRIGGER_COUNT
} EventType;

typedef struct {
    EventType type;
    Vec2 pos;
    s32 param1;
    s32 param2;
    char text[64];
    bool triggered;
    bool once;
} GameEvent;

typedef struct {
    GameEvent events[MK_MAX_EVENTS];
    u32 count;
} EventSystem;

bool EventSystem_Init(void);
void EventSystem_Shutdown(void);
EventSystem* EventSystem_Get(void);

s32  EventSystem_Register(EventType type, const Vec2* pos, s32 p1, s32 p2, const char* text);
void EventSystem_Trigger(s32 id);
void EventSystem_Update(f32 dt);

#endif
