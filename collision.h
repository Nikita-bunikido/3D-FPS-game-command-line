/*****************************************
-Nikita Donskov 2021
*****************************************/

#include "settings.h"

extern char MAP[mapX][mapY];
extern double playerX;
extern double playerY;

double collision(double x, double y){
    int i, j;
    for(i = 0; i < mapY; i++)
        for(j = 0; j < mapX; j++){
            if((x > j * tile && x < j * tile + tile) && (y > i * tile && y < i * tile + tile) && 
            ((MAP[j][i] == '#') || (MAP[j][i] == '@') || (MAP[j][i] == '0') || (MAP[j][i] == 'h')))
                return 0;
        }
    return 1;
}