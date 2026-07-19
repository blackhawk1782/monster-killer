#ifndef MK_SPRITES_H
#define MK_SPRITES_H

#include "types.h"
#include "camera/camera.h"
#include "map/map.h"

/* Sprite instance in world space */
typedef struct {
    Vec2 pos;
    f32 z;
    f32 scale;
    s32 atlasId;
    s32 animId;
    f32 animTime;
    bool visible;
    bool billboard;
    f32 distance;
    Color32 tint;
} Sprite;

typedef struct {
    Sprite sprites[MK_MAX_SPRITES];
    u32 count;
    u32 activeCount;
} SpriteManager;

bool Sprite_Init(void);
void Sprite_Shutdown(void);
SpriteManager* Sprite_GetManager(void);

/* Sprite lifecycle */
s32  Sprite_Spawn(const Vec2* pos, f32 z, s32 atlasId);
void Sprite_Destroy(s32 id);
void Sprite_ClearAll(void);

/* Updates */
void Sprite_UpdateAll(f32 dt);
void Sprite_DrawAll(const CameraSystem* cam, const Map* map);

/* Animation */
void Sprite_SetAnimation(s32 id, s32 animId);
void Sprite_SetScale(s32 id, f32 scale);
void Sprite_SetVisible(s32 id, bool visible);

#endif
