#include "gui_platform.h"

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @struct GuiWindow
 * @brief Structure interne représentant une fenêtre graphique.
 *
 * Cette structure encapsule un pointeur GLFWwindow afin de masquer
 * l’implémentation GLFW au reste du programme.
 * Elle permet d’assurer une séparation claire entre la plateforme
 * graphique et le reste de l’application.
 */
struct GuiWindow {
    GLFWwindow* handle;
};

/**
 * @brief Callback d’erreur GLFW.
 *
 * Cette fonction est appelée automatiquement par GLFW lorsqu’une erreur
 * survient. Elle permet d’afficher un message explicite sur la sortie
 * d’erreur standard afin de faciliter le débogage.
 *
 * @param error Code d’erreur GLFW (non utilisé ici).
 * @param description Description textuelle de l’erreur.
 */
static void error_callback(int error, const char* description) {
    (void)error;
    fprintf(stderr, "[GLFW] %s\n", description);
}

/**
 * @brief Crée et initialise une fenêtre graphique OpenGL.
 *
 * Cette fonction :
 * - initialise la bibliothèque GLFW,
 * - crée une fenêtre avec un contexte OpenGL,
 * - rend ce contexte courant,
 * - active la synchronisation verticale (VSync),
 * - alloue et retourne une structure GuiWindow.
 *
 * En cas d’erreur (initialisation, création de fenêtre ou allocation),
 * la fonction nettoie les ressources et retourne NULL.
 *
 * @param width Largeur de la fenêtre en pixels.
 * @param height Hauteur de la fenêtre en pixels.
 * @param title Titre de la fenêtre.
 * @return Un pointeur vers GuiWindow si succès, NULL sinon.
 */
GuiWindow* gui_create_window(int width, int height, const char* title) {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        fprintf(stderr, "Failed to init GLFW\n");
        return NULL;
    }

    /* Contexte OpenGL volontairement conservateur :p
       afin d’assurer une compatibilité maximale */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWwindow* win = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!win) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(win);
    glfwSwapInterval(1); // Ici on active la VSync (1=activée, 0=désactivée)

    GuiWindow* w = (GuiWindow*)malloc(sizeof(GuiWindow));
    if (!w) {
        glfwDestroyWindow(win);
        glfwTerminate();
        return NULL;
    }
    w->handle = win;
    return w;
}

/**
 * @brief Détruit une fenêtre graphique et libère les ressources associées.
 *
 * Cette fonction :
 * - détruit la fenêtre GLFW,
 * - libère la structure GuiWindow,
 * - termine proprement la bibliothèque GLFW.
 *
 * @param w Pointeur vers la fenêtre à détruire.
 */
void gui_destroy_window(GuiWindow* w) {
    if (!w) return;
    if (w->handle) glfwDestroyWindow(w->handle);
    free(w);
    glfwTerminate();
}

/**
 * @brief Indique si la fenêtre doit être fermée.
 *
 * Cette fonction permet de savoir si l’utilisateur a demandé
 * la fermeture de la fenêtre (clic sur la croix, Alt+F4...... ).
 *
 * @param w Pointeur vers la fenêtre.
 * @return true si la fenêtre doit se fermer, false sinon.
 */
bool gui_should_close(GuiWindow* w) {
    return w && w->handle && glfwWindowShouldClose(w->handle);
}

/**
 * @brief Traite les événements de la fenêtre.
 *
 * Cette fonction doit être appelée à chaque itération de la boucle
 * principale afin de traiter les événements clavier, souris et système c'est plus clairement un io update ici on établit l'etat actuel des controles utilisateur.
 */
void gui_poll_events(void) {
    glfwPollEvents();
}

/**
 * @brief Échange les buffers d’affichage.
 *
 * Cette fonction affiche le contenu rendu dans le buffer arrière
 * et prépare le prochain cycle de rendu.
 *
 * @param w Pointeur vers la fenêtre.
 */
void gui_swap_buffers(GuiWindow* w) {
    if (!w || !w->handle) return;
    glfwSwapBuffers(w->handle);
}


/**
 * @brief Récupère la taille réelle du framebuffer.
 *
 * La taille du framebuffer peut différer de la taille logique
 * de la fenêtre (notamment sur les écrans haut de gamme genre les 1080p, 2K, 4K..... Enfin plus l'image est grande plus le frame buffer sera imposant).
 *
 * @param w Pointeur vers la fenêtre.
 * @param out_w Pointeur vers la largeur retournée (peut être NULL).
 * @param out_h Pointeur vers la hauteur retournée (peut être NULL).
 */
void gui_get_framebuffer_size(GuiWindow* w, int* out_w, int* out_h) {
    if (!w || !w->handle) {
        if (out_w) *out_w = 0;
        if (out_h) *out_h = 0;
        return;
    }
    int fw = 0, fh = 0;
    glfwGetFramebufferSize(w->handle, &fw, &fh);
    if (out_w) *out_w = fw;
    if (out_h) *out_h = fh;
}

void gui_get_mouse_pos(GuiWindow* w, double* out_x, double* out_y) {
    if (!w || !w->handle) {
        if (out_x) *out_x = 0.0;
        if (out_y) *out_y = 0.0;
        return;
    }
    glfwGetCursorPos(w->handle, out_x, out_y);
}

bool gui_mouse_button_down(GuiWindow* w, int button) {
    if (!w || !w->handle) return false;
    return glfwGetMouseButton(w->handle, button) == GLFW_PRESS;
}

bool gui_key_down(GuiWindow* w, int key) {
    if (!w || !w->handle) return false;
    return glfwGetKey(w->handle, key) == GLFW_PRESS;
}

double gui_get_time_seconds(void) {
    return glfwGetTime();
}
