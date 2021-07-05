/*****************************************
-Nikita Donskov 2021
*****************************************/

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>

#include "settings.h"
#include "gun.h"
#include "online.h"

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
extern int hp;
extern double statuss;
extern double radians (double a);

char shading[] = "@%%#*+=-:. ";
char *skymap;
char *screen;
char texture[] = "# # # # # ";

const double texturescale = texturex / tile;

const int enemyxscale = enemysizex / enemysize;
const int enemyyscale = enemysizey / enemysize;

char texture1[texturex][texturey] = {
"! ! ! !",
" # # # ",
"# #.;.#",
" # # # ",
"# # # #",
" # # # ",
" ^ ^ ~ ",
};

char texture2[texturex][texturey] = {
"#######",
"#     #",
"#  ~  #",
"# ~.~ #",
"#  ~  #",
"#  ;  #",
"#######"
};

char texture3[texturex][texturey] = {
"#$^;:. ",
"##*8&;.",
"@#7^&v ",
"$##&#. ",
"##8^@::",
"#;:,.,:",
"###^#;:"
};

char texture4[texturex][texturey] = {
"  ; ;  ",
" ;# #; ",
";##;##;",
";#####;",
" ;###; ",
"  ;#;  ",
"   ;   "
};

char floorr[texturex][texturey] = {
"_ _ _ _",
" ~ ~ ~ ",
"_ _ _ _",
" ~ ~ ~ ",
"_ _ _ _",
" ~ ~ ~ ",
"_ _ _ _"
};

char enemy[enemysizey][enemysizex] = {
" _ ",
"(o)",
" v ",
"-|-",
"/ \\",
"| |"
};

int tx, ty;

double convert(double value,double From1,double From2,double To1,double To2)
{
      return (value-From1)/(From2-From1)*(To2-To1)+To1;
}

void newscreen(void){
    int i, j, index, q = 0, q2 = 0;
    double coeff = depth / strlen(shading) - 1;
    double texturecoeff = h / strlen(texture) - 1;
    double top, bottom;


    playerA = fmod(playerA, 360);


    for(j = 0; j < h; j++)
        for(i = 0; i < w; i++){
            top = (h / 2) * (1 - 1 / distances[i]) - (cameraz - 3.0);         //Top and bottom of the line
            bottom = (h / 2) * (1 + 1 / distances[i]) - (cameraz - 3.0);

            if(j >= top && j <= bottom){

                if(what[i] != 2){
                    index = convert(j - top, 0, bottom - top, 0, texturex);    //Converting J variable to scale the line
                    if(index > texturex - 1)
                        index = texturex - 1;
                    if(index < 0)
                        index = 0;
                }
                else {
                    index = convert(j - top, 0, bottom - top, 0, enemysizey - 1);    //Converting J variable to scale the line
                    if(index > enemysizey - 2)
                        index = enemysizey - 2;
                    if(index < 0)
                        index = 0;
                }

                if(distances[i] < 5){                                       //Texture view distance
                    switch(what[i]){
                        case 0: screen[IX(i,j)] = texture1[index][offsets[i]]; break;        //Show piece of texture with offset
                        case 1: screen[IX(i,j)] = texture2[index][offsets[i]]; break;
                        case 2: screen[IX(i,j)] = enemy[index][offsets[i]]; break;
                        case 3: screen[IX(i,j)] = texture3[index][offsets[i]]; break;
                        case 4: screen[IX(i,j)] = texture4[index][offsets[i]]; break;
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

            if(!shoot){
                if(re){
                    if(reshet > retime){
                        gunx -= 5;
                        guny -= 5;
                    }
                    re = 0;
                    reshet = 0;
                    restart = 1;
                }
                if((i > w - gunx) && (j > h - guny)){  //If it is gun zone
                    if(_gun[q] != 'a')               //and if it is not alpha
                        screen[IX(i,j)] = _gun[q];   //drawing gun char and increment q
                    if(i != w - 1)
                        q++;
                    else
                        q += gunsizex - gunx + 1; 
                }
            }
            else
            {
                if(shootshet > shoottime){
                    shootshet = 0;
                    shoot = 0;
                    re = 1;
                }
                if((i > w - explx) && (j > h - exply)){  //If it is gun zone
                    if(_expl[q] != 'a')               //and if it is not alpha
                        screen[IX(i,j)] = _expl[q];   //drawing gun char and increment q
                    if(i != w - 1)
                        q++;
                    else
                        q += explsizex - explx + 1; 
                }
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

    /*hp*/
    char buffer[100];
    itoa(hp, buffer, 10);
    strcat(buffer, " HP");
    for(i = 0; buffer[i] != '\0'; i++)
        screen[IX(i, 1)] = buffer[i];

    screen[w*h] = '\0';
    
    if(!shoot && !restart){
        guny = cos(shet * 0.2) * 2 + 16;     //Animating the gun
        exply = cos(shet * 0.2) * 2 + 23;     //Animating the explosion
    }
    if(restart)
        if(gunx < gunsizex && guny < gunsizey){
            gunx += restartspeed;
            guny += restartspeed;
        }
        else {
            restart = 0;
            gunx = gunsizex;
        }
    if(shoot && se && !re && !restart){
        if(what[(w/2)] == 2){
            statuss = 1;
            se = 0;
        }
    }
    else
        statuss = 0;

}