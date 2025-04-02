#include <GLFW/glfw3.h>
#include <stdio.h>
#include "game_engine.c"
#include "./ge/load_map.h"

extern struct p_initial_pos pip;

int main(void){

    open_map();

    GameEngine engine = {0};
    engine.fov = 45.0f;
    engine.playerX = pip.pX;
    engine.playerY = pip.pY;
    engine.playerZ = pip.pZ;
    engine.angle = pip.pA;

    if (initWindow(&engine) == NULL){
        return -1;
    }

    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(engine.window, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight); //Establecer el tamaño del área de renderizado

    double lastTime = glfwGetTime(); // Inicializar lastTime al tiempo actual
    double deltaTime = 0.0;
    double frame_counter = 0;
    double frameStartTime = lastTime; // Variable para controlar el paso de 1 segundo

    // Bucle principal
    while (!glfwWindowShouldClose(engine.window)) {
        double currentTime = glfwGetTime();

        // Calcular deltaTime (el tiempo que pasó desde el último cuadro)
        deltaTime = currentTime - lastTime;
        lastTime = currentTime; // Actualizar lastTime con el tiempo actual

        frame_counter += 1.0; // Incrementar el contador de cuadros

        handleInput(&engine, deltaTime); // Pasar deltaTime para el manejo de la entrada
        draw(&engine); // Dibujar la escena

        glfwSwapBuffers(engine.window);
        glfwPollEvents();

        // Imprimir los FPS cada segundo
        if (currentTime - frameStartTime >= 1.0) {
            printf("%.0f fps\n", (double)(1000.0 / (1000.0 / frame_counter))); // Calcular fps (para ms/frame es 1000/frame_counter)
            frame_counter = 0; // Reiniciar el contador de cuadros
            frameStartTime = currentTime; // Actualizar el tiempo para el siguiente segundo
        }
    }

    glfwTerminate();
    return 0;
}
