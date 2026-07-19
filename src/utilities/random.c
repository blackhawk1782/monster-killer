#include "utilities/random.h"

static u64 g_state = 0x853c49e6748fea9bULL;
static u64 g_inc = 0xda3e39cb94b95bdbULL;

static u32 pcg32(void) {
    u64 oldstate = g_state;
    g_state = oldstate * 6364136223846793005ULL + g_inc;
    u32 xorshifted = (u32)(((oldstate >> 18u) ^ oldstate) >> 27u);
    u32 rot = (u32)(oldstate >> 59u);
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

void Random_Seed(u64 state, u64 inc) {
    g_state = 0U;
    g_inc = (inc << 1u) | 1u;
    pcg32();
    g_state += state;
    pcg32();
}

u32 Random_NextU32(void) {
    return pcg32();
}

f32 Random_NextFloat(void) {
    return (f32)(pcg32() & 0x7fffffff) / (f32)0x7fffffff;
}

s32 Random_Range(s32 min, s32 max) {
    if (min >= max) return min;
    u32 range = (u32)(max - min + 1);
    return min + (s32)(pcg32() % range);
}
