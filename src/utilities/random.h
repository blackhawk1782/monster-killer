#ifndef MK_RANDOM_H
#define MK_RANDOM_H

#include "types.h"

/* PCG-style lightweight RNG */
u32 Random_NextU32(void);
f32 Random_NextFloat(void);
s32 Random_Range(s32 min, s32 max);
void Random_Seed(u64 state, u64 inc);

#endif
