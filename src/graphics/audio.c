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
 * @brief Charge un son depuis un fichier avec un chemin de repli (fallback).
 * Tente de charger le fichier au chemin indiqué, puis tente "../chemin" si le premier échoue.
 * @param sound Pointeur vers la structure de son miniaudio.
 * @param path Chemin vers le fichier audio.
 * @param flags Drapeaux de configuration (ex: MA_SOUND_FLAG_STREAM).
 * @return true si le son est chargé avec succès, false sinon.
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
 * @brief Initialise le moteur audio et charge toutes les ressources sonores.
 * Configure la musique de fond en boucle (looping) et charge les différents SFX (click, invalid, draw, etc.).
 * @return true si l'initialisation du moteur a réussi, false sinon.
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
 * @brief Arrête le moteur audio et libère toutes les ressources chargées.
 * Désinitialise la musique et chaque effet sonore avant d'éteindre le moteur miniaudio.
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
 * @brief Lance la lecture de la musique de fond (BGM).
 * La musique est configurée pour boucler automatiquement si elle a été chargée correctement.
 */
void audio_play_bgm(void) {
    if (!g_ready || !g_bgm_loaded) {
        return;
    }
    ma_sound_start(&g_bgm);
}

/**
 * @brief Arrête la lecture de la musique de fond.
 */
void audio_stop_bgm(void) {
    if (!g_ready || !g_bgm_loaded) {
        return;
    }
    ma_sound_stop(&g_bgm);
}

/**
 * @brief Joue un effet sonore spécifique (SFX).
 * Si le son est déjà en cours de lecture, il est stoppé et redémarré depuis le début.
 * @param sfx L'identifiant de l'effet sonore à jouer (indexé sur AudioSfx).
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
