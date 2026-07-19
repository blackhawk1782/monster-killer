#ifndef MK_HUD_H
#define MK_HUD_H

#include "types.h"

/* Additional HUD helpers */
void HUD_DrawRank(const char* rank);
void HUD_DrawTimer(f32 time);
void HUD_DrawAccuracy(f32 accuracy);
void HUD_DrawLives(s32 lives);

#endif
