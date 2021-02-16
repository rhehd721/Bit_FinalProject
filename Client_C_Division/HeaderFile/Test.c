#include "EndSocket.h"
#include "OpenSocket.h"
#include "Recv.h"
#include "Send.h"
#include <pthread.h>

int main(){
    int sock = OpenSocket("127.0.0.1", "8080");

    printf("%d", sock);
    printf("1");

    SendCommand(sock, "park", "hello");

    printf("2");

    RecvCommand(sock);

    printf("3");

    EndSocket(sock);

    printf("4");

}