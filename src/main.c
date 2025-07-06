#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>
#include "game_engine.c"
#include "./ge/load_map.h"
#include "main.h"

extern struct p_initial_pos pip;
extern struct flagstruct flags;

int main(int argc, char *argv[]){

    if(argv[1] == NULL){
        printf("Error: You must include a map filename and any optional flags you want.\n");
	return 1;
    }

    for(int i = 1; i < argc; i++){
        if(i == 1)
	    strcpy(pip.filename, argv[i]);

        if((strcmp(argv[i], "-f") == 0) || (strcmp(argv[i], "--fps") == 0))
            flags.fflag = 1;
	else if((strcmp(argv[i], "-d") == 0) || (strcmp(argv[i], "--debug") == 0))
            flags.dflag = 1;
    }

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
    glViewport(0, 0, framebufferWidth, framebufferHeight);

    double lastTime = glfwGetTime();
    double deltaTime = 0.0;
    int frame_counter = 0;
    double frameStartTime = lastTime;

    while (!glfwWindowShouldClose(engine.window)) {
        double currentTime = glfwGetTime();

        //Calcular deltaTime
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        frame_counter += 1;

        handleInput(&engine, deltaTime);
        draw(&engine);

        glfwSwapBuffers(engine.window);
        glfwPollEvents();

        if ((currentTime - frameStartTime >= 1.0) && flags.fflag){
            printf("%.0f fps\n", (double)(1000.0 / (1000.0 / frame_counter))); //Calcular fps (para ms/frame es 1000/frame_counter)
            frame_counter = 0;
            frameStartTime = currentTime;
        }
    }

    glfwTerminate();
    return 0;
}
