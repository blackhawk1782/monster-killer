#ifndef MK_CONFIG_H
#define MK_CONFIG_H

#include "types.h"

/* Game configuration constants */
#define CFG_PLAYER_HEALTH       100
#define CFG_PLAYER_LIVES        3
#define CFG_DEFAULT_FOV         60.0f
#define CFG_MOUSE_SENSITIVITY   1.0f
#define CFG_MASTER_VOLUME       1.0f
#define CFG_MUSIC_VOLUME        0.7f
#define CFG_SFX_VOLUME          1.0f

/* Scoring */
#define CFG_SCORE_ENEMY         100
#define CFG_SCORE_HEADSHOT      250
#define CFG_SCORE_BOSS          5000
#define CFG_SCORE_SECRET        500

/* Rank thresholds */
#define CFG_RANK_S_PLUS         100000
#define CFG_RANK_S              80000
#define CFG_RANK_A              60000
#define CFG_RANK_B              40000
#define CFG_RANK_C              20000
#define CFG_RANK_D              10000
#define CFG_RANK_E              5000

#endif
