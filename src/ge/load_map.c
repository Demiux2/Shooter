#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE 256

FILE *map_file;
char map_name[100] = {0};
float pX, pY, pZ, pA;
int walls[2048][4] = {0}, sectors[1024][1024] = {0};
int w_mats[2048], w_mat;
float wall_colors[2048][3];

int w_counter = 0, s_counter = 0;

int map_loaded = 0;

void open_map() {
    if (map_loaded) {
        return;  // Si el mapa ya está cargado, no hacer nada
    }

    map_loaded = 1;  // Marcar como cargado

    map_file = fopen("./maps/test2.swnm", "r");
    if (map_file == NULL) {
        printf("Error: could not open the map\n");
        exit(1);
    }

    int line_counter = 1, header_counter = 0, w_quantity;
    int i = 0, j = 0;
    char line[MAX_LINE];

while (fgets(line, sizeof(line), map_file) != NULL) {
        // Limpiar salto de línea al final de la línea y otros caracteres no deseados
        line[strcspn(line, "\n")] = 0;  // Eliminar el salto de línea
        line[strcspn(line, "\r")] = 0;  // Eliminar el retorno de carro si existe

        if (line_counter == 1) {
            sscanf(line, "%s", map_name);  // El nombre del mapa
            strcpy(map_name, line);
            printf("Map name: %s\n", map_name);
        }

        if (line_counter == 2) {
            if (sscanf(line, "%f %f %f %f", &pX, &pY, &pZ, &pA) != 4) {
                printf("Error: Incorrect amount of player parameters (at line 2)\n");
                exit(1);
            }
            line_counter++;
            continue;
        }

        // Detectar encabezados de secciones
        if (strcmp(line, "[WALLS]") == 0) {
            printf("Reading [WALLS]\n");
            header_counter = 1;
            continue;
        }
        if (strcmp(line, "[SECTORS]") == 0) {
            header_counter = 2;
            continue;
        }

        // Procesar [WALLS]
        if (header_counter == 1) {
            if (sscanf(line, "%i %i %i %i %i", &walls[w_counter][0], &walls[w_counter][1], &walls[w_counter][2], &walls[w_counter][3], &w_mat) == 5) {
                // Generar color aleatorio para cada pared
                wall_colors[w_counter][0] = ((float)rand() / (float)(RAND_MAX)); // Rojo
                wall_colors[w_counter][1] = ((float)rand() / (float)(RAND_MAX)); // Verde
                wall_colors[w_counter][2] = ((float)rand() / (float)(RAND_MAX)); // Azul

                printf("Wall %d: (%d, %d, %d, %d) Color: (%f, %f, %f)\n", 
                        w_counter, walls[w_counter][0], walls[w_counter][1], 
                        walls[w_counter][2], walls[w_counter][3],
                        wall_colors[w_counter][0], wall_colors[w_counter][1], wall_colors[w_counter][2]);
                w_counter++;
            } else {
                printf("Error: Invalid wall format in line: %s\n", line);
            }
        }

        // Procesar [SECTORS]
        if (header_counter == 2) {
            // Código para procesar sectores
        }

        line_counter++;
    }
}

int render_map() {
    for (int i = 0; i < w_counter; i++) {  // Recorrer todas las paredes
        // Asignar el color de la pared previamente calculado
        glColor3f(wall_colors[i][0], wall_colors[i][1], wall_colors[i][2]);

        // Dibujar la pared como un cuadrilátero (plano)
        glBegin(GL_QUADS);
            glVertex3f(walls[i][0], 0, walls[i][1]); // Primer vértice (suelo)
            glVertex3f(walls[i][0], 5, walls[i][1]); // Segundo vértice (arriba)
            glVertex3f(walls[i][2], 5, walls[i][3]); // Tercer vértice (arriba del otro lado)
            glVertex3f(walls[i][2], 0, walls[i][3]); // Cuarto vértice (suelo del otro lado)
        glEnd();
    }

    return 0;  // Retorno exitoso
}
