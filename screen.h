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
extern double angles[];
extern int what[];
extern int offsets[];
extern char _gun[];

extern double guny;
extern double gunx;
extern double playerA;
extern double statuss;
extern int shet;
extern int shoot;
extern int hp;
extern int hits;

extern double radians (double a);

const double texturescale = texturex / tile;

const int enemyxscale = enemysizex / enemysize;
const int enemyyscale = enemysizey / enemysize;
int tx, ty;

char shading[] = "@%%#*+=-:. ";
char *skymap;
char *screen;

char texture1[texturex][texturey] = {
"## # ##",
"## # ##",
"   #   ",
"## # ##",
"## # ##",
"   #   ",
"## # ##",
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

double convert(double value,double From1,double From2,double To1,double To2)
{
      return (value-From1)/(From2-From1)*(To2-To1)+To1;
}

int flr(int val, int min, int max){
    val = (val > max) ? max : val;
    val = (val < min) ? min : val;
    return val;
}

void newscreen(void){
    int i, j, index, q = 0, q2 = 0;
    double coeff = depth / strlen(shading) - 1;
    double top, bottom;

    playerA = fmod(playerA, 360);

    for(j = 0; j < h; j++)
        for(i = 0; i < w; i++){

            top = (h / 2) * (1 - 1 / distances[i]) - (cameraz - 3.0);         //Top and bottom of the line
            bottom = (h / 2) * (1 + 1 / distances[i]) - (cameraz - 3.0);

            if(j >= top && j <= bottom){
                if(what[i] != 2)
                    index = flr(convert(j - top, 0, bottom - top, 0, texturex), 0, texturex - 1);    //Converting J variable to scale the line
                else
                    index = flr(convert(j - top, 0, bottom - top, 0, enemysizey - 1), 0, enemysizey - 1);    //Converting J variable to scale the lin

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
                    index = flr(distances[i] / coeff, 0, strlen(shading) - 1);  //Else standart shading 
                    screen[IX(i,j)] = shading[index];
                }
            }
            else
            if(j > bottom){
                double dy = j - (h / 2.0) + cameraz;
                double deg = radians(angles[i]);

                tx = playerX + cos(deg) * 33 / dy;
                ty = playerY + sin(deg) * 33 / dy;

                tx = flr((tx + (texturex-1))%(texturex-1), 0, texturex - 1);
                ty = flr((ty + (texturey-1))%(texturey-1), 0, texturey - 1);

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
                    re = reshet = 0;
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
                    shootshet = shoot = 0;
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


    /*STATISTICS*/
    char HPS[100], kills[100];

    itoa(hp, HPS, 10);
    strcat(HPS, " HP | ");
    for(i = 0; HPS[i] != '\0'; i++)
        screen[IX(i, 1)] = HPS[i];

    itoa((int)(hits / 10), kills, 10);
    strcat(kills, " KILLS");

    for(j = i; kills[j - i] != '\0'; j++)
        screen[IX(j, 1)] = kills[j - i];

    screen[w*h] = '\0'; //end of screen
    
    if(!shoot && !restart){
        guny = cos(shet * 0.2) * 2 + 16;     //Animating the gun
        exply = cos(shet * 0.2) * 2 + 23;     //Animating the explosion
    }

    if(restart)                                  //Restart gun
        if(gunx < gunsizex && guny < gunsizey){
            gunx += restartspeed;
            guny += restartspeed;
        }
        else {
            restart = 0;
            gunx = gunsizex;
        }

    if(shoot && se && !re && !restart){ //Check for hit to send
        if(what[(w/2)] == 2){
            statuss = 1;
            se = 0;
            hits++;
        }
    }
    else
        statuss = 0;

}