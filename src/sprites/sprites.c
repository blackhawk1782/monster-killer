#include "sprites/sprites.h"
#include "sprites/sprite_atlas.h"
#include "raycaster/raycaster.h"
#include "utilities/math_utils.h"
#include "engine/engine.h"
#include <raylib.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

static SpriteManager g_sprMgr = {0};

bool Sprite_Init(void) {
    memset(&g_sprMgr, 0, sizeof(g_sprMgr));
    return true;
}

void Sprite_Shutdown(void) {
    memset(&g_sprMgr, 0, sizeof(g_sprMgr));
}

SpriteManager* Sprite_GetManager(void) {
    return &g_sprMgr;
}

s32 Sprite_Spawn(const Vec2* pos, f32 z, s32 atlasId) {
    if (g_sprMgr.count >= MK_MAX_SPRITES) return -1;
    s32 id = (s32)g_sprMgr.count;
    Sprite* s = &g_sprMgr.sprites[id];
    memset(s, 0, sizeof(Sprite));
    s->pos = *pos;
    s->z = z;
    s->atlasId = atlasId;
    s->scale = 1.0f;
    s->visible = true;
    s->billboard = true;
    s->tint = (Color32){255, 255, 255, 255};
    g_sprMgr.count++;
    g_sprMgr.activeCount++;
    return id;
}

void Sprite_Destroy(s32 id) {
    if (id < 0 || id >= (s32)g_sprMgr.count) return;
    g_sprMgr.sprites[id].visible = false;
    g_sprMgr.activeCount--;
}

void Sprite_ClearAll(void) {
    memset(&g_sprMgr, 0, sizeof(g_sprMgr));
}

void Sprite_SetAnimation(s32 id, s32 animId) {
    if (id < 0 || id >= (s32)g_sprMgr.count) return;
    g_sprMgr.sprites[id].animId = animId;
    g_sprMgr.sprites[id].animTime = 0.0f;
}

void Sprite_SetScale(s32 id, f32 scale) {
    if (id < 0 || id >= (s32)g_sprMgr.count) return;
    g_sprMgr.sprites[id].scale = scale;
}

void Sprite_SetVisible(s32 id, bool visible) {
    if (id < 0 || id >= (s32)g_sprMgr.count) return;
    g_sprMgr.sprites[id].visible = visible;
}

/* Depth sort comparator */
static int Sprite_CompareDepth(const void* a, const void* b) {
    const Sprite* sa = *(const Sprite**)a;
    const Sprite* sb = *(const Sprite**)b;
    if (sa->distance < sb->distance) return 1;
    if (sa->distance > sb->distance) return -1;
    return 0;
}

void Sprite_UpdateAll(f32 dt) {
    for (u32 i = 0; i < g_sprMgr.count; i++) {
        Sprite* s = &g_sprMgr.sprites[i];
        if (!s->visible) continue;
        s->animTime += dt;
    }
}

