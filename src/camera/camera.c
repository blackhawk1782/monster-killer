#include "camera/camera.h"
#include "camera/rail_path.h"
#include "engine/engine.h"
#include "utilities/math_utils.h"
#include <math.h>
#include <string.h>

static CameraSystem g_cam = {0};
static RailPath g_railPath = {0};

bool CameraSystem_Init(void) {
    memset(&g_cam, 0, sizeof(g_cam));
    g_cam.pos = (Vec2){ 4.0f, 4.0f };
    g_cam.renderPos = g_cam.pos;
    g_cam.dir = (Vec2){ 1.0f, 0.0f };
    g_cam.plane = (Vec2){ 0.0f, 0.66f };
    g_cam.fov = MK_FOV * MK_DEG2RAD;
    g_cam.zoom = 1.0f;

    /* Setup a simple straight horizontal rail path in corridor at y=4 */
    RailPath_Init(&g_railPath);
    RailPath_Reset(&g_railPath);

    /* Node 0: Start at left side */
    RailNode n0 = { {2.0f, 4.0f}, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, RAIL_WAIT, -1, "start" };
    RailPath_AddNode(&g_railPath, &n0);

    /* Node 1: Move to middle */
    RailNode n1 = { {7.0f, 4.0f}, 0.0f, 0.0f, 1.5f, 0.0f, 1.0f, RAIL_MOVE, -1, "mid" };
    RailPath_AddNode(&g_railPath, &n1);

    /* Node 2: Wait briefly */
    RailNode n2 = { {7.0f, 4.0f}, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, RAIL_WAIT, -1, "pause" };
    RailPath_AddNode(&g_railPath, &n2);

    /* Node 3: Move to right side */
    RailNode n3 = { {13.0f, 4.0f}, 0.0f, 0.0f, 1.5f, 0.0f, 1.0f, RAIL_MOVE, -1, "end" };
    RailPath_AddNode(&g_railPath, &n3);

    /* Node 4: Wait at end */
    RailNode n4 = { {13.0f, 4.0f}, 0.0f, 0.0f, 0.0f, 2.0f, 1.0f, RAIL_WAIT, -1, "wait" };
    RailPath_AddNode(&g_railPath, &n4);

    /* Node 5: Turn around */
    RailNode n5 = { {13.0f, 4.0f}, MK_PI, 0.0f, 1.0f, 0.0f, 1.0f, RAIL_ROTATE, -1, "turn" };
    RailPath_AddNode(&g_railPath, &n5);

    /* Node 6: Move back to left */
    RailNode n6 = { {2.0f, 4.0f}, MK_PI, 0.0f, 1.5f, 0.0f, 1.0f, RAIL_MOVE, -1, "back" };
    RailPath_AddNode(&g_railPath, &n6);

    /* Node 7: Turn back to forward */
    RailNode n7 = { {2.0f, 4.0f}, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, RAIL_ROTATE, -1, "turn_fwd" };
    RailPath_AddNode(&g_railPath, &n7);

    g_railPath.loop = true;

    return true;
}

void CameraSystem_Shutdown(void) {
    memset(&g_cam, 0, sizeof(g_cam));
}

CameraSystem* CameraSystem_Get(void) {
    return &g_cam;
}

void CameraSystem_BuildPlane(void) {
    f32 halfFov = tanf(g_cam.fov * 0.5f);
    g_cam.plane.x = -g_cam.dir.y * halfFov;
    g_cam.plane.y =  g_cam.dir.x * halfFov;
}

void CameraSystem_SetPosition(f32 x, f32 y) {
    g_cam.pos.x = x;
    g_cam.pos.y = y;
    g_cam.renderPos.x = x;
    g_cam.renderPos.y = y;
}

void CameraSystem_SetDirection(f32 angle) {
    g_cam.dir.x = cosf(angle);
    g_cam.dir.y = sinf(angle);
    CameraSystem_BuildPlane();
}

void CameraSystem_SetPitch(f32 pitch) {
    g_cam.pitch = pitch;
}

void CameraSystem_SetFOV(f32 fov) {
    g_cam.fov = fov;
    CameraSystem_BuildPlane();
}

void CameraSystem_ApplyShake(f32 intensity, f32 duration) {
    if (intensity > g_cam.shakeIntensity) {
        g_cam.shakeIntensity = intensity;
        g_cam.shakeDuration = duration;
    }
}

void CameraSystem_SetBob(f32 amount) {
    g_cam.bobAmount = amount;
}

void CameraSystem_SetZoom(f32 zoom) {
    g_cam.zoom = zoom;
    if (g_cam.zoom < 0.1f) g_cam.zoom = 0.1f;
    if (g_cam.zoom > 3.0f) g_cam.zoom = 3.0f;
}

void CameraSystem_Update(void) {
    f32 dt = Engine_GetDeltaTime();

    /* Camera bob */
    if (g_cam.bobAmount > 0.0f) {
        g_cam.bobPhase += dt * 10.0f;
        g_cam.pitch = sinf(g_cam.bobPhase) * g_cam.bobAmount;
    }

    /* Camera shake decay */
    if (g_cam.shakeDuration > 0.0f) {
        g_cam.shakeDuration -= dt;
        if (g_cam.shakeDuration <= 0.0f) {
            g_cam.shakeDuration = 0.0f;
            g_cam.shakeIntensity = 0.0f;
        }
    }

    /* Apply shake offset to render position only */
    if (g_cam.shakeIntensity > 0.0f) {
        g_cam.shakeOffsetX = (Math_RandomFloat() * 2.0f - 1.0f) * g_cam.shakeIntensity;
        g_cam.shakeOffsetY = (Math_RandomFloat() * 2.0f - 1.0f) * g_cam.shakeIntensity;
    } else {
        g_cam.shakeOffsetX = 0.0f;
        g_cam.shakeOffsetY = 0.0f;
    }

    g_cam.renderPos.x = g_cam.pos.x + g_cam.shakeOffsetX;
    g_cam.renderPos.y = g_cam.pos.y + g_cam.shakeOffsetY;

    /* Update rail path movement */
    RailPath_Update(&g_railPath, dt);
}
