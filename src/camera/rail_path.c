#include "camera/rail_path.h"
#include "camera/camera.h"
#include "map/map.h"
#include "physics/collision.h"
#include "utilities/math_utils.h"
#include <string.h>
#include <math.h>

bool RailPath_Init(RailPath* path) {
    if (!path) return false;
    memset(path, 0, sizeof(RailPath));
    return true;
}

void RailPath_Reset(RailPath* path) {
    if (!path) return;
    path->current = 0;
    path->progress = 0.0f;
    path->timer = 0.0f;
    path->active = true;
}

void RailPath_AddNode(RailPath* path, const RailNode* node) {
    if (!path || !node || path->count >= MK_MAX_RAIL_NODES) return;
    path->nodes[path->count] = *node;
    path->count++;
}

void RailPath_Update(RailPath* path, f32 dt) {
    if (!path || !path->active || path->count < 2) return;

    CameraSystem* cam = CameraSystem_Get();
    if (!cam) return;

    RailNode* current = &path->nodes[path->current];
    RailNode* next = &path->nodes[(path->current + 1) % path->count];

    switch (current->type) {
        case RAIL_WAIT:
            path->timer += dt;
            if (path->timer >= current->waitTime) {
                path->timer = 0.0f;
                path->current++;
                if (path->current >= path->count) {
                    if (path->loop) path->current = 0;
                    else path->active = false;
                }
            }
            break;

        case RAIL_MOVE:
        default: {
            f32 moveSpeed = current->speed * dt;
            f32 dx = next->position.x - cam->pos.x;
            f32 dy = next->position.y - cam->pos.y;
            f32 dist = sqrtf(dx*dx + dy*dy);

            if (dist <= moveSpeed || dist < 0.01f) {
                cam->pos = next->position;
                path->current++;
                if (path->current >= path->count) {
                    if (path->loop) path->current = 0;
                    else path->active = false;
                }
            } else {
                /* Compute desired next position */
                f32 nextX = cam->pos.x + (dx / dist) * moveSpeed;
                f32 nextY = cam->pos.y + (dy / dist) * moveSpeed;

                /* Wall collision: check if next position is inside a wall */
                const Map* map = Map_Get();
                if (map && !Collision_PointInWall(map, nextX, nextY)) {
                    cam->pos.x = nextX;
                    cam->pos.y = nextY;
                } else {
                    /* Try sliding along X only */
                    if (!Collision_PointInWall(map, nextX, cam->pos.y)) {
                        cam->pos.x = nextX;
                    }
                    /* Try sliding along Y only */
                    else if (!Collision_PointInWall(map, cam->pos.x, nextY)) {
                        cam->pos.y = nextY;
                    }
                    /* Both blocked — advance to next node to avoid getting stuck */
                    else {
                        path->current++;
                        if (path->current >= path->count) {
                            if (path->loop) path->current = 0;
                            else path->active = false;
                        }
                    }
                }

                /* Smooth rotation toward target angle */
                f32 targetAngle = next->angle;
                f32 currentAngle = atan2f(cam->dir.y, cam->dir.x);
                f32 angleDiff = Math_AngleDifference(currentAngle, targetAngle);
                f32 rotSpeed = 2.0f * dt;
                if (fabsf(angleDiff) < rotSpeed) {
                    CameraSystem_SetDirection(targetAngle);
                } else {
                    CameraSystem_SetDirection(currentAngle + (angleDiff > 0 ? rotSpeed : -rotSpeed));
                }

                /* Interpolate pitch and zoom */
                cam->pitch += (next->pitch - cam->pitch) * dt * 2.0f;
                cam->zoom += (next->zoom - cam->zoom) * dt * 2.0f;
            }
            break;
        }
    }
}

bool RailPath_IsComplete(const RailPath* path) {
    if (!path) return true;
    return !path->active;
}
