#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "load_map.h"
#include "../main.h"
#include "load_textures.h"

#define MAX_WALLS 2048
#define MAX_SECTORS 1024

extern struct p_initial_pos pip;
extern struct flagstruct flags;

int walls[MAX_WALLS][4] = {0}, sectors[MAX_SECTORS][MAX_WALLS + 5] = {0};
int w_mats[MAX_WALLS] = {0};
float wall_colors[MAX_WALLS][3] = {0};

int w_counter = 0, s_counter = 0;

void open_map(){
    FILE *map_file;
    int map_loaded = 0;
    if(map_loaded) return;

    char path1[125] = "./maps/";
    char path2[125] = "./src/maps/";
    strcpy(pip.map, pip.filename);
    strcat(path1, pip.map);
    strcat(path2, pip.map);

    map_loaded = 1;

    map_file = fopen(path1, "r");
    if(map_file == NULL){
        map_file = fopen(path2, "r");
        if(map_file == NULL){
            printf("Error: could not open the map.\n");
            exit(1);
        }
        printf("[DEBUG] READING %s", pip.filename);
    }

    int line_counter = 1, header_counter = 0, w_quantity;
    char line[512];

    srand(time(NULL));

    while(fgets(line, sizeof(line), map_file) != NULL){
        line[strcspn(line, "\n")] = 0;
        line[strcspn(line, "\r")] = 0;

        if(line[0] == '\0') continue;

        if(line_counter == 1){
            strcpy(pip.map_name, line);
            if(flags.dflag) printf("[DEBUG] Map name: %s\n", pip.map_name);
        }

        if(line_counter == 2){
            if(sscanf(line, "%f %f %f %f", &pip.pX, &pip.pY, &pip.pZ, &pip.pA) != 4){
                printf("Error: Incorrect amount of player parameters (at line 2)\n");
                exit(1);
            }
	    if(flags.dflag) printf("[DEBUG] Player parameters: X:%lf, Y:%lf, Z:%lf, Yaw:%lf\n", pip.pX, pip.pY, pip.pZ, pip.pA);
            line_counter++;
            continue;
        }

        if(strcmp(line, "[WALLS]") == 0){
	        if(flags.dflag)
		        printf("\n[DEBUG] Reading [WALLS]:\n");
            header_counter = 1;
            continue;
        }

        if(strcmp(line, "[SECTORS]") == 0){
	        if(flags.dflag)
		        printf("\n[DEBUG] Reading [SECTORS]:\n");
            header_counter = 2;
            continue;
        }

        //Procesar [WALLS]
        if(header_counter == 1){
            if(sscanf(line, "%i %i %i %i %i", &walls[w_counter][0], &walls[w_counter][1], &walls[w_counter][2], &walls[w_counter][3], &w_mats[w_counter]) == 5){
                wall_colors[w_counter][0] = ((float)rand() / (float)(RAND_MAX));
                wall_colors[w_counter][1] = ((float)rand() / (float)(RAND_MAX));
                wall_colors[w_counter][2] = ((float)rand() / (float)(RAND_MAX));

		    if(flags.dflag)
                        if(w_mats[w_counter] == 0){
                            printf("[DEBUG] Wall %d: p1(%d, %d), p2(%d, %d) - Color: (%f, %f, %f)\n", 
                            w_counter + 1, walls[w_counter][0], walls[w_counter][1], 
                            walls[w_counter][2], walls[w_counter][3],
                            wall_colors[w_counter][0], wall_colors[w_counter][1], wall_colors[w_counter][2]);
			}
                        else printf("[DEBUG] Wall %d: p1(%d, %d), p2(%d, %d) - Material: %d\n",
                            w_counter + 1, walls[w_counter][0], walls[w_counter][1], 
                            walls[w_counter][2], walls[w_counter][3],
                            w_mats[w_counter]);

                w_counter++;
            }
        else printf("Error: Invalid wall format in line: %s\n", line);
        }

        //Procesar [SECTORS]
        if(header_counter == 2){
            char *ptr = line;
            int wall_count = 0;

            if(sscanf(ptr, "%d", &wall_count) == 1){
                if(flags.dflag)
                    printf("[DEBUG] Sector %d has %d walls, which IDs are: ", s_counter + 1, wall_count);

                sectors[s_counter][0] = wall_count;

                while (*ptr != ' ' && *ptr != '\0') ptr++;
                if (*ptr == ' ') ptr++;

                for(int i = 1; i <= wall_count; i++){
                    if(sscanf(ptr, "%d", &sectors[s_counter][i]) != 1){
                        printf("Error: Missing wall ID in sector %d\n", s_counter + 1);
                        break;
                    }
                    if(flags.dflag){
                        printf("%d", sectors[s_counter][i]);
                        if(i < wall_count) printf(", ");
                    }

                    while(*ptr != ' ' && *ptr != '\0') ptr++;
                    if(*ptr == ' ') ptr++;
                }

                if(flags.dflag) printf("\n");

                for(int i = 0; i < 4; i++){
                    if(sscanf(ptr, "%d", &sectors[s_counter][wall_count + 1 + i]) == 1){
                        ptr = strchr(ptr, ' ');
                        if(ptr) ptr++;
                    }
                    else{
                        printf("Error: Invalid sector format in line: %s\n", line);
                        break;
                    }
                }
                s_counter++;
            }
            else printf("Error: Could not read wall count in sector %d\n", s_counter - 1);   
        }
        line_counter++;
    }
    fclose(map_file);
}

