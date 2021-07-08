/*****************************************
-Nikita Donskov 2021
*****************************************/

#include <stdio.h>

#define gunsizex 19
#define gunsizey 22
#define explsizex 28
#define explsizey 29

FILE *gun;
FILE *explos;
double guny = gunsizey;
double gunx = gunsizex;
int exply = explsizey;
int explx = explsizex;
int shet = 0;
int shoot = 0;
int shootshet = 0;
int re = 0;
int reshet = 0;
int restart = 0;
int se;
int hits = 0;
char _gun[gunsizex * gunsizey];
char _expl[explsizex * explsizey];

void gunsetup(void){
    int i;
    char c;
    i = 0;
    explos = fopen("RES/EXPL.txt", "r");
    gun = fopen("RES/GUN.txt", "r");
    while((c = fgetc(gun)) != EOF)
        if(c != '\n')
            _gun[i++] = c;        //Copy all file to _gun string
    _gun[i] = '\0';
    fclose(gun);

    i = 0;

    while((c = fgetc(explos)) != EOF)
        if(c != '\n')
            _expl[i++] = c;        //Copy all file to _expl string
    _expl[i] = '\0';
    fclose(explos);
}