#ifndef MK_AUDIO_MANAGER_H
#define MK_AUDIO_MANAGER_H

#include "types.h"
#include <raylib.h>

typedef struct {
    Sound sounds[MK_MAX_SOUNDS];
    char names[MK_MAX_SOUNDS][64];
    u32 count;
    f32 masterVolume;
    f32 sfxVolume;
    f32 musicVolume;
} AudioManager;

bool AudioManager_Init(void);
void AudioManager_Shutdown(void);
AudioManager* AudioManager_Get(void);

/* Sound loading and playback */
s32  AudioManager_LoadSound(const char* filename, const char* name);
void AudioManager_PlaySound(const char* name);
void AudioManager_PlaySound3D(const char* name, const Vec2* pos, const Vec2* listenerPos);
void AudioManager_StopSound(const char* name);

/* Music */
void AudioManager_PlayMusic(const char* filename);
void AudioManager_StopMusic(void);
void AudioManager_SetMusicVolume(f32 vol);

/* Volume control */
void AudioManager_SetMasterVolume(f32 vol);
void AudioManager_SetSFXVolume(f32 vol);

#endif
