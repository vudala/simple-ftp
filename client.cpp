
#include "socket.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>

int main() {
    int sockfd = ConexaoRawSocket((char *) "lo");

    char buffer[1024];

    recv(sockfd, buffer, 30, MSG_WAITALL);

    buffer[29] = '\0';

    printf(buffer);   
}