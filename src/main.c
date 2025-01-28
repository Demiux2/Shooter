#include <GLFW/glfw3.h>
#include <stdio.h>
#include "game_engine.c"  // Aquí incluimos el archivo con las funciones del motor de juego

int main(void) {
    GameEngine engine = {0}; // Inicializamos el motor de juego

    // Inicializar el motor (ventana, OpenGL y otras configuraciones)
    if (initGameEngine(&engine) != 0) {
        return -1; // Si algo salió mal, salimos
    }

    // Ciclo principal del juego
    while (!glfwWindowShouldClose(engine.window)) {
        processInput(&engine);  // Procesar la entrada del jugador
        update(&engine);        // Actualizar la lógica del juego
        render(&engine);        // Renderizar la escena

        glfwPollEvents();       // Gestionar los eventos de la ventana
    }

    // Limpiar recursos antes de salir
    glfwDestroyWindow(engine.window);
    glfwTerminate();  // Finalizar GLFW

    return 0;
}

