#ifndef MK_SOUND_MANAGER_H
#define MK_SOUND_MANAGER_H

#include "types.h"

/* Sound event names for gameplay */
#define SND_SHOOT       "shoot"
#define SND_RELOAD      "reload"
#define SND_EXPLOSION   "explosion"
#define SND_HIT         "hit"
#define SND_ENEMY_DEATH "enemy_death"
#define SND_PLAYER_PAIN "player_pain"
#define SND_PICKUP      "pickup"
#define SND_DOOR        "door"
#define SND_ALERT       "alert"

void SoundManager_InitDefaults(void);

#endif
