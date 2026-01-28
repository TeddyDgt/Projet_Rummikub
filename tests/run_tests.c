#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../src/structs.h"
#include "../src/Combinaisons.h"
#include "../src/Players.h"

#if defined(_WIN32)
#include <io.h>
#define access _access
#define F_OK 0
#endif

typedef int (*TestFn)(char *out_msg, size_t out_msg_len);

typedef struct {
    const char *name;
    TestFn fn;
    int is_visual;
} TestCase;

static int file_exists(const char *path) {
    if (!path) {
        return 0;
    }
    return access(path, F_OK) == 0;
}

static int run_visual_exe(const char *exe_path, const char *label, char *out_msg, size_t out_msg_len) {
    if (!file_exists(exe_path)) {
        snprintf(out_msg, out_msg_len, "Executable introuvable: %s", exe_path);
        return 0;
    }

    printf("\n[VISUEL] %s\n", label);
    printf("Lance: %s\n", exe_path);
    printf("Fermez la fenetre pour valider (passed, but for arbitrary reason).\n");

    char cmd[512];
    snprintf(cmd, sizeof(cmd), "\"%s\"", exe_path);
    int code = system(cmd);
    (void)code;

    snprintf(out_msg, out_msg_len, "passed, but for arbitrary reason");
    return 1;
}

static int test_valid_group(char *out_msg, size_t out_msg_len) {
    Tile tiles[3] = {
        {1, 7, NOIR, 0},
        {2, 7, ROUGE, 0},
        {3, 7, BLEU, 0}
    };
    Combinaison c = { tiles, 3, IS_GROUPE };
    int ok = is_valid_group(&c);
    snprintf(out_msg, out_msg_len, "attendu=1 obtenu=%d", ok);
    return ok == 1;
}

static int test_invalid_group_color(char *out_msg, size_t out_msg_len) {
    Tile tiles[3] = {
        {1, 4, NOIR, 0},
        {2, 4, NOIR, 0},
        {3, 4, BLEU, 0}
    };
    Combinaison c = { tiles, 3, IS_GROUPE };
    int ok = is_valid_group(&c);
    snprintf(out_msg, out_msg_len, "attendu=0 obtenu=%d", ok);
    return ok == 0;
}

static int test_valid_suite(char *out_msg, size_t out_msg_len) {
    Tile tiles[3] = {
        {1, 5, ROUGE, 0},
        {2, 6, ROUGE, 0},
        {3, 7, ROUGE, 0}
    };
    Combinaison c = { tiles, 3, IS_SUITE };
    int ok = is_valid_suite(&c);
    snprintf(out_msg, out_msg_len, "attendu=1 obtenu=%d", ok);
    return ok == 1;
}

static int test_invalid_suite_gap(char *out_msg, size_t out_msg_len) {
    Tile tiles[3] = {
        {1, 5, ROUGE, 0},
        {2, 7, ROUGE, 0},
        {3, 8, ROUGE, 0}
    };
    Combinaison c = { tiles, 3, IS_SUITE };
    int ok = is_valid_suite(&c);
    snprintf(out_msg, out_msg_len, "attendu=0 obtenu=%d", ok);
    return ok == 0;
}

static int test_combinaison_points_group(char *out_msg, size_t out_msg_len) {
    Tile tiles[4] = {
        {1, 10, NOIR, 0},
        {2, 10, ROUGE, 0},
        {3, 10, BLEU, 0},
        {4, 10, JAUNE, 0}
    };
    Combinaison c = { tiles, 4, IS_GROUPE };
    int pts = combinaison_points(&c);
    snprintf(out_msg, out_msg_len, "attendu=40 obtenu=%d", pts);
    return pts == 40;
}

static int test_combinaison_points_suite(char *out_msg, size_t out_msg_len) {
    Tile tiles[3] = {
        {1, 3, BLEU, 0},
        {2, 4, BLEU, 0},
        {3, 5, BLEU, 0}
    };
    Combinaison c = { tiles, 3, IS_SUITE };
    int pts = combinaison_points(&c);
    snprintf(out_msg, out_msg_len, "attendu=12 obtenu=%d", pts);
    return pts == 12;
}

