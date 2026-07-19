#ifndef MK_SAVEGAME_H
#define MK_SAVEGAME_H

#include "types.h"
#include "player/player.h"

bool SaveGame_Write(const char* filename, const Player* player, s32 level);
bool SaveGame_Read(const char* filename, Player* player, s32* level);

#endif
