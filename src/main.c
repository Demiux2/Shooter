#include <GLFW/glfw3.h>
#include <stdio.h>
#include "game_engine.c"
#include "./ge/load_map.c"

int main(void) {
    GameEngine engine = {0};  // Inicializamos la estructura GameEngine
    engine.playerX = -9;
    engine.playerY = 0;
    engine.playerZ = 0;
    engine.scale = 2;
    engine.fov = 45;

    // Inicializar la ventana y OpenGL
    if (initWindow(&engine) == NULL) {
        return -1;  // Si no se pudo inicializar la ventana
    }

    // Al inicio del programa, antes del bucle principal
    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(engine.window, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);  // Establecer el tamaño del área de renderizado

    double lastTime = 0.0;
    double deltaTime = 0.0;

    open_map();

    // Bucle principal del juego
    while (!glfwWindowShouldClose(engine.window)) {

        double currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        handleInput(&engine, deltaTime);  // Maneja la entrada del jugador

        // Dibuja el escenario
        draw(&engine);

        // Intercambia los buffers de la pantalla
        glfwSwapBuffers(engine.window);
        glfwPollEvents();  // Procesa los eventos
    }

    glfwTerminate();  // Finaliza GLFW
    return 0;
}

