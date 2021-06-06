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
int offsets[w];
int what[w];
double angles[w];
double wallX[w];
double wallY[w];

FILE *gun;
char _gun[418];
double guny = 16;
double gunx = 19;
int shet = 0;
int shoot = 0;

double radians (double a){
    return a * M_PI / 180;
}

double RAYCOLLISION(double x, double y, int ii){
    int i, j;
    for(i = 0; i < mapY; i++){
        for(j = 0; j < mapX; j++){
            if((x > j * tile && x < j * tile + tile) && (y > i * tile && y < i * tile + tile) && ((MAP[j][i] == '#') || (MAP[j][i] == '@'))){
                if((x > j * tile - 1 && x < j * tile + 1) && (x > j * tile + tile - 1 && x < j * tile + tile + 1))   //If it is horizontal
                    offsets[ii] = fmod(x, tile) * texturescale;                //Offset with horizontal.
                else
                    offsets[ii] = fmod(y, tile) * texturescale;                //Offset with vertical.
                switch(MAP[j][i]){
                    case '#': what[ii] = 0; break;
                    case '@': what[ii] = 1; break;
                }
                wallX[ii] = x;
                wallY[ii] = y;
                return sqrt(pow((double)(x - playerX), 2) + pow((double)(y - playerY), 2));
            }
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
        angles[i] = rayA;
        rayR = radians(rayA);
        for(s = 0; s < MAX_DEPTH; s++){
            end = 0;
            if((prom = RAYCOLLISION(rayX, rayY, i)) != -1){
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
    for(i = 0; i < w; i++){
        distances[i] = 0;
        offsets[i] = 0;
        what[i] = 0;
        angles[i] = 0;
        wallX[i] = 0;
        wallY[i] = 0;
    }

    screen = calloc(w * h, sizeof(char));
    skymap = calloc(w * (h / 2), sizeof(char));

    srand(time(NULL));
    for(i = 0; i < w * (h / 2); i++){
        if(rand()%55 == 2)
            skymap[i] = '*';
        else
            skymap[i] = ' ';
    }

    char c;
    i = 0;
    gun = fopen("RES/GUN.txt", "r");
    while((c = fgetc(gun)) != EOF)
        if(c != '\n')
            _gun[i++] = c;        //Copy all file to _gun string
    _gun[i] = '\0';
    fclose(gun);

    hook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0);
    MSG message;

    while(1){
        PeekMessageA(&message, NULL, 0, 0, PM_REMOVE);
        castrays();
        newscreen();
        playerA = p.x;
        GetCursorPos(&p);
        printf("%s",screen);
        shet++;
    }
    free(screen);
    free(skymap);
}