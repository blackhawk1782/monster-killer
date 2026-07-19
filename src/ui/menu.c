#include "ui/menu.h"
#include "engine/engine.h"
#include <raylib.h>

static MenuScreen g_menuScreen = MENU_MAIN;

void Menu_Init(void) {
    g_menuScreen = MENU_MAIN;
}

void Menu_Update(void) {
    if (IsKeyPressed(KEY_ESCAPE)) {
        if (g_menuScreen != MENU_MAIN) g_menuScreen = MENU_MAIN;
        else Engine_SetState(GS_TITLE);
    }
}

void Menu_Draw(void) {
    ClearBackground(BLACK);
    switch (g_menuScreen) {
        case MENU_MAIN:
            DrawText("MAIN MENU", 80, 40, 20, WHITE);
            DrawText("START", 100, 80, 10, YELLOW);
            DrawText("OPTIONS", 100, 100, 10, GRAY);
            DrawText("QUIT", 100, 120, 10, GRAY);
            break;
        default:
            break;
    }
}
