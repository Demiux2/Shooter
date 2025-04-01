#include <GLFW/glfw3.h>
#include <stdio.h>
#include "game_engine.c"
#include "./ge/load_map.c"

extern struct p_initial_pos pip;

int main(void) {
    GameEngine engine = {0};
    /*engine.playerX = -9;
    engine.playerY = 3;
    engine.playerZ = 0;*/
    engine.fov = 45.0f;

    if (initWindow(&engine) == NULL) {
        return -1;
    }

    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(engine.window, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight); //Establecer el tamaño del área de renderizado

    double lastTime = 0.0;
    double deltaTime = 0.0;

    open_map();

    engine.playerX = pip.pX;
    engine.playerY = pip.pY;
    engine.playerZ = pip.pZ;


    //Bucle principal
    while (!glfwWindowShouldClose(engine.window)) {

        double currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        handleInput(&engine, deltaTime);
        draw(&engine);

        glfwSwapBuffers(engine.window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

