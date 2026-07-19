#include "audio/sound_manager.h"
#include "audio/audio_manager.h"

void SoundManager_InitDefaults(void) {
    AudioManager_LoadSound("shoot.wav", SND_SHOOT);
    AudioManager_LoadSound("reload.wav", SND_RELOAD);
    AudioManager_LoadSound("explosion.wav", SND_EXPLOSION);
    AudioManager_LoadSound("hit.wav", SND_HIT);
    AudioManager_LoadSound("enemy_death.wav", SND_ENEMY_DEATH);
    AudioManager_LoadSound("player_pain.wav", SND_PLAYER_PAIN);
    AudioManager_LoadSound("pickup.wav", SND_PICKUP);
    AudioManager_LoadSound("door.wav", SND_DOOR);
    AudioManager_LoadSound("alert.wav", SND_ALERT);
}
