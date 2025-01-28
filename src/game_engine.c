#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>

// Definir la estructura del motor de juego
typedef struct {
    GLFWwindow* window; // Puntero a la ventana
    float playerX, playerZ; // Posición inicial del jugador
    float angle; // Ángulo de rotación
    float scale; // Factor de escala
} GameEngine;

// Función para inicializar GLFW y OpenGL
GLFWwindow* initWindow(GameEngine* engine) {
    if (!glfwInit()) {
        return NULL;
    }

    engine->window = glfwCreateWindow(800, 600, "BASIC to OpenGL", NULL, NULL);
    if (!engine->window) {
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(engine->window);
    glOrtho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f); // Configuración ortográfica
    return engine->window;
}

// Función de dibujado
void draw(GameEngine* engine) {
    glClear(GL_COLOR_BUFFER_BIT);

    // Aplicar la escala a la escena
    glPushMatrix();   // Guardar la matriz de transformación actual
    glScalef(engine->scale, engine->scale, 1.0f);  // Escalar en X y Y (solo en 2D)

    // Dibuja una línea fija (roja)
    glColor3f(1.0f, 0.0f, 0.0f);  // Color rojo
    glBegin(GL_LINES);
    glVertex2f(200, 140);           // Primer punto
    glVertex2f(203, 249);          // Segundo punto
    glEnd();

    // Dibuja la línea que rota (azul)
    float vx2 = engine->playerX + cos(engine->angle * 3.14159f / 180.0f) * 50;
    float vy2 = engine->playerZ + sin(engine->angle * 3.14159f / 180.0f) * 50;
    glColor3f(0.0f, 0.0f, 1.0f);  // Color azul
    glBegin(GL_LINES);
    glVertex2f(engine->playerX, engine->playerZ);           // Punto inicial
    glVertex2f(vx2, vy2);         // Punto final
    glEnd();

    // Dibuja el punto en la posición actual (blanco)
    glPointSize(10.0f);
    glColor3f(1.0f, 1.0f, 1.0f);  // Color blanco
    glBegin(GL_POINTS);
    glVertex2f(engine->playerX, engine->playerZ);
    glEnd();

    glPopMatrix();  // Restaurar la matriz de transformación
}

// Función para manejar la entrada del jugador
void handleInput(GameEngine* engine) {
    // Comprobamos las teclas presionadas
    if (glfwGetKey(engine->window, GLFW_KEY_UP) == GLFW_PRESS) {
        engine->playerX += cos(engine->angle * 3.14159f / 180.0f);  // Mover hacia adelante
        engine->playerZ += sin(engine->angle * 3.14159f / 180.0f);
    }
    if (glfwGetKey(engine->window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        engine->playerX -= cos(engine->angle * 3.14159f / 180.0f);  // Mover hacia atrás
        engine->playerZ -= sin(engine->angle * 3.14159f / 180.0f);
    }
    if (glfwGetKey(engine->window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        engine->angle -= 1.0f;  // Rotar hacia la izquierda
    }
    if (glfwGetKey(engine->window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        engine->angle += 1.0f;  // Rotar hacia la derecha
    }

    // Movimiento perpendicular
    if (glfwGetKey(engine->window, GLFW_KEY_A) == GLFW_PRESS) {
        engine->playerX += sin(engine->angle * 3.14159f / 180.0f);  // Movimiento a la izquierda
        engine->playerZ -= cos(engine->angle * 3.14159f / 180.0f);
    }
    if (glfwGetKey(engine->window, GLFW_KEY_D) == GLFW_PRESS) {
        engine->playerX -= sin(engine->angle * 3.14159f / 180.0f);  // Movimiento a la derecha
        engine->playerZ += cos(engine->angle * 3.14159f / 180.0f);
    }

    // Modificar la escala
    if (glfwGetKey(engine->window, GLFW_KEY_W) == GLFW_PRESS) {
        engine->scale += 0.01f;  // Aumentar la escala
    }
    if (glfwGetKey(engine->window, GLFW_KEY_S) == GLFW_PRESS) {
        engine->scale -= 0.01f;  // Reducir la escala
        if (engine->scale < 0.1f) engine->scale = 0.1f;  // Limitar la escala mínima
    }

    // Salir si se presionan las teclas Q o Escape
    if (glfwGetKey(engine->window, GLFW_KEY_Q) == GLFW_PRESS || glfwGetKey(engine->window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(engine->window)) {
        glfwSetWindowShouldClose(engine->window, GLFW_TRUE);  // Salir
    }
}

