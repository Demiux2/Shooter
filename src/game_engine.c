#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "./ge/load_map.h"
#include "main.h"

extern struct p_initial_pos pip;
extern struct flagstruct flags;
extern struct GameEngine engine;

int width = 1366, height = 768;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

//Función para inicializar GLFW y OpenGL
GLFWwindow* initWindow(struct GameEngine*){

    if(!glfwInit()) return NULL;

    strcat(pip.map_name, " - Shooter Without Name");

    //Activar doble buffer
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

    engine.window = glfwCreateWindow(width, height, pip.map_name, NULL, NULL);
    if(!engine.window){
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(engine.window);
    glEnable(GL_TEXTURE_2D);

    //Deshabilitar V-Sync
    glfwSwapInterval(0);

    //Configuración de la perspectiva
    glMatrixMode(GL_PROJECTION); //Matriz de proyección
    glLoadIdentity();            //Limpiar la matriz de proyección actual
    gluPerspective(engine.fov, (float)width / height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);  //Volver a la matriz de modelo

    //Activar el test de profundidad
    glEnable(GL_DEPTH_TEST);

    glfwSetKeyCallback(engine.window, key_callback);

    return engine.window;
}
void draw(struct GameEngine*) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(engine.fov, (float)width / height, 0.1f, 100.0f);  
    glMatrixMode(GL_MODELVIEW);

    //Establecer la cámara
    glLoadIdentity(); //Restablecer la matriz de modelo
    gluLookAt(engine.playerX, engine.playerY, engine.playerZ, //Posición de la cámara
              engine.playerX + cos(engine.angle * 3.14159f / 180.0f), engine.playerY, engine.playerZ + sin(engine.angle * 3.14159f / 180.0f), //Ángulo de la cámara
              0.0f, 1.0f, 0.0f);

    //Aplicar la escala 3D a la escena
    glPushMatrix();
    glScalef(2.0f, 2.0f, 2.0f);

    glBegin(GL_QUADS);
    	render_map();
    glEnd();

    glPopMatrix(); //Restaurar la matriz de transformación
    
    glfwSwapBuffers(engine.window);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(key == GLFW_KEY_F && action == GLFW_PRESS && (mods & GLFW_MOD_CONTROL)){
        if(flags.fflag) flags.fflag = 0;
	else flags.fflag = 1;
	printf("DEBUG: Toggled fflag with Ctrl+F\n");
    }
}
        

void handleInput(struct GameEngine*, double deltaTime){
    float speed = 10.0f;
    float rotationSpeed = 75.0f;

    //Comprobamos las teclas presionadas
    if(glfwGetKey(engine.window, GLFW_KEY_W) == GLFW_PRESS){
        engine.playerX += cos(engine.angle * 3.14159f / 180.0f) * speed * deltaTime; //Mover hacia adelante
        engine.playerZ += sin(engine.angle * 3.14159f / 180.0f) * speed * deltaTime;
    }
    if(glfwGetKey(engine.window, GLFW_KEY_S) == GLFW_PRESS){
        engine.playerX -= cos(engine.angle * 3.14159f / 180.0f) * speed * deltaTime; //Mover hacia atrás
        engine.playerZ -= sin(engine.angle * 3.14159f / 180.0f) * speed * deltaTime;
    }
    if(glfwGetKey(engine.window, GLFW_KEY_LEFT) == GLFW_PRESS){
        engine.angle -= rotationSpeed * deltaTime; //Rotar hacia la izquierda
    }
    if(glfwGetKey(engine.window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        engine.angle += rotationSpeed * deltaTime; //Rotar hacia la derecha
    }
    //Movimiento perpendicular
    if(glfwGetKey(engine.window, GLFW_KEY_A) == GLFW_PRESS){
        engine.playerX += sin(engine.angle * 3.14159f / 180.0f) * speed * deltaTime; //Movimiento a la izquierda
        engine.playerZ -= cos(engine.angle * 3.14159f / 180.0f) * speed * deltaTime;
    }
    if(glfwGetKey(engine.window, GLFW_KEY_D) == GLFW_PRESS){
        engine.playerX -= sin(engine.angle * 3.14159f / 180.0f) * speed * deltaTime; //Movimiento a la derecha
        engine.playerZ += cos(engine.angle * 3.14159f / 180.0f) * speed * deltaTime;
    }
    if(glfwGetKey(engine.window, GLFW_KEY_UP) == GLFW_PRESS){
        engine.playerY +=  speed * deltaTime; //Movimiento hacia arriba
    }
    if(glfwGetKey(engine.window, GLFW_KEY_DOWN) == GLFW_PRESS){
        engine.playerY -= speed * deltaTime; //Movimiento hacia abajo
    }


    //Modificar la escala
    if(glfwGetKey(engine.window, GLFW_KEY_E) == GLFW_PRESS){
        engine.fov += 2.5f * deltaTime; //Aumentar el FOV
        if(engine.fov > 120.0f) engine.fov = 120.0f; //Limitar la escala máxima
    }
    if(glfwGetKey(engine.window, GLFW_KEY_Q) == GLFW_PRESS){
        engine.fov -= 2.5f * deltaTime; //Reducir el FOV
        if(engine.fov < 30.0f) engine.fov = 30.0f; //Limitar la escala mínima
    }

    if(glfwGetKey(engine.window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(engine.window)){
        glfwSetWindowShouldClose(engine.window, GLFW_TRUE);
    }
}
