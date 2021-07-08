/*****************************************
-Nikita Donskov 2021
*****************************************/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "settings.h"
#include "collision.h"

POINT p;

extern double playerX;
extern double playerY;
extern double playerA;

extern double gunx;
extern double guny;
extern double ban;
extern double enx;
extern int shoot;
extern int shet;
extern int re;
extern int restart;
extern int se;
extern int EXIT;
extern char cs;

double cameraz = 3.00;

extern double radians (double a);

void cameramove(void){
    cameraz = cos(shet * 0.2) * 2 + 3;
}

void update(void){
        if(GetKeyState(87) & 0x8000 && collision(playerX + (speed * coloffset) * cos(radians(playerA)), playerY + speed * sin(radians(playerA)))){
            playerX += speed * cos(radians(playerA));
            playerY += speed * sin(radians(playerA));
            cameramove();
        }
        if(GetKeyState(83) & 0x8000 && collision(playerX - (speed * coloffset) * cos(radians(playerA)), playerY - speed * sin(radians(playerA)))){
            playerX -= speed * cos(radians(playerA));
            playerY -= speed * sin(radians(playerA));
            cameramove();
        }
        if(GetKeyState(65) & 0x8000 && collision(playerX + (speed * coloffset) * cos(radians(playerA)), playerY - speed * sin(radians(playerA)))){
            playerX += speed * sin(radians(playerA));
            playerY -= speed * cos(radians(playerA));
            cameramove();
        }
        if(GetKeyState(68) & 0x8000 && collision(playerX - (speed * coloffset) * cos(radians(playerA)), playerY + speed * sin(radians(playerA)))){
            playerX -= speed * sin(radians(playerA));
            playerY += speed * cos(radians(playerA));
            cameramove();
        }
        if(GetKeyState(32) & 0x8000){
            if(!shoot && !re && !restart){
                shoot = 1;
                se = 1;
            }
        }
        if(GetKeyState('B') & 0x8000 && cs == 's')
            ban = 1;
        if(GetKeyState(EXIT_CODE) & 0x8000)
            EXIT = 1;
}