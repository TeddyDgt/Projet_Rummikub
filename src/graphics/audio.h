#ifndef AUDIO_H
#define AUDIO_H

#include <stdbool.h>

typedef enum {
    AUDIO_SFX_CLICK = 0,
    AUDIO_SFX_INVALID,
    AUDIO_SFX_DRAW,
    AUDIO_SFX_PLAY,
    AUDIO_SFX_VALIDATE,
    AUDIO_SFX_COUNT
} AudioSfx;

bool audio_init(void);
void audio_shutdown(void);

void audio_play_bgm(void);
void audio_stop_bgm(void);

void audio_play_sfx(AudioSfx sfx);

#endif
