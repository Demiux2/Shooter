#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <stdio.h>

#define WIN_WIDHT 800
#define WIN_HEIGHT 600

// Definimos una estructura para almacenar el estado del juego
typedef struct {
    GLFWwindow* window;  // Puntero a la ventana
    float playerX, playerY, playerZ;  // Posición del jugador
    float playerAngle;  // Ángulo de visión
    int shoot;  // Estado del disparo (0: no disparando, 1: disparando)
} GameEngine;

// Función para inicializar la ventana y OpenGL
int initWindow(GameEngine* engine, int width, int height, const char* title) {
    if (!glfwInit()) {
        printf("Error al inicializar GLFW\n");
        return -1;
    }

    engine->window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!engine->window) {
        printf("Error al crear la ventana\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(engine->window);
    glfwSwapInterval(1);  // Habilitar V-Sync (opcional)

    return 0;
}

// Función para inicializar el motor de juego
int initGameEngine(GameEngine* engine) {
    // Inicializamos la ventana con dimensiones de 800x600 y título "Shooter Tipo Doom"
    if (initWindow(engine, WIN_WIDHT, WIN_HEIGHT, "Shooter Tipo Doom") != 0) {
        return -1;
    }

    // Inicializar otras partes del motor si es necesario
    engine->playerX = 0.0f;
    engine->playerY = 0.0f;
    engine->playerZ = 0.0f;
    engine->playerAngle = 0.0f;
    engine->shoot = 0;

    return 0;
}

// Función para procesar la entrada del jugador
void processInput(GameEngine* engine) {
    if (glfwGetKey(engine->window, GLFW_KEY_W) == GLFW_PRESS) {
	if((engine->playerY + 0.075f) < 0.9){
        engine->playerY += 0.075f;  // Mover hacia adelante
	}
    }
    if (glfwGetKey(engine->window, GLFW_KEY_S) == GLFW_PRESS) {
	if((engine->playerY - 0.075f) > -0.9){
        engine->playerY -= 0.075f;  // Mover hacia atrás
	}
    }
    if (glfwGetKey(engine->window, GLFW_KEY_A) == GLFW_PRESS) {
	if((engine->playerX - 0.075f) > -0.9){
        engine->playerX -= 0.075f;  // Mover a la izquierda
	}
    }
    if (glfwGetKey(engine->window, GLFW_KEY_D) == GLFW_PRESS) {
	if((engine->playerX + 0.075f) < 0.9){
        engine->playerX += 0.075f;  // Mover a la derecha
	}
    }

    // Disparar
    if (glfwGetKey(engine->window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        engine->shoot = 1;
    } else {
        engine->shoot = 0;
    }

    // Rotación (mirar a izquierda/derecha)
    if (glfwGetKey(engine->window, GLFW_KEY_LEFT) == GLFW_PRESS) {
	if((engine->playerAngle - 1.0f) < -360){
	engine->playerAngle = 0.0f;
	}
	else{
        engine->playerAngle -= 1.0f;  // Girar a la izquierda
	}
    }
    if (glfwGetKey(engine->window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
	if((engine->playerAngle + 1.0f) > 360){
	engine->playerAngle = 0.0f;
	}
	else{
        engine->playerAngle += 1.0f;  // Girar a la derecha
	}
    }


    if (glfwGetKey(engine->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(engine->window, GLFW_TRUE);  // Cerrar la ventana
    }
}

// Función para actualizar la lógica del juego (movimiento, disparos, etc.)
void update(GameEngine* engine) {
    // Lógica para el disparo (aquí solo imprimimos si está disparando)
    if (engine->shoot) {
        printf("Disparando desde (%f, %f) con ángulo %f\n", engine->playerX, engine->playerY, engine->playerAngle);
    }
}

// Función para renderizar la escena
void render(GameEngine* engine) {
    glClear(GL_COLOR_BUFFER_BIT);  // Limpiar la pantalla (buffer de color)
    
    // Aquí podrías agregar código para dibujar el jugador, enemigos, objetos, etc.
    // Ejemplo: renderizar al jugador como un cuadrado en la pantalla
    glPushMatrix();
    glTranslatef(engine->playerX, engine->playerY, 0.0f);  // Mover al jugador
    glBegin(GL_QUADS);  // Dibujar el jugador como un cuadrado
    glColor3f(0.0f, 1.0f, 0.0f);  // Color verde
    glVertex2f(-0.1f, -0.1f);
    glVertex2f( 0.1f, -0.1f);
    glVertex2f( 0.1f,  0.1f);
    glVertex2f(-0.1f,  0.1f);
    glEnd();
    glPopMatrix();
    
    glfwSwapBuffers(engine->window);  // Intercambiar los buffers para mostrar lo renderizado
}

