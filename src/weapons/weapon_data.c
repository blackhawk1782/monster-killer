#include "weapons/weapon_data.h"
#include <string.h>

void WeaponData_LoadAll(Weapon* weapons) {
    if (!weapons) return;

    /* Pistol */
    weapons[WEAPON_PISTOL].type = WEAPON_PISTOL;
    weapons[WEAPON_PISTOL].damage = 15;
    weapons[WEAPON_PISTOL].accuracy = 0.95f;
    weapons[WEAPON_PISTOL].spread = 0.02f;
    weapons[WEAPON_PISTOL].magazine = 12;
    weapons[WEAPON_PISTOL].currentMag = 12;
    weapons[WEAPON_PISTOL].ammo = 120;
    weapons[WEAPON_PISTOL].maxAmmo = 120;
    weapons[WEAPON_PISTOL].fireRate = 0.25f;
    weapons[WEAPON_PISTOL].reloadTime = 1.2f;
    weapons[WEAPON_PISTOL].recoil = 0.1f;
    strncpy(weapons[WEAPON_PISTOL].name, "Pistol", sizeof(weapons[WEAPON_PISTOL].name) - 1);

    /* Shotgun */
    weapons[WEAPON_SHOTGUN].type = WEAPON_SHOTGUN;
    weapons[WEAPON_SHOTGUN].damage = 8;
    weapons[WEAPON_SHOTGUN].accuracy = 0.75f;
    weapons[WEAPON_SHOTGUN].spread = 0.12f;
    weapons[WEAPON_SHOTGUN].magazine = 8;
    weapons[WEAPON_SHOTGUN].currentMag = 8;
    weapons[WEAPON_SHOTGUN].ammo = 64;
    weapons[WEAPON_SHOTGUN].maxAmmo = 64;
    weapons[WEAPON_SHOTGUN].fireRate = 0.8f;
    weapons[WEAPON_SHOTGUN].reloadTime = 2.0f;
    weapons[WEAPON_SHOTGUN].recoil = 0.4f;
    strncpy(weapons[WEAPON_SHOTGUN].name, "Shotgun", sizeof(weapons[WEAPON_SHOTGUN].name) - 1);

    /* SMG */
    weapons[WEAPON_SMG].type = WEAPON_SMG;
    weapons[WEAPON_SMG].damage = 10;
    weapons[WEAPON_SMG].accuracy = 0.85f;
    weapons[WEAPON_SMG].spread = 0.04f;
    weapons[WEAPON_SMG].magazine = 30;
    weapons[WEAPON_SMG].currentMag = 30;
    weapons[WEAPON_SMG].ammo = 300;
    weapons[WEAPON_SMG].maxAmmo = 300;
    weapons[WEAPON_SMG].fireRate = 0.1f;
    weapons[WEAPON_SMG].reloadTime = 1.5f;
    weapons[WEAPON_SMG].recoil = 0.15f;
    strncpy(weapons[WEAPON_SMG].name, "SMG", sizeof(weapons[WEAPON_SMG].name) - 1);

    /* Machine Gun */
    weapons[WEAPON_MACHINEGUN].type = WEAPON_MACHINEGUN;
    weapons[WEAPON_MACHINEGUN].damage = 12;
    weapons[WEAPON_MACHINEGUN].accuracy = 0.80f;
    weapons[WEAPON_MACHINEGUN].spread = 0.06f;
    weapons[WEAPON_MACHINEGUN].magazine = 60;
    weapons[WEAPON_MACHINEGUN].currentMag = 60;
    weapons[WEAPON_MACHINEGUN].ammo = 600;
    weapons[WEAPON_MACHINEGUN].maxAmmo = 600;
    weapons[WEAPON_MACHINEGUN].fireRate = 0.08f;
    weapons[WEAPON_MACHINEGUN].reloadTime = 2.5f;
    weapons[WEAPON_MACHINEGUN].recoil = 0.2f;
    strncpy(weapons[WEAPON_MACHINEGUN].name, "Machine Gun", sizeof(weapons[WEAPON_MACHINEGUN].name) - 1);

    /* Magnum */
    weapons[WEAPON_MAGNUM].type = WEAPON_MAGNUM;
    weapons[WEAPON_MAGNUM].damage = 45;
    weapons[WEAPON_MAGNUM].accuracy = 0.98f;
    weapons[WEAPON_MAGNUM].spread = 0.01f;
    weapons[WEAPON_MAGNUM].magazine = 6;
    weapons[WEAPON_MAGNUM].currentMag = 6;
    weapons[WEAPON_MAGNUM].ammo = 48;
    weapons[WEAPON_MAGNUM].maxAmmo = 48;
    weapons[WEAPON_MAGNUM].fireRate = 0.5f;
    weapons[WEAPON_MAGNUM].reloadTime = 1.8f;
    weapons[WEAPON_MAGNUM].recoil = 0.5f;
    strncpy(weapons[WEAPON_MAGNUM].name, "Magnum", sizeof(weapons[WEAPON_MAGNUM].name) - 1);

    /* Rocket Launcher */
    weapons[WEAPON_ROCKETLAUNCHER].type = WEAPON_ROCKETLAUNCHER;
    weapons[WEAPON_ROCKETLAUNCHER].damage = 100;
    weapons[WEAPON_ROCKETLAUNCHER].accuracy = 0.90f;
    weapons[WEAPON_ROCKETLAUNCHER].spread = 0.00f;
    weapons[WEAPON_ROCKETLAUNCHER].magazine = 4;
    weapons[WEAPON_ROCKETLAUNCHER].currentMag = 4;
    weapons[WEAPON_ROCKETLAUNCHER].ammo = 20;
    weapons[WEAPON_ROCKETLAUNCHER].maxAmmo = 20;
    weapons[WEAPON_ROCKETLAUNCHER].fireRate = 1.0f;
    weapons[WEAPON_ROCKETLAUNCHER].reloadTime = 2.5f;
    weapons[WEAPON_ROCKETLAUNCHER].recoil = 0.6f;
    strncpy(weapons[WEAPON_ROCKETLAUNCHER].name, "Rocket Launcher", sizeof(weapons[WEAPON_ROCKETLAUNCHER].name) - 1);
}
