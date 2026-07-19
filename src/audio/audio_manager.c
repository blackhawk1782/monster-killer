#include "audio/audio_manager.h"
#include "audio/sound_manager.h"
#include <stdio.h>
#include <string.h>

static AudioManager g_audio = {0};
static Music g_music = {0};
static bool g_musicPlaying = false;

bool AudioManager_Init(void) {
    memset(&g_audio, 0, sizeof(g_audio));
    InitAudioDevice();
    if (!IsAudioDeviceReady()) {
        fprintf(stderr, "Audio device not ready\n");
        return false;
    }
    g_audio.masterVolume = 1.0f;
    g_audio.sfxVolume = 1.0f;
    g_audio.musicVolume = 1.0f;
    return true;
}

void AudioManager_Shutdown(void) {
    if (g_musicPlaying) {
        StopMusicStream(g_music);
        UnloadMusicStream(g_music);
        g_musicPlaying = false;
    }
    for (u32 i = 0; i < g_audio.count; i++) {
        UnloadSound(g_audio.sounds[i]);
    }
    memset(&g_audio, 0, sizeof(g_audio));
    CloseAudioDevice();
}

AudioManager* AudioManager_Get(void) {
    return &g_audio;
}

s32 AudioManager_LoadSound(const char* filename, const char* name) {
    if (g_audio.count >= MK_MAX_SOUNDS) return -1;

    char path[256];
    snprintf(path, sizeof(path), "assets/audio/%s", filename);

    Sound snd = LoadSound(path);
    if (snd.frameCount == 0) {
        fprintf(stderr, "Failed to load sound: %s\n", path);
        return -1;
    }

    s32 id = (s32)g_audio.count;
    g_audio.sounds[id] = snd;
    strncpy(g_audio.names[id], name, sizeof(g_audio.names[id]) - 1);
    g_audio.names[id][sizeof(g_audio.names[id]) - 1] = '\0';
    g_audio.count++;
    return id;
}

void AudioManager_PlaySound(const char* name) {
    for (u32 i = 0; i < g_audio.count; i++) {
        if (strcmp(g_audio.names[i], name) == 0) {
            SetSoundVolume(g_audio.sounds[i], g_audio.masterVolume * g_audio.sfxVolume);
            PlaySound(g_audio.sounds[i]);
            return;
        }
    }
}

void AudioManager_PlaySound3D(const char* name, const Vec2* pos, const Vec2* listenerPos) {
    if (!pos || !listenerPos) return;
    f32 dx = pos->x - listenerPos->x;
    f32 dy = pos->y - listenerPos->y;
    f32 dist = dx*dx + dy*dy;
    f32 vol = 1.0f / (1.0f + dist * 0.1f);
    if (vol > 1.0f) vol = 1.0f;

    for (u32 i = 0; i < g_audio.count; i++) {
        if (strcmp(g_audio.names[i], name) == 0) {
            SetSoundVolume(g_audio.sounds[i], vol * g_audio.masterVolume * g_audio.sfxVolume);
            PlaySound(g_audio.sounds[i]);
            return;
        }
    }
}

void AudioManager_StopSound(const char* name) {
    for (u32 i = 0; i < g_audio.count; i++) {
        if (strcmp(g_audio.names[i], name) == 0) {
            StopSound(g_audio.sounds[i]);
            return;
        }
    }
}

void AudioManager_PlayMusic(const char* filename) {
    if (g_musicPlaying) {
        StopMusicStream(g_music);
        UnloadMusicStream(g_music);
        g_musicPlaying = false;
    }

    char path[256];
    snprintf(path, sizeof(path), "assets/music/%s", filename);

    g_music = LoadMusicStream(path);
    if (g_music.frameCount > 0) {
        SetMusicVolume(g_music, g_audio.masterVolume * g_audio.musicVolume);
        PlayMusicStream(g_music);
        g_musicPlaying = true;
    }
}

void AudioManager_StopMusic(void) {
    if (g_musicPlaying) {
        StopMusicStream(g_music);
    }
}

void AudioManager_SetMusicVolume(f32 vol) {
    g_audio.musicVolume = vol;
    if (g_musicPlaying) {
        SetMusicVolume(g_music, g_audio.masterVolume * g_audio.musicVolume);
    }
}

void AudioManager_SetMasterVolume(f32 vol) {
    g_audio.masterVolume = vol;
}

void AudioManager_SetSFXVolume(f32 vol) {
    g_audio.sfxVolume = vol;
}