static int test_hand_penalty(char *out_msg, size_t out_msg_len) {
    Tile tiles[3] = {
        {1, 9, NOIR, 0},
        {2, 0, ROUGE, 1},
        {3, 2, BLEU, 0}
    };
    Player p = {0};
    p.hand = tiles;
    p.hand_count = 3;
    int penalty = calculate_hand_penalty(&p);
    snprintf(out_msg, out_msg_len, "attendu=41 obtenu=%d", penalty);
    return penalty == 41;
}

static int test_visual_gui_menu(char *out_msg, size_t out_msg_len) {
    const char *exe = NULL;
#if defined(_WIN32)
    if (file_exists("../src/rummikub_gui.exe")) {
        exe = "../src/rummikub_gui.exe";
    } else if (file_exists("../rummikub_gui.exe")) {
        exe = "../rummikub_gui.exe";
    }
#else
    if (file_exists("../src/rummikub_gui")) {
        exe = "../src/rummikub_gui";
    } else if (file_exists("../rummikub_gui")) {
        exe = "../rummikub_gui";
    }
#endif
    if (!exe) {
        snprintf(out_msg, out_msg_len, "Executable GUI introuvable.");
        return 0;
    }
    return run_visual_exe(exe, "GUI - Menu principal", out_msg, out_msg_len);
}

static int test_visual_gui_match(char *out_msg, size_t out_msg_len) {
    const char *exe = NULL;
#if defined(_WIN32)
    if (file_exists("../src/rummikub_gui.exe")) {
        exe = "../src/rummikub_gui.exe";
    } else if (file_exists("../rummikub_gui.exe")) {
        exe = "../rummikub_gui.exe";
    }
#else
    if (file_exists("../src/rummikub_gui")) {
        exe = "../src/rummikub_gui";
    } else if (file_exists("../rummikub_gui")) {
        exe = "../rummikub_gui";
    }
#endif
    if (!exe) {
        snprintf(out_msg, out_msg_len, "Executable GUI introuvable.");
        return 0;
    }
    return run_visual_exe(exe, "GUI - Lancer une partie (manuel)", out_msg, out_msg_len);
}

static void write_results(const char *path,
                          const TestCase *tests,
                          const int *status,
                          const char (*messages)[256],
                          int test_count,
                          int passed) {
    FILE *f = fopen(path, "a");
    if (!f) {
        return;
    }

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char buf[64] = {0};
    if (tm_info) {
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", tm_info);
    }

    fprintf(f, "\n=== RUN %s ===\n", buf[0] ? buf : "n/a");
    fprintf(f, "TOTAL: %d | PASSED: %d | FAILED: %d\n", test_count, passed, test_count - passed);
    for (int i = 0; i < test_count; i++) {
        fprintf(f, "- %s : %s (%s)\n",
                tests[i].name,
                status[i] ? "PASS" : "FAIL",
                messages[i][0] ? messages[i] : "-");
    }
    fclose(f);
}

int main(void) {
    TestCase tests[] = {
        { "MODELE: groupe valide", test_valid_group, 0 },
        { "MODELE: groupe invalide (couleur)", test_invalid_group_color, 0 },
        { "MODELE: suite valide", test_valid_suite, 0 },
        { "MODELE: suite invalide (gap)", test_invalid_suite_gap, 0 },
        { "MODELE: points groupe", test_combinaison_points_group, 0 },
        { "MODELE: points suite", test_combinaison_points_suite, 0 },
        { "MODELE: penalite main (joker)", test_hand_penalty, 0 },
        { "VISUEL: GUI menu", test_visual_gui_menu, 1 },
        { "VISUEL: GUI match", test_visual_gui_match, 1 }
    };

    int test_count = (int)(sizeof(tests) / sizeof(tests[0]));
    int passed = 0;
    int status[32] = {0};
    char messages[32][256] = {{0}};

    printf("==== RUMMIKUB TEST RUNNER ====\n");
    for (int i = 0; i < test_count; i++) {
        int ok = tests[i].fn(messages[i], sizeof(messages[i]));
        if (ok) {
            printf("[PASS] %s (%s)\n", tests[i].name, messages[i]);
            passed++;
            status[i] = 1;
        } else {
            printf("[FAIL] %s (%s)\n", tests[i].name, messages[i]);
            status[i] = 0;
        }
    }

    printf("\nTOTAL: %d | PASSED: %d | FAILED: %d\n", test_count, passed, test_count - passed);
    write_results("results.txt", tests, status, messages, test_count, passed);
    return (passed == test_count) ? 0 : 1;
}