int render_map(){

    for(int i = 0; i < w_counter; i++){
        if(w_mats[i] > 0){
            glEnable(GL_TEXTURE_2D);
	    if(w_mats[i] != w_mats[i-1]) glBindTexture(GL_TEXTURE_2D, textures[w_mats[i]]);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            glColor3f(1.0f, 1.0f, 1.0f);
        }
        else{
            glDisable(GL_TEXTURE_2D);
            glColor3f(wall_colors[i][0], wall_colors[i][1], wall_colors[i][2]);
        }

        //Dibujar la pared
        float x1 = walls[i][0], x2 = walls[i][2], z1 = walls[i][1], z2 = walls[i][3];

        float dx = x2 - x1, dz = z2 - z1;
        float length = sqrt(dx * dx + dz * dz);

        float texture_repeat = length / 1.0f;

        glBegin(GL_QUADS);
            if(w_mats[i] > 0){
                glTexCoord2f(0.0f          , 0.0f          );  glVertex3f(x1, 0.0f, z1);
                glTexCoord2f(0.0f          , texture_repeat);  glVertex3f(x1, 5.0f, z1);
                glTexCoord2f(texture_repeat, texture_repeat);  glVertex3f(x2, 5.0f, z2);
                glTexCoord2f(texture_repeat, 0.0f          );  glVertex3f(x2, 0.0f, z2);
            }
            else{
                glVertex3f(x1, 0.0f, z1);
                glVertex3f(x1, 5.0f, z1);
                glVertex3f(x2, 5.0f, z2);
                glVertex3f(x2, 0.0f, z2);
            }
        glEnd();
    }

    for(int s = 0; s < s_counter; s++){
        int wall_id;
        int wall_count = sectors[s][0];
        float floor_height = sectors[s][wall_count + 1];
        float ceiling_height = sectors[s][wall_count + 3];
        for(int i = 1; i <= wall_count; i++){
            int wall_id = sectors[s][i];
	}
	if(walls[wall_id - 1][1] != walls[wall_id][0]){
            int temp = walls[wall_id][0];
            walls[wall_id][0] = walls[wall_id][1];
            walls[wall_id][1] = temp;
        }

        // Dibujar el piso
        glColor3f(0.243f, 0.259f, 0.294f); //"Anchor gray" para el piso
        glBegin(GL_POLYGON);
        for(int i = 1; i <= wall_count; i++){
            int wall_id = sectors[s][i];
            glVertex3f(walls[wall_id][0], floor_height, walls[wall_id][1]);
        }
        glEnd();

        // Dibujar el techo
        glColor3f(0.607f, 0.607f, 0.560f); //"Trout gray" para el techo
        glBegin(GL_POLYGON);
        for(int i = 1; i <= wall_count; i++){
            int wall_id = sectors[s][i];
            glVertex3f(walls[wall_id][0], ceiling_height, walls[wall_id][1]);
        }
        glEnd();
    }
    return 0;
}
