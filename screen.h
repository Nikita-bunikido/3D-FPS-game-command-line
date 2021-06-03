#include <stdio.h>
/*****************************************
-Nikita Donskov 2021
*****************************************/

#include <string.h>

#include "settings.h"

extern const double DELTA_ANGLE;
extern double distances[];

char shading[] = "@%%#*+=-:. ";
char *skymap;
char *screen;

void newscreen(void){
    int i, j, index;
    double coeff = 20.0f / strlen(shading) - 1;
    for(i = 0; i < w; i++)
        for(j = 0; j < h; j++){
            if((j >= (h / 2) * (1 - 1 / distances[i])) && (j <= (h / 2) * (1 + 1 / distances[i]))){
                index = distances[i] / coeff;
                if(index > strlen(shading) - 1)
                    index = strlen(shading) - 1;
                if(index < 0)
                    index = 0;
                screen[IX(i,j)] = shading[index];
            }
            else
            if(j > h / 2)
                screen[IX(i,j)] = '_';
            else
                screen[IX(i,j)] = skymap[IX(i,j)];
        }
    screen[w*h] = '\0';
}