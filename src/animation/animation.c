#include "animation/animation.h"
#include <math.h>

void Animation_Init(Animation* anim, s32 start, s32 end, f32 fps, bool loop) {
    if (!anim) return;
    anim->currentFrame = start;
    anim->startFrame = start;
    anim->endFrame = end;
    anim->fps = fps;
    anim->timer = 0.0f;
    anim->playing = false;
    anim->finished = false;
    anim->loop = loop;
    anim->pingpong = false;
    anim->reverse = false;
}

void Animation_Play(Animation* anim) {
    if (!anim) return;
    anim->playing = true;
    anim->finished = false;
    anim->timer = 0.0f;
}

void Animation_Stop(Animation* anim) {
    if (!anim) return;
    anim->playing = false;
}

void Animation_Update(Animation* anim, f32 dt) {
    if (!anim || !anim->playing || anim->finished) return;

    anim->timer += dt;
    f32 frameTime = 1.0f / anim->fps;
    s32 frameCount = anim->endFrame - anim->startFrame + 1;
    if (frameCount <= 0) frameCount = 1;

    s32 totalFrames = (s32)(anim->timer / frameTime);

    if (anim->pingpong) {
        s32 cycle = totalFrames % (frameCount * 2);
        if (cycle < frameCount) {
            anim->currentFrame = anim->startFrame + cycle;
        } else {
            anim->currentFrame = anim->endFrame - (cycle - frameCount);
        }
    } else if (anim->reverse) {
        s32 frame = totalFrames % frameCount;
        anim->currentFrame = anim->endFrame - frame;
        if (!anim->loop && totalFrames >= frameCount) {
            anim->currentFrame = anim->startFrame;
            anim->finished = true;
            anim->playing = false;
        }
    } else {
        if (anim->loop) {
            anim->currentFrame = anim->startFrame + (totalFrames % frameCount);
        } else {
            anim->currentFrame = anim->startFrame + totalFrames;
            if (anim->currentFrame >= anim->endFrame) {
                anim->currentFrame = anim->endFrame;
                anim->finished = true;
                anim->playing = false;
            }
        }
    }
}

s32 Animation_GetFrame(const Animation* anim) {
    if (!anim) return 0;
    return anim->currentFrame;
}

bool Animation_IsFinished(const Animation* anim) {
    if (!anim) return true;
    return anim->finished;
}
