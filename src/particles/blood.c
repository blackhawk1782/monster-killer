#include "particles/blood.h"
#include "particles/particles.h"
#include "utilities/math_utils.h"
#include <math.h>

void Blood_Spawn(const Vec2* pos, s32 amount) {
    if (!pos) return;
    for (s32 i = 0; i < amount; i++) {
        f32 angle = Math_RandomFloat() * MK_TWO_PI;
        f32 speed = 1.0f + Math_RandomFloat() * 3.0f;
        Vec2 vel = { cosf(angle) * speed, sinf(angle) * speed };
        u8 shade = (u8)(128 + Math_RandomFloat() * 127);
        Color32 c = { shade, 0, 0, 255 };
        Particles_Spawn(pos, 0.5f, &vel, 0.5f + Math_RandomFloat() * 1.0f, 1.5f, &c);
    }
}

void Blood_Draw(const CameraSystem* cam) {
    (void)cam;
    /* Blood is drawn through particle system */
}
