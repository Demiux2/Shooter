#include <GL/gl.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
//#include "ge/load_map.c"

int width = 1366, height = 768;

// Definir la estructura del motor de juego
typedef struct {
    GLFWwindow* window; // Puntero a la ventana
    float playerX, playerY, playerZ; // Posición inicial del jugador
    float angle; // Ángulo de rotación
    double fov;
} GameEngine;

// Función para inicializar GLFW y OpenGL
GLFWwindow* initWindow(GameEngine* engine) {
    if (!glfwInit()) {
        return NULL;
    }

    // Asegurémonos de que el doble buffer esté activado
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);  // Asegurarnos de usar doble buffer

    engine->window = glfwCreateWindow(width, height, "Shooter Without Name", NULL, NULL);
    if (!engine->window) {
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(engine->window);

    // Deshabilitar V-Sync para probar si ayuda (puedes cambiarlo a 1 si prefieres habilitarlo)
    glfwSwapInterval(0);  // Deshabilitar V-Sync

    // Configuración de la perspectiva
//    engine->fov = 45.0f;  // Ángulo de visión (FOV)
    glMatrixMode(GL_PROJECTION);   // Seleccionar la matriz de proyección
    glLoadIdentity();              // Limpiar la matriz de proyección actual
    gluPerspective(engine->fov, (float)width / height, 0.1f, 100.0f);  // Configurar perspectiva
    glMatrixMode(GL_MODELVIEW);    // Volver a la matriz de modelo

    // Activar el test de profundidad
    glEnable(GL_DEPTH_TEST);  // Asegúrate de tener esto activado

    return engine->window;
}
void draw(GameEngine* engine) {
    // Limpiar los buffers de color y profundidad
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Limpiar los dos buffers al principio del ciclo de dibujo

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(engine->fov, (float)width / height, 0.1f, 100.0f);  
    glMatrixMode(GL_MODELVIEW);    // Volver a la matriz de modelo

    // Establecer la cámara con las nuevas coordenadas del jugador
    glLoadIdentity();  // Restablecer la matriz de modelo
    gluLookAt(engine->playerX, engine->playerY, engine->playerZ,  // Posición de la cámara
              engine->playerX + cos(engine->angle * 3.14159f / 180.0f), engine->playerY, engine->playerZ + sin(engine->angle * 3.14159f / 180.0f),  // Dirección de la cámara
              0.0f, 1.0f, 0.0f);  // El "arriba" es el eje Y

    // Aplicar la escala 3D a la escena
    glPushMatrix();
    glScalef(2.0f, 2.0f, 2.0f);  // Escalar todo en 3D


    // Dibuja la pirámide (esfera, cubo o lo que desees en 3D)
    glBegin(GL_QUADS);
    	render_map();
    glEnd();  // Finalizar la definición de los triángulos

    glPopMatrix();  // Restaurar la matriz de transformación

    // Intercambiar los buffers de la pantalla
    glfwSwapBuffers(engine->window);  // Intercambia los buffers (esto es crucial)
}

// Función para manejar la entrada del jugador
void handleInput(GameEngine* engine, double deltaTime) {
    float speed = 10.0f;  // Velocidad de movimiento en unidades por segundo
    float rotationSpeed = 75.0f;  // Velocidad de rotación en grados por segundo

    // Comprobamos las teclas presionadas
    if (glfwGetKey(engine->window, GLFW_KEY_W) == GLFW_PRESS) {
        engine->playerX += cos(engine->angle * 3.14159f / 180.0f) * speed * deltaTime;  // Mover hacia adelante
        engine->playerZ += sin(engine->angle * 3.14159f / 180.0f) * speed * deltaTime;
    }
    if (glfwGetKey(engine->window, GLFW_KEY_S) == GLFW_PRESS) {
        engine->playerX -= cos(engine->angle * 3.14159f / 180.0f) * speed * deltaTime;  // Mover hacia atrás
        engine->playerZ -= sin(engine->angle * 3.14159f / 180.0f) * speed * deltaTime;
    }
    if (glfwGetKey(engine->window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        engine->angle -= rotationSpeed * deltaTime;  // Rotar hacia la izquierda
    }
    if (glfwGetKey(engine->window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        engine->angle += rotationSpeed * deltaTime;  // Rotar hacia la derecha
    }
    // Movimiento perpendicular
    if (glfwGetKey(engine->window, GLFW_KEY_A) == GLFW_PRESS) {
        engine->playerX += sin(engine->angle * 3.14159f / 180.0f) * speed * deltaTime;  // Movimiento a la izquierda
        engine->playerZ -= cos(engine->angle * 3.14159f / 180.0f) * speed * deltaTime;
    }
    if (glfwGetKey(engine->window, GLFW_KEY_D) == GLFW_PRESS) {
        engine->playerX -= sin(engine->angle * 3.14159f / 180.0f) * speed * deltaTime;  // Movimiento a la derecha
        engine->playerZ += cos(engine->angle * 3.14159f / 180.0f) * speed * deltaTime;
    }
    if (glfwGetKey(engine->window, GLFW_KEY_UP) == GLFW_PRESS) {
        engine->playerY +=  speed * deltaTime;  // Movimiento hacia arriba
    }
    if (glfwGetKey(engine->window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        engine->playerY -= speed * deltaTime;  // Movimiento hacia abajo
    }


    // Modificar la escala
    if (glfwGetKey(engine->window, GLFW_KEY_E) == GLFW_PRESS) {
        engine->fov += 2.5f * deltaTime;  // Aumentar el FOV
        if (engine->fov > 120.0f) engine->fov = 120.0f;  // Limitar la escala máxima
    }
    if (glfwGetKey(engine->window, GLFW_KEY_Q) == GLFW_PRESS) {
        engine->fov -= 2.5f * deltaTime;  // Reducir el FOV
        if (engine->fov < 30.0f) engine->fov = 30.0f;  // Limitar la escala mínima
    }

    // Salir si se presiona Escape
    if (glfwGetKey(engine->window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(engine->window)) {
        glfwSetWindowShouldClose(engine->window, GLFW_TRUE);  // Salir
    }
}

