#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>
#include "./ge/load_map.h"
#include "./ge/load_textures.h"
#include "main.h"

struct p_initial_pos pip;
struct flagstruct flags;
struct player_config player_conf;
struct GameEngine engine;
extern unsigned int textures[];
extern unsigned int textureID;

int main(int argc, char *argv[]){

    if(argv[1] == NULL){
        printf("Error: You must include a map filename and any optional flags you want.\n");
        return 1;
    }

    for(int i = 1; i < argc; i++){
        if(i == 1) strcpy(pip.filename, argv[i]);

	if((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0)){
            printf("Use: <mapfilename> [-h | --help] [-f | --fps] [-d | --debug]\n");
	    return 0;
        }
	else if((strcmp(argv[i], "-f") == 0) || (strcmp(argv[i], "--fps") == 0)) flags.fflag = 1;
        else if((strcmp(argv[i], "-d") == 0) || (strcmp(argv[i], "--debug") == 0)) flags.dflag = 1;
    }

    memset(&engine, 0, sizeof(engine));

    FILE* config_file = fopen("./config.txt", "r"); //Se lee el archivo de configuración
    char line[64] = {0};
    if(config_file == NULL){
        printf("Configuration file \"config.txt\" not found, using default values instead.\n");
        strcpy(player_conf.player_name, "Username");
        strcpy(player_conf.language_id, "en_us");
        engine.fov = 45.0f;
        player_conf.res_x = 1366;
        player_conf.res_y = 768;
        player_conf.fullscreen = 1;
    }
    else{ //Se guardan las configuraciones del jugador
        for(int i = 1; i <= 5 && fgets(line, sizeof(line), config_file) != NULL; i++){
            line[strcspn(line, "\n")] = 0;
            switch(i){
                case 1:
                    if(strcpy(player_conf.player_name, line) == NULL){
                        printf("Could not read line %d, using default player name.\n", i);
                        strcpy(player_conf.player_name, "Username");
                    }
                    break;
                case 2:
                    if(sscanf(line, "%s", player_conf.language_id) != 1){
                        printf("Incorrect format in line %d, using default language.\n", i);
                        strcpy(player_conf.language_id, "en");
                    }
                    break;
                case 3:
                    if(sscanf(line, "%lf", &engine.fov) != 1){
                        printf("Incorrect format in line %d, using default FOV.\n", i);
                        engine.fov = 45.0f;
                    }
                    break;
                case 4:
                    if(sscanf(line, "%d %d", &player_conf.res_x, &player_conf.res_y) != 2){
                        printf("Incorrect format in line %d, using default resolution.\n", i);
                        player_conf.res_x = 1366;
                        player_conf.res_y = 768;
                    }
                    break;
                case 5:
                    if(sscanf(line, "%d", &player_conf.fullscreen) != 1){
                        printf("Incorrect format in line %d, using default fullscreen mode.\n", i);
                        player_conf.fullscreen = 1;
                    }
                    break;
                default:
                    break;
            }
        }
        fclose(config_file);
    }

    if(flags.dflag){
        printf("[DEBUG] Player name: %s\n", player_conf.player_name);
        printf("[DEBUG] In-game language id: %s\n", player_conf.language_id);
        printf("[DEBUG] Player FOV: %lf\n", engine.fov);
        printf("[DEBUG] Width: %d, Height: %d\n", player_conf.res_x, player_conf.res_y);
        player_conf.fullscreen ? printf("[DEBUG] Fullscreen mode: YES\n") : printf("[DEBUG] Fullscreen mode: NO\n");
        printf("\n");
    }

    open_map();

    engine.playerX = pip.pX;
    engine.playerY = pip.pY;
    engine.playerZ = pip.pZ;
    engine.yaw = pip.pA;

    if(initWindow(&engine) == NULL) return -1;

    load_all_tex();

    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(engine.window, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);

    double lastTime = glfwGetTime();
    double deltaTime = 0.0;
    int frame_counter = 0;
    double frameStartTime = lastTime;

    while (!glfwWindowShouldClose(engine.window)){
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
