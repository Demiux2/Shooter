#include <stdio.h>
#include <string.h>
#include "../main.h"

#define MAX_TRANSLATIONS 256

typedef struct{
    char trad_key[64];
    char value[512];
}Translations;

Translations translations[MAX_TRANSLATIONS] = {0};
extern struct flagstruct flags;

int load_translations(char lang_code[]){
    char filename[32];
    sprintf(filename, "%s/%s.%s", "build/lang", lang_code, "lang");

    FILE *file = fopen(filename, "r");
    if(file == NULL && flags.dflag){
        if(flags.dflag){
            printf("[DEBUG] Could not open %s, trying en_us.lang instead.\n", filename);
            file = fopen("build/lang/en_us.lang", "r");
            if(file == NULL && flags.dflag) printf("[DEBUG] It also wasn't possible to open en_us.lang, so no translations are not going to be loaded.\n");
            else if(file == NULL) printf("Couldn't open any translation files, so no translations are going to be available.\n");
        }
        return 1;
    }

    char line[576] = {0};
    int counter = 0;
    while(fgets(line, sizeof(line), file) != NULL){

        line[strcspn(line, "\n")] = 0;
        line[strcspn(line, "\r")] = 0;
        if (line[0] == '\0' || line[0] == '#') continue;

        if(sscanf(line, "%[^=]=%[^\n]", translations[counter].trad_key, translations[counter].value) != 2)
            if(flags.dflag) printf("[DEBUG] Invalid translation key in line %d.%s\n", counter, line);

        counter++;
    }
    fclose(file);
    return 0;
}

void print_translated(char key[]){
    for(int i = 0; i < MAX_TRANSLATIONS; i++){
        if(strcmp(key, translations[i].trad_key) == 0){
            printf("%s\n", translations[i].value);
            return;
        }
        if(strcmp(key, translations[i].trad_key) == 0 && i + 1 == MAX_TRANSLATIONS){
            printf("%s\n", key);
            return;
        }
    }
}
