#ifndef MK_TEXTURE_MANAGER_H
#define MK_TEXTURE_MANAGER_H

#include "types.h"
#include <raylib.h>

typedef struct {
    Texture2D tex;
    u32 width;
    u32 height;
    bool loaded;
    char name[64];
} ManagedTexture;

typedef struct {
    ManagedTexture textures[MK_MAX_TEXTURES];
    u32 count;
} TextureManager;

bool TextureManager_Init(void);
void TextureManager_Shutdown(void);
TextureManager* TextureManager_Get(void);

/* Load a texture from assets/textures/ */
s32  TextureManager_Load(const char* filename, const char* name);
void TextureManager_Unload(s32 id);

/* Access */
const ManagedTexture* TextureManager_GetById(s32 id);
s32 TextureManager_FindByName(const char* name);

/* Animated texture frame */
Texture2D TextureManager_GetFrame(s32 id, u32 frame, u32 maxFrames);

#endif
