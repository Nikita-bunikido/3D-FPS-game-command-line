/*****************************************
This is a implementation of pseudo-3d game
in command promt.
                      -Nikita Donskov 2021
*****************************************/

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "settings.h"
#include "management.h"
#include "screen.h"
#include "map.h"

const double DELTA_ANGLE = FOV / NUM_RAYS;
double playerX = tile + tile / 3;
double playerY = tile + tile / 3;
double playerA = 0;
double distances[w];

double radians (double a){
    return a * M_PI / 180;
}

double RAYCOLLISION(double x, double y){
    int i, j;
    for(i = 0; i < mapY; i++){
        for(j = 0; j < mapX; j++){
            if((x > j * tile && x < j * tile + tile) && (y > i * tile && y < i * tile + tile) && (MAP[j][i] == '#'))
                return sqrt(pow((double)(x - playerX), 2) + pow((double)(y - playerY), 2));
        }
    }
    return -1;
}

void castrays(void){
    double rayX = playerX, rayY = playerY;
    double rayA = playerA - FOV / 2;
    double rayR = radians(rayA);
    double prom;
    int i, s;
    int end = 0;
    for(i = 0; i < NUM_RAYS; i++){
        rayA += DELTA_ANGLE;
        rayR = radians(rayA);
        for(s = 0; s < MAX_DEPTH; s++){
            end = 0;
            if((prom = RAYCOLLISION(rayX, rayY)) != -1){
                distances[i] = prom;
                end = 1;
                break;
            }
            rayX += 0.2f * cos(rayR);
            rayY += 0.2f * sin(rayR);
        }
        if(!end)
            distances[i] = MAX_DEPTH;
        rayX = playerX;
        rayY = playerY;
    }
}

int main(void){
    int i;
    for(i = 0; i < w; i++)
        distances[i] = 0;

    screen = calloc(w * h, sizeof(char));
    skymap = calloc(w * (h / 2), sizeof(char));

    srand(time(NULL));
    for(i = 0; i < w * (h / 2); i++){
        if(rand()%55 == 2)
            skymap[i] = '*';
        else
            skymap[i] = ' ';
    }

    hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0);
    MSG message;

    while(1){
        PeekMessageA(&message, NULL, 0, 0, PM_REMOVE);
        castrays();
        newscreen();
        playerA = p.x;
        GetCursorPos(&p);
        printf("%s",screen);
    }
    free(screen);
    free(skymap);
}