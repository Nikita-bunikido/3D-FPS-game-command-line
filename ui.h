#include <stdio.h>
#include <string.h>
#include "settings.h"

extern char* screen;

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
        for(int j = cy; j < cy + height; j++) //184 190
            screen[IX(i,j)] = getborder(width, height, i - cx, j - cy);

    print(getcenterpos(w, strlen(messagemain)), cy + 2, messagemain);
    print(getcenterpos(w, strlen(messagesecond)), cy + 5, messagesecond);
    return;
}