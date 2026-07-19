#ifndef MK_RAIL_PATH_H
#define MK_RAIL_PATH_H

#include "types.h"

typedef enum {
    RAIL_MOVE,
    RAIL_WAIT,
    RAIL_ROTATE,
    RAIL_PAN,
    RAIL_ZOOM,
    RAIL_SHAKE,
    RAIL_EVENT
} RailNodeType;

typedef struct {
    Vec2 position;
    f32 angle;
    f32 pitch;
    f32 speed;
    f32 waitTime;
    f32 zoom;
    RailNodeType type;
    s32 eventId;
    char name[32];
} RailNode;

typedef struct {
    RailNode nodes[MK_MAX_RAIL_NODES];
    u32 count;
    u32 current;
    f32 progress;
    f32 timer;
    bool active;
    bool loop;
} RailPath;

bool RailPath_Init(RailPath* path);
void RailPath_Reset(RailPath* path);
void RailPath_AddNode(RailPath* path, const RailNode* node);
void RailPath_Update(RailPath* path, f32 dt);
bool RailPath_IsComplete(const RailPath* path);

#endif
