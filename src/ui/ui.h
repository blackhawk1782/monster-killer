#ifndef MK_UI_H
#define MK_UI_H

#include "types.h"

/* Forward declaration to avoid circular include */
struct CameraSystem;

bool UI_Init(void);
void UI_Shutdown(void);

/* Screens */
void UI_UpdateTitle(void);
void UI_DrawTitle(void);
void UI_UpdateMenu(void);
void UI_DrawMenu(void);
void UI_DrawHUD(void);
void UI_DrawPause(void);
void UI_UpdateGameOver(void);
void UI_DrawGameOver(void);
void UI_UpdateMissionComplete(void);
void UI_DrawMissionComplete(void);

/* HUD elements */
void UI_DrawCrosshair(void);
void UI_DrawLaserSight(const struct CameraSystem* cam);
void UI_DrawHealth(s32 health, s32 maxHealth);
void UI_DrawAmmo(s32 current, s32 total);
void UI_DrawScore(s32 score);
void UI_DrawCombo(s32 combo);
void UI_DrawWeaponName(const char* name);
void UI_DrawAccuracy(f32 accuracy);
void UI_DrawBossHealth(const char* name, s32 health, s32 maxHealth);

#endif
