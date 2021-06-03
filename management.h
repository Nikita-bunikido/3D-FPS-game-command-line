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

void update(int code){
    switch(code){
        case 87: {
            playerX += speed * cos(radians(playerA));
            playerY += speed * sin(radians(playerA));
        } break;
        case 83: {
            playerX -= speed * cos(radians(playerA));
            playerY -= speed * sin(radians(playerA));
        } break;
        case 65: {
            playerX += speed * sin(radians(playerA));
            playerY -= speed * cos(radians(playerA));
        } break;
        case 68: {
            playerX -= speed * sin(radians(playerA));
            playerY += speed * cos(radians(playerA));
        } break;
    }
}

LRESULT HookCallback(int nCode, WPARAM wParam, LPARAM lParam){
    if(nCode >= 0){
        if(wParam == WM_KEYDOWN){
            kbStruct = *((KBDLLHOOKSTRUCT*)lParam);
            if(kbStruct.vkCode != EXIT_CODE){
                update((int)kbStruct.vkCode);
            }
            else
                exit(1);
        }
    }
    return CallNextHookEx(hook, nCode, wParam, lParam);
}