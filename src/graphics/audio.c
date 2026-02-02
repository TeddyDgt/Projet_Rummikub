#include "audio.h"

#include <stdio.h>

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

static ma_engine g_engine;
static bool g_ready = false;

static ma_sound g_bgm;
static bool g_bgm_loaded = false;

static ma_sound g_sfx[AUDIO_SFX_COUNT];
static bool g_sfx_loaded[AUDIO_SFX_COUNT];

static const char *g_sfx_paths[AUDIO_SFX_COUNT] = {
    "audio/click.wav",
    "audio/invalid.wav",
    "audio/draw.wav",
    "audio/play.wav",
    "audio/validate.wav",
};

static const char *g_bgm_path = "audio/bgm.wav";

/**
 * @brief Realise l'operation audio_load_sound.
 *
 * @param sound Parameter sound.
 * @param path Parameter path.
 * @param flags Parameter flags.
 * @return Result value.
 */
static bool audio_load_sound(ma_sound *sound, const char *path, ma_sound_flags flags) {
    if (!sound || !path) {
        return false;
    }
    if (ma_sound_init_from_file(&g_engine, path, flags, NULL, NULL, sound) == MA_SUCCESS) {
        return true;
    }

    char alt_path[256];
    snprintf(alt_path, sizeof(alt_path), "../%s", path);
    if (ma_sound_init_from_file(&g_engine, alt_path, flags, NULL, NULL, sound) == MA_SUCCESS) {
        return true;
    }
    return false;
}

/**
 * @brief Initialise le systeme audio.
 *
 * @return Result value.
 */
bool audio_init(void) {
    if (g_ready) {
        return true;
    }

    if (ma_engine_init(NULL, &g_engine) != MA_SUCCESS) {
        fprintf(stderr, "[Audio] Failed to init miniaudio engine.\n");
        return false;
    }

    g_ready = true;

    if (audio_load_sound(&g_bgm, g_bgm_path, MA_SOUND_FLAG_STREAM)) {
        g_bgm_loaded = true;
        ma_sound_set_looping(&g_bgm, MA_TRUE);
        ma_sound_set_volume(&g_bgm, 0.45f);
    } else {
        fprintf(stderr, "[Audio] Missing BGM: %s\n", g_bgm_path);
    }

    for (int i = 0; i < AUDIO_SFX_COUNT; i++) {
        if (audio_load_sound(&g_sfx[i], g_sfx_paths[i], 0)) {
            g_sfx_loaded[i] = true;
            ma_sound_set_volume(&g_sfx[i], 0.75f);
        } else {
            g_sfx_loaded[i] = false;
            fprintf(stderr, "[Audio] Missing SFX: %s\n", g_sfx_paths[i]);
        }
    }

    return true;
}

/**
 * @brief Arrete et nettoie le systeme audio.
 *
 * @return None.
 */
void audio_shutdown(void) {
    if (!g_ready) {
        return;
    }

    if (g_bgm_loaded) {
        ma_sound_uninit(&g_bgm);
        g_bgm_loaded = false;
    }

    for (int i = 0; i < AUDIO_SFX_COUNT; i++) {
        if (g_sfx_loaded[i]) {
            ma_sound_uninit(&g_sfx[i]);
            g_sfx_loaded[i] = false;
        }
    }

    ma_engine_uninit(&g_engine);
    g_ready = false;
}

/**
 * @brief Demarre la musique de fond si disponible.
 *
 * @return None.
 */
void audio_play_bgm(void) {
    if (!g_ready || !g_bgm_loaded) {
        return;
    }
    ma_sound_start(&g_bgm);
}

/**
 * @brief Arrete la musique de fond.
 *
 * @return None.
 */
void audio_stop_bgm(void) {
    if (!g_ready || !g_bgm_loaded) {
        return;
    }
    ma_sound_stop(&g_bgm);
}

/**
 * @brief Joue un effet sonore.
 *
 * @param sfx Parameter sfx.
 * @return None.
 */
void audio_play_sfx(AudioSfx sfx) {
    if (!g_ready) {
        return;
    }
    if (sfx < 0 || sfx >= AUDIO_SFX_COUNT) {
        return;
    }
    if (!g_sfx_loaded[sfx]) {
        return;
    }

    ma_sound_stop(&g_sfx[sfx]);
    ma_sound_seek_to_pcm_frame(&g_sfx[sfx], 0);
    ma_sound_start(&g_sfx[sfx]);
}





