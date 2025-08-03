#ifndef MAIN_H
#define MAIN_H

#include <GLFW/glfw3.h>

struct flagstruct{
    int fflag, dflag;
};

struct player_config{
    char player_name[64], language_id[64];
    int res_x, res_y;
    int fullscreen;
};

struct GameEngine{
    GLFWwindow* window;
    float playerX, playerY, playerZ;
    float yaw, pitch;
    double fov;
};

extern struct flagstruct flags;
extern struct player_config player_conf;
extern struct GameEngine engine;

GLFWwindow* initWindow(struct GameEngine*);
void draw(struct GameEngine*);
void handleInput(struct GameEngine*, double deltaTime);

#endif
