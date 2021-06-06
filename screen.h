/*****************************************
-Nikita Donskov 2021
*****************************************/

#include <stdio.h>
#include <time.h>
#include <string.h>

#include "settings.h"

extern const double DELTA_ANGLE;
extern double distances[];
extern char _gun[];
extern int offsets[];
extern double guny;
extern double gunx;
extern int shet;
extern int what[];
extern double playerA;
extern double angles[];
extern double wallX[];
extern double wallY[];
extern int shoot;
extern double radians (double a);

char shading[] = "@%%#*+=-:. ";
char *skymap;
char *screen;
char texture[] = "# # # # # ";

const int texturescale = texturex / tile;

char texture1[texturex][texturey] = {
"# # #",
" # # ",
"# # #",
" # # ",
"# # #",
};

char texture2[texturex][texturey] = {
"#####",
"#   #",
"#   #",
"#   #",
"#####"
};

char floorr[texturex][texturey] = {
"_ _ _",
" ~ ~ ",
"_ _ _",
" ~ ~ ",
"_ _ _"
};

int tx, ty;

double convert(double value,double From1,double From2,double To1,double To2)
{
      return (value-From1)/(From2-From1)*(To2-To1)+To1;
}

void newscreen(void){
    int i, j, index, q = 0;
    double coeff = depth / strlen(shading) - 1;
    double texturecoeff = h / strlen(texture) - 1;
    double top, bottom;
    for(j = 0; j < h; j++)
        for(i = 0; i < w; i++){
            top = (h / 2) * (1 - 1 / distances[i]);         //Top and bottom of the line
            bottom = (h / 2) * (1 + 1 / distances[i]);

            if(j >= top && j <= bottom){

                index = convert(j - top, 0, bottom - top, 0, texturex);    //Converting J variable to scale the line
                if(index > texturex - 1)
                    index = texturex - 1;
                if(index < 0)
                    index = 0;

                if(distances[i] < 6){                                       //Texture view distance
                    switch(what[i]){
                        case 0: screen[IX(i,j)] = texture1[index][offsets[i]]; break;        //Show piece of texture with offset
                        case 1: screen[IX(i,j)] = texture2[index][offsets[i]]; break;
                    }
                }
                else {
                    index = distances[i] / coeff;                          //Else standart shading 
                    if(index > strlen(shading) - 1)
                        index = strlen(shading) - 1;
                    if(index < 0)
                        index = 0;
                    screen[IX(i,j)] = shading[index];
                }
            }
            else
            if(j > bottom){
                double dy = j - (h / 2.0) + cameraz;
                double deg = radians(angles[i]);
                tx = playerX + cos(deg) * 33 / dy;
                ty = playerY + sin(deg) * 33 / dy;
                tx = (tx + (texturex-1))%(texturex-1);
                ty = (ty + (texturey-1))%(texturey-1); 

                if(tx > texturex - 1)
                    tx = texturex - 1;
                if(ty > texturey - 1)
                    ty = texturey - 1;
                if(tx < 0)
                    tx = 0;
                if(ty < 0)
                    ty = 0;
                screen[IX(i,j)] = floorr[tx][ty]; 
            }            //Floor in the second half of sreen
            else
                screen[IX(i,j)] = skymap[IX(i,j)]; //In the first skymap
            
            /*SOME POST-PROCESSING*/

            if((i > w - gunx) && (j > h - guny)){  //If it is gun zone
                if(_gun[q] != 'a')               //and if it is not alpha
                    screen[IX(i,j)] = _gun[q];   //drawing gun char and increment q
                if(i != w - 1)
                    q++;
                else
                    q += 19 - gunx + 1; 
            }

            /*Cursel*/
            if(i == (w / 2) && j == (h / 2))   //Simple cursel in the center of screen
                screen[IX(i,j)] = '+';
            if(i == (w / 2) && j == (h / 2)-1)
                screen[IX(i,j)] = '|';
            if(i == (w / 2) && j == (h / 2)+1)
                screen[IX(i,j)] = '|';
            if(i == (w / 2)-1 && j == (h / 2))
                screen[IX(i,j)] = '-';
            if(i == (w / 2)+1 && j == (h / 2))
                screen[IX(i,j)] = '-';

        }
    screen[w*h] = '\0';
    if(!shoot){
        guny = cos(shet * 0.02) * 2 + 16;     //Animating the gun
    }
    else{
        if(guny <= 15)
            guny += 0.1;
        if(gunx <= 18){
            gunx += 0.1;
        } else
            shoot = 0;
    }
}