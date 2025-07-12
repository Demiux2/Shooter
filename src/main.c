#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>
#include "game_engine.c"
#include "./ge/load_map.h"
#include "main.h"

extern struct p_initial_pos pip;
extern struct flagstruct flags;
extern struct player_config player_conf;

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

    GameEngine engine = {0};

    FILE* config_file = fopen("./config.txt", "r");
    char line[64] = {0};
    int i = 0;
    if(config_file == NULL){
        printf("Configuration file \"config.txt\" not found, using default values instead.\n");
        engine.fov = 45.0f;
    }
    else{
        while(fgets(line, sizeof(line), config_file) != NULL){
            i++;
            line[strcspn(line, "\n")] = 0;
            if(i == 1){
                fgets(line, sizeof(line), config_file);
                strcpy(player_conf.player_name, line);
            }
            else if(i == 2){
                if(sscanf(line, "%s", player_conf.language_id) != 1){
                    printf("Incorrect amount of parameters or wrong format in line %d, using default language instead.\n", i+1);
		    strcpy(player_conf.language_id, "en");
                }
            }
            else if(i == 3){
                if(sscanf(line, "%lf", &engine.fov) != 1){
                    printf("Incorrect amount of parameters or wrong format in line %d, using default FOV instead.\n", i+1);
                    engine.fov = 45.0f;
                }
            }
            else if(i == 4){
                if(sscanf(line, "%d %d", &player_conf.res_x, &player_conf.res_y) != 2){
                    printf("Incorrect amount of parameters or wrong format in line %d, using default resolution instead.\n", i+1);
                    player_conf.res_x = 1366;
                    player_conf.res_y = 768;
                }
	    }
	    else if(i == 5){
                if(sscanf(line, "%d", &player_conf.fullscreen) != 1){
                    printf("Incorrect amount of parameteres or wrong format in line %d, using default fullscreen mode instead.\n", i+1);
                    player_conf.fullscreen = 1;
                }
	    }
        }
        fclose(config_file);
    }

    printf("FOV: %lf\n", engine.fov);

    open_map();

    engine.playerX = pip.pX;
    engine.playerY = pip.pY;
    engine.playerZ = pip.pZ;
    engine.angle = pip.pA;

    if(initWindow(&engine) == NULL){
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
