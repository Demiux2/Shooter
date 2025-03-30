#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE 256

FILE *map_file;
char map_name[100] = {0};
float pX, pY, pZ, pA;
int walls[2048][4] = {0}, sectors[1024][1024] = {0};
int w_mats[2048], w_mat = 0, sw_c = 0;
float wall_colors[2048][3];

int w_counter = 0, s_counter = 0;

int map_loaded = 0;

void open_map() {
    if(map_loaded){
        return;
    }

    map_loaded = 1;

    map_file = fopen("./maps/test3.swnm", "r");
    if(map_file == NULL){
        map_file = fopen("./src/maps/test3.swnm", "r");
        if(map_file == NULL){
            printf("Error: could not open the map\n");
            exit(1);
        }
    }

    int line_counter = 1, header_counter = 0, w_quantity;
    int i = 0, j = 0;
    char line[MAX_LINE];

    srand(time(NULL));

while(fgets(line, sizeof(line), map_file) != NULL){
        //Limpiar salto de línea al final de la línea y otros caracteres no deseados
        line[strcspn(line, "\n")] = 0;  //Eliminar el salto de línea
        line[strcspn(line, "\r")] = 0;  //Eliminar el retorno de carro si existe

        if(line_counter == 1){
            sscanf(line, "%s", map_name);  //El nombre del mapa
            strcpy(map_name, line);
            printf("Map name: %s\n", map_name);
        }

        if(line_counter == 2){
            if(sscanf(line, "%f %f %f %f", &pX, &pY, &pZ, &pA) != 4){
                printf("Error: Incorrect amount of player parameters (at line 2)\n");
                exit(1);
            }
            line_counter++;
            continue;
        }

        //Detectar encabezados de secciones
        if(strcmp(line, "[WALLS]") == 0){
            printf("Reading [WALLS]\n");
            header_counter = 1;
            continue;
        }
        if(strcmp(line, "[SECTORS]") == 0){
            header_counter = 2;
            continue;
        }

        //Procesar [WALLS]
        if(header_counter == 1){
            if(sscanf(line, "%i %i %i %i %i", &walls[w_counter][0], &walls[w_counter][1], &walls[w_counter][2], &walls[w_counter][3], &w_mat) == 5){
                //Generación de color aleatorio para cada pared
                wall_colors[w_counter][0] = ((float)rand() / (float)(RAND_MAX)); //Rojo
                wall_colors[w_counter][1] = ((float)rand() / (float)(RAND_MAX)); //Verde
                wall_colors[w_counter][2] = ((float)rand() / (float)(RAND_MAX)); //Azul

                printf("Wall %d: (%d, %d; %d, %d) Color: (%f, %f, %f)\n", 
                        w_counter, walls[w_counter][0], walls[w_counter][1], 
                        walls[w_counter][2], walls[w_counter][3],
                        wall_colors[w_counter][0], wall_colors[w_counter][1], wall_colors[w_counter][2]);
                w_counter++;
            }

	    else{
                printf("Error: Invalid wall format in line: %s\n", line);
            }
        }

        //Procesar [SECTORS]
        if(header_counter == 2){
            char *ptr = line;
            int spc = 0;
            if(sscanf(ptr, "%d", &sw_c) == 1){
                printf("Number of walls in sector %d: %d\n", spc + 1, sw_c);
                
                //Avanzamos el puntero después de leer la cantidad inicial
                while(*ptr != ' ' && *ptr != '\0'){
                    ptr++;  //Avanzamos hasta el primer espacio o fin de cadena
                }
                if(*ptr == ' '){
                    ptr++;  //Mueve el puntero al siguiente valor después del espacio
                }

                //Ahora leemos los valores según la cantidad especificada (sw_c)
                while(spc < sw_c && sscanf(ptr, "%d", &sectors[s_counter][spc]) == 1){
                    printf("Wall ID %d: %d\n", spc + 1, sectors[s_counter][spc]);
                    while(*ptr != ' ' && *ptr != '\0'){
                        ptr++;  //Avanza el puntero hasta el siguiente espacio o fin
                    }
                    if(*ptr == ' '){
                        ptr++;  //Mueve el puntero después del espacio
                    }
                    spc++;
                }

                //Leer floor_height, floor_mat, ceiling_height y ceiling_mat
                for(int i = 0; i < 4; i++){
                    if(sscanf(ptr, "%d", &sectors[s_counter][sw_c + i]) == 1){
                        ptr = strchr(ptr, ' ');  //Avanza al siguiente espacio
                        if(ptr) ptr++;  //Mueve el puntero después del espacio
                    }
                    else{
                        break;
                    }
                }

                s_counter++;
            }
            else{
                printf("Error: Could not get \"n_walls\" in the sector number %s\n", line);
            }
        }

        line_counter++;
    }
}

int render_map(){

    for(int i = 0; i < w_counter; i++){
        //Asignar el color previamente calculado de la pared
        glColor3f(wall_colors[i][0], wall_colors[i][1], wall_colors[i][2]);

        //Dibujar la pared
        glBegin(GL_QUADS);
            glVertex3f(walls[i][0], 0, walls[i][1]);
            glVertex3f(walls[i][0], 5, walls[i][1]);
            glVertex3f(walls[i][2], 5, walls[i][3]);
            glVertex3f(walls[i][2], 0, walls[i][3]);
        glEnd();
    }
    
    for(int s = 0; s < s_counter; s++){
        float floor_height = sectors[s][sw_c];
        float ceiling_height = sectors[s][sw_c + 2];

        //Dibujar el piso
        glColor3f(0.243f, 0.259f, 0.294f); //"Anchor gray" para el piso
        glBegin(GL_POLYGON);
        for(int i = 0; i < sw_c; i++){
            int wall_id = sectors[s][i];  //ID de la pared
            int x1 = walls[wall_id][0];
            int y1 = walls[wall_id][1];

            //Dibujar el piso (en y = floor_height)
            glVertex3f(x1, floor_height, y1); 
        }
        glEnd();

        //Dibujar el techo
        glColor3f(0.607f, 0.607f, 0.560f); //"Trout gray" para el techo
        glBegin(GL_POLYGON);
        for(int i = 0; i < sw_c; i++){
            int wall_id = sectors[s][i];
            int x1 = walls[wall_id][0];
            int y1 = walls[wall_id][1];

            //Dibujar el techo (en y = ceiling_height)
            glVertex3f(x1, ceiling_height, y1);
        }
        glEnd();
    }

    return 0;
}
