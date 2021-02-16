#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>

#include "EndSocket.h"
#include "OpenSocket.h"
#include "Recv.h"
#include "Send.h"


int main(){
    int sock = OpenSocket("127.0.0.1", "8080");
    fputs("sleep\n", stderr);
    sleep(5);

    fputs("1\n", stderr);

    SendCommand(sock, "park", "world");

fputs("2\n", stderr);
    sleep(5);

fputs("3\n", stderr);

    RecvCommand(sock);

fputs("4\n", stderr);
    EndSocket(sock);

fputs("5\n", stderr);
}