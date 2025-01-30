#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 512

FILE *map_file;
char map_name[100] = {0};
float pX, pY, pZ, pA;
int walls[2048][4] = {0}, sectors[1024][1024] = {0};

void open_map(FILE *map) {
    int line_counter = 1, header_counter = 0, w_counter = 0, s_counter = 0, w_quantity;
    int i = 0, j = 0;
    char line[MAX_LINE];

    while (fgets(line, sizeof(line), map) != NULL) {
        // Limpiar salto de línea al final de la línea y otros caracteres no deseados
        line[strcspn(line, "\n")] = 0;  // Eliminar el salto de línea
        line[strcspn(line, "\r")] = 0;  // Eliminar el retorno de carro si existe

        if (line_counter == 1) {
            fscanf(line, "%s", map_name);  // El nombre del mapa
            strcpy(map_name, line);
            printf("Map name: %s\n", map_name);
        }

        if (line_counter == 2) {

            // Intentamos leer los parámetros directamente desde la línea
            //int result = sscanf(line, "%f %f %f %f", &pX, &pY, &pZ, &pA);

            // Depuración: Ver los valores leídos
            printf("Player params: pX = %f, pY = %f, pZ = %f, pA = %f\n", pX, pY, pZ, pA);

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
        if (strlen(line) == 0) {
            // Ignorar líneas vacías
            header_counter = 0;
            continue;
        }

        // Procesar [WALLS]
        if (header_counter == 1) {
            sscanf(line, "%i %i %i %i", &walls[w_counter][0], &walls[w_counter][1], &walls[w_counter][2], &walls[w_counter][3]);
            printf("Wall %i: %s\n", w_counter, line);
            w_counter++;
        }

        // Procesar [SECTORS]
        if (header_counter == 2) {
            int sector_idx = 0;
            printf("Reading [SECTORS]\n");

            // Leer la cantidad de sectores en la línea
            if (sscanf(line, "%i", &w_quantity) == 1) {
                // Usar strtok para separar los valores restantes
                char *token = strtok(line, " ");  // El primer token es la cantidad de sectores, lo ignoramos
                token = strtok(NULL, " ");  // El siguiente token será el primer sector
                while (token != NULL && sector_idx < w_quantity) {
                    sectors[i][sector_idx] = atoi(token);  // Guardamos el valor en el arreglo de sectores
                    token = strtok(NULL, " ");  // Continuamos con el siguiente valor
                    sector_idx++;
                }

                // Imprimir los sectores
                printf("Sector %i: ", i);
                for (int k = 0; k < w_quantity; k++) {
                    printf("%i ", sectors[i][k]);
                }
                printf("\n");
                i++;
            }
        }

        line_counter++;
    }
}

int main() {
    map_file = fopen("../maps/test.swnm", "r");
    if (map_file == NULL) {
        printf("Error: could not open the map\n");
        return 1;
    }
    open_map(map_file);
    fclose(map_file);
    return 0;
}
