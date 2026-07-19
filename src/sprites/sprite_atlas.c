#include "sprites/sprite_atlas.h"
#include <stdio.h>
#include <string.h>

static SpriteAtlasManager g_atlasMgr = {0};

bool SpriteAtlas_Init(void) {
    memset(&g_atlasMgr, 0, sizeof(g_atlasMgr));
    return true;
}

void SpriteAtlas_Shutdown(void) {
    for (u32 i = 0; i < g_atlasMgr.count; i++) {
        if (g_atlasMgr.atlases[i].loaded) {
            UnloadTexture(g_atlasMgr.atlases[i].tex);
        }
    }
    memset(&g_atlasMgr, 0, sizeof(g_atlasMgr));
}

SpriteAtlasManager* SpriteAtlas_GetManager(void) {
    return &g_atlasMgr;
}

s32 SpriteAtlas_Load(const char* filename, const char* name, s32 frameW, s32 frameH) {
    if (g_atlasMgr.count >= MK_MAX_ATLASES) return -1;

    char path[256];
    snprintf(path, sizeof(path), "assets/sprites/%s", filename);

    Image img = LoadImage(path);
    if (img.data == NULL) {
        fprintf(stderr, "Failed to load sprite atlas: %s\n", path);
        return -1;
    }

    s32 id = (s32)g_atlasMgr.count;
    SpriteAtlas* atlas = &g_atlasMgr.atlases[id];
    memset(atlas, 0, sizeof(SpriteAtlas));

    atlas->tex = LoadTextureFromImage(img);
    UnloadImage(img);

    if (atlas->tex.id == 0) {
        fprintf(stderr, "Failed to create atlas texture: %s\n", path);
        return -1;
    }

    atlas->frameWidth = frameW > 0 ? frameW : atlas->tex.width;
    atlas->frameHeight = frameH > 0 ? frameH : atlas->tex.height;
    atlas->frameCount = atlas->tex.width / atlas->frameWidth;
    atlas->loaded = true;
    strncpy(atlas->name, name, sizeof(atlas->name) - 1);
    atlas->name[sizeof(atlas->name) - 1] = '\0';

    g_atlasMgr.count++;
    return id;
}

void SpriteAtlas_Unload(s32 id) {
    if (id < 0 || id >= (s32)g_atlasMgr.count) return;
    SpriteAtlas* atlas = &g_atlasMgr.atlases[id];
    if (atlas->loaded) {
        UnloadTexture(atlas->tex);
        atlas->loaded = false;
    }
}

const SpriteAtlas* SpriteAtlas_GetById(s32 id) {
    if (id < 0 || id >= (s32)g_atlasMgr.count) return NULL;
    return &g_atlasMgr.atlases[id];
}

s32 SpriteAtlas_FindByName(const char* name) {
    for (u32 i = 0; i < g_atlasMgr.count; i++) {
        if (g_atlasMgr.atlases[i].loaded && strcmp(g_atlasMgr.atlases[i].name, name) == 0) {
            return (s32)i;
        }
    }
    return -1;
}

s32 SpriteAtlas_AddAnim(s32 atlasId, const char* name, s32 start, s32 end, f32 fps, AnimMode mode) {
    if (atlasId < 0 || atlasId >= (s32)g_atlasMgr.count) return -1;
    SpriteAtlas* atlas = &g_atlasMgr.atlases[atlasId];
    if (atlas->animCount >= MK_MAX_ANIMATIONS) return -1;

    s32 animId = atlas->animCount;
    AnimDef* def = &atlas->anims[animId];
    def->startFrame = start;
    def->endFrame = end;
    def->fps = fps > 0.0f ? fps : 10.0f;
    def->mode = mode;
    strncpy(def->name, name, sizeof(def->name) - 1);
    def->name[sizeof(def->name) - 1] = '\0';
    atlas->animCount++;
    return animId;
}

s32 SpriteAtlas_FindAnim(const SpriteAtlas* atlas, const char* name) {
    if (!atlas) return -1;
    for (s32 i = 0; i < atlas->animCount; i++) {
        if (strcmp(atlas->anims[i].name, name) == 0) return i;
    }
    return -1;
}

s32 SpriteAtlas_GetFrameIndex(const SpriteAtlas* atlas, s32 animId, f32 time) {
    if (!atlas || animId < 0 || animId >= atlas->animCount) return 0;

    const AnimDef* def = &atlas->anims[animId];
    s32 frameCount = def->endFrame - def->startFrame + 1;
    if (frameCount <= 0) return def->startFrame;

    f32 duration = frameCount / def->fps;
    f32 t = time;

    switch (def->mode) {
        case ANIM_ONCE:
            if (t >= duration) return def->endFrame;
            return def->startFrame + (s32)(t * def->fps) % frameCount;

        case ANIM_LOOP:
            return def->startFrame + (s32)(t * def->fps) % frameCount;

        case ANIM_REVERSE: {
            s32 frame = (s32)(t * def->fps) % frameCount;
            return def->endFrame - frame;
        }

        case ANIM_PINGPONG: {
            s32 cycle = (s32)(t * def->fps) % (frameCount * 2);
            if (cycle < frameCount) return def->startFrame + cycle;
            return def->endFrame - (cycle - frameCount);
        }
    }

    return def->startFrame;
}
