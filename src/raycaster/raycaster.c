#include "raycaster/raycaster.h"
#include "renderer/texture_manager.h"
#include "utilities/math_utils.h"
#include "engine/engine.h"
#include <raylib.h>
#include <math.h>
#include <string.h>

static ZBuffer g_zbuf = {0};
static Color g_fogColor = {16, 16, 24, 255};
static f32 g_fogDensity = 0.015f;

bool Raycaster_Init(void) {
    memset(&g_zbuf, 0, sizeof(g_zbuf));
    return true;
}

void Raycaster_Shutdown(void) {
}

const ZBuffer* Raycaster_GetZBuffer(void) {
    return &g_zbuf;
}

f32 Raycaster_FogFactor(f32 distance) {
    f32 f = 1.0f - expf(-g_fogDensity * distance);
    if (f < 0.0f) f = 0.0f;
    if (f > 1.0f) f = 1.0f;
    return f;
}

/* Wall raycasting with DDA */
static void Raycaster_DrawWalls(const CameraSystem* cam, const Map* map) {
    f32 posX = cam->renderPos.x;
    f32 posY = cam->renderPos.y;
    f32 dirX = cam->dir.x;
    f32 dirY = cam->dir.y;
    f32 planeX = cam->plane.x;
    f32 planeY = cam->plane.y;

    for (s32 x = 0; x < MK_SCREEN_WIDTH; x++) {
        f32 cameraX = 2.0f * x / (f32)MK_SCREEN_WIDTH - 1.0f;
        f32 rayDirX = dirX + planeX * cameraX;
        f32 rayDirY = dirY + planeY * cameraX;

        s32 mapX = (s32)posX;
        s32 mapY = (s32)posY;

        f32 deltaDistX = (rayDirX == 0.0f) ? 1e30f : fabsf(1.0f / rayDirX);
        f32 deltaDistY = (rayDirY == 0.0f) ? 1e30f : fabsf(1.0f / rayDirY);

        f32 sideDistX, sideDistY;
        s32 stepX, stepY;

        if (rayDirX < 0) { stepX = -1; sideDistX = (posX - mapX) * deltaDistX; }
        else               { stepX =  1; sideDistX = (mapX + 1.0f - posX) * deltaDistX; }
        if (rayDirY < 0) { stepY = -1; sideDistY = (posY - mapY) * deltaDistY; }
        else               { stepY =  1; sideDistY = (mapY + 1.0f - posY) * deltaDistY; }

        s32 side = 0;
        bool hit = false;
        s32 texId = 0;

        for (s32 i = 0; i < MK_MAX_RAYCAST_DEPTH && !hit; i++) {
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }

            if (mapX < 0 || mapX >= map->width || mapY < 0 || mapY >= map->height) {
                hit = true;
                texId = 0;
            } else {
                u8 cell = map->grid[mapY * map->width + mapX];
                if (cell > 0) {
                    hit = true;
                    texId = cell;
                }
            }
        }

        f32 perpWallDist;
        if (side == 0) perpWallDist = sideDistX - deltaDistX;
        else           perpWallDist = sideDistY - deltaDistY;

        if (perpWallDist < 0.01f) perpWallDist = 0.01f;

        s32 lineHeight = (s32)(MK_SCREEN_HEIGHT / perpWallDist);
        s32 drawStart = -lineHeight / 2 + MK_SCREEN_HEIGHT / 2;
        if (drawStart < 0) drawStart = 0;
        s32 drawEnd = lineHeight / 2 + MK_SCREEN_HEIGHT / 2;
        if (drawEnd >= MK_SCREEN_HEIGHT) drawEnd = MK_SCREEN_HEIGHT - 1;

        /* Texture coordinate */
        f32 wallX;
        if (side == 0) wallX = posY + perpWallDist * rayDirY;
        else           wallX = posX + perpWallDist * rayDirX;
        wallX -= floorf(wallX);

        /* Store in z-buffer */
        g_zbuf.columns[x].distance = perpWallDist;
        g_zbuf.columns[x].texId = texId;
        g_zbuf.columns[x].texCoord = wallX;
        g_zbuf.columns[x].drawStart = drawStart;
        g_zbuf.columns[x].drawEnd = drawEnd;
        g_zbuf.columns[x].hit = hit;

        /* Draw wall column */
        const ManagedTexture* mt = TextureManager_GetById(texId);
        if (mt && mt->loaded) {
            s32 texX = (s32)(wallX * mt->width);
            if ((side == 0 && rayDirX > 0) || (side == 1 && rayDirY < 0))
                texX = mt->width - texX - 1;

            Rectangle src = { (f32)texX, 0.0f, 1.0f, (f32)mt->height };
            Rectangle dst = { (f32)x, (f32)drawStart, 1.0f, (f32)(drawEnd - drawStart + 1) };

            f32 fog = Raycaster_FogFactor(perpWallDist);
            Color tint = { (u8)(255 * (1.0f - fog)), (u8)(255 * (1.0f - fog)), (u8)(255 * (1.0f - fog)), 255 };
            if (side == 1) {
                tint.r = (u8)(tint.r * 0.7f);
                tint.g = (u8)(tint.g * 0.7f);
                tint.b = (u8)(tint.b * 0.7f);
            }
            DrawTexturePro(mt->tex, src, dst, (Vector2){0,0}, 0.0f, tint);
        } else {
            /* Fallback solid color with fog */
            f32 fog = Raycaster_FogFactor(perpWallDist);
            u8 c = (u8)(192 * (1.0f - fog));
            if (side == 1) c = (u8)(c * 0.7f);
            DrawRectangle(x, drawStart, 1, drawEnd - drawStart + 1, (Color){c, c, c, 255});
        }
    }
}

