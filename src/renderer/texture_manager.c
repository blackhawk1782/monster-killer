#include "renderer/texture_manager.h"
#include <stdio.h>
#include <string.h>

static TextureManager g_texMgr = {0};

bool TextureManager_Init(void) {
    memset(&g_texMgr, 0, sizeof(g_texMgr));
    g_texMgr.count = 0;
    return true;
}

void TextureManager_Shutdown(void) {
    for (u32 i = 0; i < g_texMgr.count; i++) {
        if (g_texMgr.textures[i].loaded) {
            UnloadTexture(g_texMgr.textures[i].tex);
        }
    }
    memset(&g_texMgr, 0, sizeof(g_texMgr));
}

TextureManager* TextureManager_Get(void) {
    return &g_texMgr;
}

s32 TextureManager_Load(const char* filename, const char* name) {
    if (g_texMgr.count >= MK_MAX_TEXTURES) return -1;

    char path[256];
    snprintf(path, sizeof(path), "assets/textures/%s", filename);

    Image img = LoadImage(path);
    if (img.data == NULL) {
        fprintf(stderr, "Failed to load texture: %s\n", path);
        return -1;
    }

    s32 id = (s32)g_texMgr.count;
    ManagedTexture* mt = &g_texMgr.textures[id];
    mt->tex = LoadTextureFromImage(img);
    UnloadImage(img);

    if (mt->tex.id == 0) {
        fprintf(stderr, "Failed to create texture from image: %s\n", path);
        return -1;
    }

    mt->width = mt->tex.width;
    mt->height = mt->tex.height;
    mt->loaded = true;
    strncpy(mt->name, name, sizeof(mt->name) - 1);
    mt->name[sizeof(mt->name) - 1] = '\0';

    g_texMgr.count++;
    return id;
}

void TextureManager_Unload(s32 id) {
    if (id < 0 || id >= (s32)g_texMgr.count) return;
    ManagedTexture* mt = &g_texMgr.textures[id];
    if (mt->loaded) {
        UnloadTexture(mt->tex);
        mt->loaded = false;
    }
}

const ManagedTexture* TextureManager_GetById(s32 id) {
    if (id < 0 || id >= (s32)g_texMgr.count) return NULL;
    return &g_texMgr.textures[id];
}

s32 TextureManager_FindByName(const char* name) {
    for (u32 i = 0; i < g_texMgr.count; i++) {
        if (g_texMgr.textures[i].loaded && strcmp(g_texMgr.textures[i].name, name) == 0) {
            return (s32)i;
        }
    }
    return -1;
}

Texture2D TextureManager_GetFrame(s32 id, u32 frame, u32 maxFrames) {
    const ManagedTexture* mt = TextureManager_GetById(id);
    if (!mt || !mt->loaded || maxFrames == 0) return (Texture2D){0};

    /* Assume horizontal strip of equal frames */
    u32 frameW = mt->width / maxFrames;
    u32 idx = frame % maxFrames;

    Texture2D sub = mt->tex;
    /* raylib doesn't support sub-texture rendering natively without custom UVs,
       so callers should use source rectangles. Return base texture. */
    (void)frameW; (void)idx;
    return sub;
}
