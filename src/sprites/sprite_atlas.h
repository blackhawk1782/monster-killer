#ifndef MK_SPRITE_ATLAS_H
#define MK_SPRITE_ATLAS_H

#include "types.h"
#include <raylib.h>

typedef enum {
    ANIM_LOOP,
    ANIM_ONCE,
    ANIM_PINGPONG,
    ANIM_REVERSE
} AnimMode;

typedef struct {
    s32 startFrame;
    s32 endFrame;
    f32 fps;
    AnimMode mode;
    char name[32];
} AnimDef;

typedef struct {
    Texture2D tex;
    s32 frameWidth;
    s32 frameHeight;
    s32 frameCount;
    s32 animCount;
    AnimDef anims[MK_MAX_ANIMATIONS];
    bool loaded;
    char name[64];
} SpriteAtlas;

typedef struct {
    SpriteAtlas atlases[MK_MAX_ATLASES];
    u32 count;
} SpriteAtlasManager;

bool SpriteAtlas_Init(void);
void SpriteAtlas_Shutdown(void);
SpriteAtlasManager* SpriteAtlas_GetManager(void);

/* Load atlas from image file with frame dimensions */
s32  SpriteAtlas_Load(const char* filename, const char* name, s32 frameW, s32 frameH);
void SpriteAtlas_Unload(s32 id);

/* Access */
const SpriteAtlas* SpriteAtlas_GetById(s32 id);
s32 SpriteAtlas_FindByName(const char* name);

/* Animation */
s32  SpriteAtlas_AddAnim(s32 atlasId, const char* name, s32 start, s32 end, f32 fps, AnimMode mode);
s32  SpriteAtlas_FindAnim(const SpriteAtlas* atlas, const char* name);
s32  SpriteAtlas_GetFrameIndex(const SpriteAtlas* atlas, s32 animId, f32 time);

#endif