/* Floor casting using perspective correction */
static void Raycaster_DrawFloorCeiling(const CameraSystem* cam, const Map* map) {
    f32 posX = cam->renderPos.x;
    f32 posY = cam->renderPos.y;
    f32 dirX = cam->dir.x;
    f32 dirY = cam->dir.y;
    f32 planeX = cam->plane.x;
    f32 planeY = cam->plane.y;

    s32 horizon = MK_SCREEN_HEIGHT / 2;

    for (s32 y = horizon + 1; y < MK_SCREEN_HEIGHT; y++) {
        f32 rowDistance = (f32)MK_SCREEN_HEIGHT / (2.0f * y - MK_SCREEN_HEIGHT);
        if (rowDistance < 0) rowDistance = -rowDistance;

        f32 floorStepX = rowDistance * (dirX + planeX - (dirX - planeX)) / MK_SCREEN_WIDTH;
        f32 floorStepY = rowDistance * (dirY + planeY - (dirY - planeY)) / MK_SCREEN_WIDTH;
        f32 floorX = posX + rowDistance * (dirX - planeX);
        f32 floorY = posY + rowDistance * (dirY - planeY);

        for (s32 x = 0; x < MK_SCREEN_WIDTH; x += 2) {
            s32 cellX = (s32)floorX;
            s32 cellY = (s32)floorY;

            u8 floorTex = 0;
            u8 ceilTex = 0;
            if (cellX >= 0 && cellX < map->width && cellY >= 0 && cellY < map->height) {
                floorTex = map->floorGrid[cellY * map->width + cellX];
                ceilTex  = map->ceilGrid[cellY * map->width + cellX];
            }

            f32 fog = Raycaster_FogFactor(rowDistance);
            u8 shade = (u8)(255 * (1.0f - fog));

            /* Floor pixel */
            if (floorTex > 0) {
                const ManagedTexture* mt = TextureManager_GetById(floorTex);
                if (mt && mt->loaded) {
                    s32 texX = (s32)(fabsf(floorX - cellX) * mt->width) % mt->width;
                    s32 texY = (s32)(fabsf(floorY - cellY) * mt->height) % mt->height;
                    Rectangle src = { (f32)texX, (f32)texY, 1.0f, 1.0f };
                    Rectangle dst = { (f32)x, (f32)y, 2.0f, 1.0f };
                    DrawTexturePro(mt->tex, src, dst, (Vector2){0,0}, 0.0f,
                                   (Color){shade, shade, shade, 255});
                } else {
                    DrawRectangle(x, y, 2, 1, (Color){shade/2, shade/2, shade/2, 255});
                }
            } else {
                DrawRectangle(x, y, 2, 1, (Color){shade/3, shade/3, shade/3, 255});
            }

            /* Ceiling pixel (mirror y) */
            s32 cy = MK_SCREEN_HEIGHT - 1 - y;
            if (cy >= 0 && cy < horizon) {
                if (ceilTex > 0) {
                    const ManagedTexture* mt = TextureManager_GetById(ceilTex);
                    if (mt && mt->loaded) {
                        s32 texX = (s32)(fabsf(floorX - cellX) * mt->width) % mt->width;
                        s32 texY = (s32)(fabsf(floorY - cellY) * mt->height) % mt->height;
                        Rectangle src = { (f32)texX, (f32)texY, 1.0f, 1.0f };
                        Rectangle dst = { (f32)x, (f32)cy, 2.0f, 1.0f };
                        DrawTexturePro(mt->tex, src, dst, (Vector2){0,0}, 0.0f,
                                       (Color){shade, shade, shade, 255});
                    } else {
                        DrawRectangle(x, cy, 2, 1, (Color){shade/4, shade/4, shade/5, 255});
                    }
                } else {
                    DrawRectangle(x, cy, 2, 1, (Color){shade/5, shade/5, shade/6, 255});
                }
            }

            floorX += floorStepX;
            floorY += floorStepY;
        }
    }
}

void Raycaster_Render(const CameraSystem* cam, const Map* map) {
    if (!cam || !map) return;

    /* Clear z-buffer */
    for (s32 x = 0; x < MK_SCREEN_WIDTH; x++) {
        g_zbuf.columns[x].distance = 1e30f;
        g_zbuf.columns[x].hit = false;
    }

    /* Draw floor and ceiling first */
    Raycaster_DrawFloorCeiling(cam, map);

    /* Draw walls over floor/ceiling */
    Raycaster_DrawWalls(cam, map);
}
