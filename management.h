/*****************************************
-Nikita Donskov 2021
*****************************************/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "settings.h"

LRESULT HookCallback(int nCode, WPARAM wParam, LPARAM lParam);
HHOOK hook;
KBDLLHOOKSTRUCT kbStruct;
POINT p;

extern double radians (double a);
extern double playerX;
extern double playerY;
extern double playerA;

double cameraz = 3.00;
extern double gunx;
extern double guny;
extern int shoot;
extern int shet;
extern int re;
extern int restart;
extern double enx;
extern char cs;
extern int se;

void cameramove(void){
    cameraz = cos(shet * 0.2) * 2 + 3;
}

void update(void){ 

        if(GetKeyState(87) & 0x8000){
            playerX += speed * cos(radians(playerA));
            playerY += speed * sin(radians(playerA));
            cameramove();
        }
        if(GetKeyState(83) & 0x8000){
            playerX -= speed * cos(radians(playerA));
            playerY -= speed * sin(radians(playerA));
            cameramove();
        }
        if(GetKeyState(65) & 0x8000){
            playerX += speed * sin(radians(playerA));
            playerY -= speed * cos(radians(playerA));
            cameramove();
        }
        if(GetKeyState(68) & 0x8000){
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
}