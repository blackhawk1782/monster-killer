#ifndef MK_RAYCASTER_H
#define MK_RAYCASTER_H

#include "types.h"
#include "camera/camera.h"
#include "map/map.h"

/* Z-buffer entry per screen column */
typedef struct {
    f32 distance;
    s32 texId;
    f32 texCoord;
    s32 drawStart;
    s32 drawEnd;
    bool hit;
} ZBufferColumn;

typedef struct {
    ZBufferColumn columns[MK_SCREEN_WIDTH];
    f32 floorDist[MK_SCREEN_HEIGHT];
    f32 ceilingDist[MK_SCREEN_HEIGHT];
} ZBuffer;

bool Raycaster_Init(void);
void Raycaster_Shutdown(void);

/* Main render pass: walls, floor, ceiling */
void Raycaster_Render(const CameraSystem* cam, const Map* map);

/* Z-buffer access for sprite clipping */
const ZBuffer* Raycaster_GetZBuffer(void);

/* Fog factor from distance [0..1] */
f32 Raycaster_FogFactor(f32 distance);

#endif
