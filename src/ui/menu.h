#ifndef MK_MENU_H
#define MK_MENU_H

#include "types.h"

/* Menu state machine for future expansion */
typedef enum {
    MENU_MAIN,
    MENU_OPTIONS,
    MENU_SOUND,
    MENU_CONTROLS,
    MENU_HIGHSCORES
} MenuScreen;

void Menu_Init(void);
void Menu_Update(void);
void Menu_Draw(void);

#endif
