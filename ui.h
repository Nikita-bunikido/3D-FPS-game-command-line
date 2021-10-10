#include <stdio.h>
#include <string.h>
#include "settings.h"

extern char* screen;

typedef struct {
    int sizex;
    int sizey;
    char img[10000];
} Texture;

static enum bds {
    upleft,
    downleft,
    upright,
    downright
};

static enum lns {
    vertical,
    horizontal
};

void loadtexture(Texture* t, char path[]){
    FILE* f = fopen(path, "r");
    char c;
    int lenx, leny;
    lenx = leny = 0;
    for(size_t i = 0; !feof(f); ++i){
        c = fgetc(f);
        if ((c != EOF) && (c != '\n')){
            t->img[i] = c;
        }
        if (leny == 0)
            lenx++;
        if (c == '\n')
            leny++;
    }
    t->sizex = lenx;
    t->sizey = leny;
    fclose(f);
}

void drawtexture(Texture t, int _x, int _y){
    int i, j;
    for(i = _x; i < _x + t.sizex-1; i++)
        for(j = _y; j < _y + t.sizey+1; j++)
            screen[IX(i,j)] = t.img[(i - _x) + (j - _y) * t.sizex];
}

int getcenterpos(int gl, int ol){
    return (gl >> 1) - (ol >> 1);
}

void print(int x, int y, char* message){
    if (message == NULL) return;
    for(int i = 0; i < strlen(message); i++)
        screen[IX(i+x, y)] = message[i];
}

static unsigned char getborder(int width, int height, int i, int j){
    const unsigned char borders[4] = {
        213,     // up left
        212,     // down left
        184,     // up right
        190      // down right
    }, lines[2] = {
        205,     // vertical
        179      // horizonal
    };

    width--;
    height--;

    if (i == 0 && j == 0)
        return borders[upleft];
    if (i == width && j == 0)
        return borders[upright];
    if (i == 0 && j == height)
        return borders[downleft];
    if (i == width && j == height)
        return borders[downright];

    if (i == 0 || i == width)
        return lines[horizontal];
    if (j == 0 || j == height)
        return lines[vertical];
    return 32;
}

void message(int width, int height, char* messagemain, char* messagesecond){
    int cx, cy;
    cx = getcenterpos(w, width);
    cy = getcenterpos(h, height);
    for(int i = cx; i < cx + width; i++)
        for(int j = cy; j < cy + height; j++)
            screen[IX(i,j)] = getborder(width, height, i - cx, j - cy);

    print(getcenterpos(w, strlen(messagemain)), cy + 2, messagemain);
    print(getcenterpos(w, strlen(messagesecond)), cy + 5, messagesecond);
    return;
}

void drawloadingscreen(void){
    for(int i = 0; i < w; i++)
        for(int j = 0; j < h; j++)
            screen[IX(i,j)] = getborder(w, h-1, i, j-1);
    
    Texture logo;
    loadtexture(&logo, "RES\\logo.txt");
    drawtexture(logo, getcenterpos(w, logo.sizex) + 2, 5);
    print(getcenterpos(w, strlen(DESCRIPTION)), (h >> 1) + 2, DESCRIPTION);
    return;
}