void Sprite_DrawAll(const CameraSystem* cam, const Map* map) {
    if (!cam || !map) return;

    const ZBuffer* zbuf = Raycaster_GetZBuffer();
    if (!zbuf) return;

    /* Compute distances and build active list */
    Sprite* sorted[MK_MAX_SPRITES];
    u32 sortCount = 0;

    for (u32 i = 0; i < g_sprMgr.count; i++) {
        Sprite* s = &g_sprMgr.sprites[i];
        if (!s->visible) continue;

        f32 dx = s->pos.x - cam->renderPos.x;
        f32 dy = s->pos.y - cam->renderPos.y;
        s->distance = dx*dx + dy*dy;

        /* Cull sprites behind camera */
        f32 spriteAngle = atan2f(dy, dx);
        f32 camAngle = atan2f(cam->dir.y, cam->dir.x);
        f32 angleDiff = Math_AngleDifference(camAngle, spriteAngle);
        if (fabsf(angleDiff) > (MK_FOV * 0.6f * MK_DEG2RAD)) continue;

        sorted[sortCount++] = s;
    }

    if (sortCount == 0) return;

    /* Depth sort: far to near */
    qsort(sorted, sortCount, sizeof(Sprite*), Sprite_CompareDepth);

    /* Draw each sprite */
    for (u32 i = 0; i < sortCount; i++) {
        Sprite* s = sorted[i];

        f32 dx = s->pos.x - cam->renderPos.x;
        f32 dy = s->pos.y - cam->renderPos.y;
        f32 dist = sqrtf(s->distance);

        /* Transform sprite to camera space */
        f32 invDet = 1.0f / (cam->plane.x * cam->dir.y - cam->dir.x * cam->plane.y);
        f32 transformX = invDet * (cam->dir.y * dx - cam->dir.x * dy);
        f32 transformY = invDet * (-cam->plane.y * dx + cam->plane.x * dy);

        if (transformY <= 0.01f) continue;

        s32 spriteScreenX = (s32)((MK_SCREEN_WIDTH / 2.0f) * (1.0f + transformX / transformY));

        /* Sprite dimensions */
        s32 spriteHeight = (s32)(MK_SCREEN_HEIGHT / transformY * s->scale);
        s32 spriteWidth = spriteHeight;

        s32 drawStartY = -spriteHeight / 2 + MK_SCREEN_HEIGHT / 2 + (s32)(cam->pitch * MK_SCREEN_HEIGHT);
        if (drawStartY < 0) drawStartY = 0;
        s32 drawEndY = spriteHeight / 2 + MK_SCREEN_HEIGHT / 2 + (s32)(cam->pitch * MK_SCREEN_HEIGHT);
        if (drawEndY >= MK_SCREEN_HEIGHT) drawEndY = MK_SCREEN_HEIGHT - 1;

        s32 drawStartX = -spriteWidth / 2 + spriteScreenX;
        if (drawStartX < 0) drawStartX = 0;
        s32 drawEndX = spriteWidth / 2 + spriteScreenX;
        if (drawEndX >= MK_SCREEN_WIDTH) drawEndX = MK_SCREEN_WIDTH - 1;

        f32 fog = Raycaster_FogFactor(dist);

        /* Get atlas frame */
        const SpriteAtlas* atlas = SpriteAtlas_GetById(s->atlasId);
        if (!atlas || !atlas->loaded) {
            /* Fallback: draw colored rectangle when no texture loaded */
            u8 shade = (u8)(255 * (1.0f - fog));
            Color fallback = {
                (u8)(s->tint.r * shade / 255),
                (u8)(s->tint.g * shade / 255),
                (u8)(s->tint.b * shade / 255),
                s->tint.a
            };
            DrawRectangle(drawStartX, drawStartY, drawEndX - drawStartX + 1, drawEndY - drawStartY + 1, fallback);
            continue;
        }

        s32 frame = SpriteAtlas_GetFrameIndex(atlas, s->animId, s->animTime);

        /* Draw sprite column by column with z-buffer clipping */
        for (s32 stripe = drawStartX; stripe < drawEndX; stripe++) {
            if (stripe < 0 || stripe >= MK_SCREEN_WIDTH) continue;

            /* Z-buffer clip: don't draw if wall is closer */
            if (transformY >= zbuf->columns[stripe].distance) continue;

            /* Texture coordinate */
            s32 texX = (s32)(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * atlas->frameWidth / spriteWidth) / 256;
            if (texX < 0) texX = 0;
            if (texX >= atlas->frameWidth) texX = atlas->frameWidth - 1;

            s32 texYStart = 0;
            s32 texYEnd = atlas->frameHeight;

            Rectangle src = {
                (f32)(texX + frame * atlas->frameWidth),
                (f32)texYStart,
                1.0f,
                (f32)(texYEnd - texYStart)
            };
            Rectangle dst = {
                (f32)stripe,
                (f32)drawStartY,
                1.0f,
                (f32)(drawEndY - drawStartY + 1)
            };

            u8 shade = (u8)(255 * (1.0f - fog));
            Color tint = {
                (u8)(s->tint.r * shade / 255),
                (u8)(s->tint.g * shade / 255),
                (u8)(s->tint.b * shade / 255),
                s->tint.a
            };

            DrawTexturePro(atlas->tex, src, dst, (Vector2){0,0}, 0.0f, tint);
        }
    }
}
