#include "save/savegame.h"
#include <stdio.h>
#include <string.h>

bool SaveGame_Write(const char* filename, const Player* player, s32 level) {
    if (!filename || !player) return false;
    FILE* f = fopen(filename, "wb");
    if (!f) return false;

    fwrite("MKSV", 1, 4, f);
    fwrite(&level, sizeof(s32), 1, f);
    fwrite(&player->score, sizeof(s32), 1, f);
    fwrite(&player->health, sizeof(s32), 1, f);
    fwrite(&player->lives, sizeof(s32), 1, f);
    fclose(f);
    return true;
}

bool SaveGame_Read(const char* filename, Player* player, s32* level) {
    if (!filename || !player || !level) return false;
    FILE* f = fopen(filename, "rb");
    if (!f) return false;

    char magic[4];
    fread(magic, 1, 4, f);
    if (memcmp(magic, "MKSV", 4) != 0) {
        fclose(f);
        return false;
    }

    fread(level, sizeof(s32), 1, f);
    fread(&player->score, sizeof(s32), 1, f);
    fread(&player->health, sizeof(s32), 1, f);
    fread(&player->lives, sizeof(s32), 1, f);
    fclose(f);
    return true;
}
