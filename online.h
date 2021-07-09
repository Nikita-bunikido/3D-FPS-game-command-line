/*****************************************
-Nikita Donskov 2021
*****************************************/

#include <stdio.h>

double enx = tile + tile / 3;
double eny = tile + tile / 3;

WSADATA ws;
SOCKET s;
SOCKADDR_IN sa; 
SOCKET client_socket;

void onlinesetup(void){
    WSAStartup(MAKEWORD(2,2), &ws);

    s = socket(AF_INET, SOCK_STREAM, 0);

    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(1234);
}