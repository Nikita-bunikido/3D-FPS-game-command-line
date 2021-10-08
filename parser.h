#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "settings.h"

void parsetextures(int walls, char tets[][texturex][texturey]){
    size_t i;
    char c = 0;
    const char path[] = "RES\\wall";
    char       current_path[256];
    char       prom[256];
    FILE* current;
    
    for (i = 0; i < walls; ++i){
        itoa(i+1, prom, 10);
        strcat(prom, ".txt");
        strcpy(current_path, path);
        strcat(current_path, prom);

        current = fopen(current_path, "r");
        for(int py = 0; py < texturey; py++)
            for(int px = 0; px < texturex; px++){
                do 
                    c = fgetc(current); 
                while (c == 10);
                tets[i][py][px] = c;
            }
        fclose(current);
    }
}