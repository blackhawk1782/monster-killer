#ifndef MK_ANIMATION_H
#define MK_ANIMATION_H

#include "types.h"

typedef struct {
    s32 currentFrame;
    s32 startFrame;
    s32 endFrame;
    f32 fps;
    f32 timer;
    bool playing;
    bool finished;
    bool loop;
    bool pingpong;
    bool reverse;
} Animation;

void Animation_Init(Animation* anim, s32 start, s32 end, f32 fps, bool loop);
void Animation_Play(Animation* anim);
void Animation_Stop(Animation* anim);
void Animation_Update(Animation* anim, f32 dt);
s32  Animation_GetFrame(const Animation* anim);
bool Animation_IsFinished(const Animation* anim);

#endif
