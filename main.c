/*****************************************
This is a implementation of pseudo-3d game
in command promt.
                      -Nikita Donskov 2021
*****************************************/

#include <stdio.h>
#include <winsock2.h>
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
char cs;
int hp = 100;
double statuss = 0;

double radians (double a){
    return a * M_PI / 180;
}

double RAYCOLLISION(double x, double y, int ii){
    int i, j;
    if(x >= enx && x <= enx + enemysize && y >= eny && y <= eny + enemysize && cs != 'o'){
        what[ii] = 2;
        if((x > enx - 0.01 && x < enx + enemysize + 0.01) && (x > enx + enemysize - 0.01 && x < enx + enemysize + 0.01))
            offsets[ii] = fmod(x, enemysize) * enemyxscale;
        else
            offsets[ii] = fmod(y, enemysize) * enemyxscale;
        return sqrt(pow((double)(x - playerX), 2) + pow((double)(y - playerY), 2));
    }

    for(i = 0; i < mapY; i++){
        for(j = 0; j < mapX; j++){
            if((x > j * tile && x < j * tile + tile) && (y > i * tile && y < i * tile + tile) && ((MAP[j][i] == '#') || (MAP[j][i] == '@') || (MAP[j][i] == '0') || (MAP[j][i] == 'h'))){
                if((x > j * tile - 1 && x < j * tile + 1) && (x > j * tile + tile - 1 && x < j * tile + tile + 1))   //If it is horizontal
                    offsets[ii] = fmod(x, tile) * texturescale;                //Offset with horizontal.
                else
                    offsets[ii] = fmod(y, tile) * texturescale;                //Offset with vertical.
                switch(MAP[j][i]){
                    case '#': what[ii] = 0; break;
                    case '@': what[ii] = 1; break;
                    case '0': what[ii] = 3; break;
                    case 'h': what[ii] = 4; break;
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

    gunsetup();

    printf("NSHOOT v3.0.0\n");
    printf("Client - c\nServer - s\nOffline - o\n");
    cs = getchar();

    if(cs == 'c' || cs == 's'){
        onlinesetup();
    }

        double data[4];
        char stringh[1000];


    if(cs == 'c'){
        printf("Server IP:\n");
        scanf("\n%s", stringh);
        sa.sin_addr.S_un.S_addr = inet_addr(stringh);
        connect(s, &sa, sizeof(sa));
    }
    else
    if(cs == 's'){

        bind(s, &sa, sizeof(sa));
        listen(s, 100);

        SOCKADDR_IN client_addr;

        int client_addr_size = sizeof(client_addr);


        while(!(client_socket = accept(s, &client_addr, &client_addr_size)))
            ;
    }

    while(1){
        //                cameraz = (p.y - 300) / 50.0;
        if(cameraz <= 3)
            cameraz = 3;
        castrays();
        newscreen();
        playerA = p.x;
        GetCursorPos(&p);
        if(cs == 'c'){
            data[0] = playerX;
            data[1] = playerY;
            data[2] = statuss;
            send(s, data, sizeof(data), 0);
            recv(s, data, sizeof(data), 0);
            enx = data[0];
            eny = data[1];
            if(data[2]){
                hp -= 10;
                playerX += 0.1;
                playerY += 0.1;
            }
        }
        else if(cs == 's'){
            if(recv(client_socket, data, sizeof(data), 0) > 0){
                //printf("%f %f\n", data[0], data[1]);
                enx = data[0];
                eny = data[1];
                if(data[2]){
                    hp -= 10;
                    playerX += 0.1;
                    playerY += 0.1;
                }
                data[0] = playerX;
                data[1] = playerY;
                data[2] = statuss;
                send(client_socket, data, sizeof(data), 0);
            }
        }
        printf("%s",screen);
        shet++;
        if(shoot)
            shootshet++;
        else
            reshet++;
        update();
        if(hp <= 0){
            hp = 100;
            playerX = tile + tile / 3;
            playerY = tile + tile / 3;
        }
        Sleep(20);
    }
    free(screen);
    free(skymap);
} 