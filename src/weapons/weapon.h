#ifndef MK_WEAPON_H
#define MK_WEAPON_H

#include "types.h"

typedef enum {
    WEAPON_PISTOL,
    WEAPON_SHOTGUN,
    WEAPON_SMG,
    WEAPON_MACHINEGUN,
    WEAPON_MAGNUM,
    WEAPON_ROCKETLAUNCHER,
    WEAPON_COUNT
} WeaponType;

typedef struct {
    WeaponType type;
    s32 damage;
    f32 accuracy;
    f32 spread;
    s32 magazine;
    s32 ammo;
    s32 maxAmmo;
    f32 fireRate;
    f32 reloadTime;
    f32 recoil;
    f32 lastFireTime;
    f32 reloadTimer;
    bool reloading;
    s32 currentMag;
    char name[32];
} Weapon;

bool Weapon_Init(void);
void Weapon_Shutdown(void);

void Weapon_Update(f32 dt);
void Weapon_Fire(void);
void Weapon_Reload(void);
void Weapon_Switch(WeaponType type);

Weapon* Weapon_GetCurrent(void);
WeaponType Weapon_GetCurrentType(void);

#endif
