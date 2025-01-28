#include <GLFW/glfw3.h>
#include <stdio.h>
#include "game_engine.c"

int main(void) {
    GameEngine engine = {0};  // Inicializamos la estructura GameEngine
    engine.playerX = 100;
    engine.playerZ = 100;
    engine.scale = 2;

    // Inicializar la ventana y OpenGL
    if (initWindow(&engine) == NULL) {
        return -1;  // Si no se pudo inicializar la ventana
    }

    // Bucle principal del juego
    while (!glfwWindowShouldClose(engine.window)) {
        handleInput(&engine);  // Maneja la entrada del jugador

        // Dibuja el escenario
        draw(&engine);

        // Intercambia los buffers de la pantalla
        glfwSwapBuffers(engine.window);
        glfwPollEvents();  // Procesa los eventos
    }

    glfwTerminate();  // Finaliza GLFW
    return 0;
}

